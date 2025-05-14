#include "FajlKezelo.h"
#include "Tranzakcio.h"
#include "Szamla.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

DinamikusTomb<Ugyfel*> FajlKezelo::betoltUgyfeleket(const char* fajlnev) {
    DinamikusTomb<Ugyfel*> lista;
    std::ifstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni az ügyfél fájlt: " + std::string(fajlnev));
    }

    std::string sor;
    std::getline(fajl, sor); // fejléc

    while (std::getline(fajl, sor)) {
        std::stringstream ss(sor);
        std::string id, nev, tel, cim;

        std::getline(ss, id, '\t');
        std::getline(ss, nev, '\t');
        std::getline(ss, tel, '\t');
        std::getline(ss, cim);

        if (!id.empty() && !nev.empty() && !tel.empty() && !cim.empty()) {
            if (Ugyfel::ervenyesTelefon(tel.c_str())) {
                lista.hozzaad(new Ugyfel(id, nev, tel, cim));
            } else {
                std::cerr << "Figyelem: Érvénytelen telefonszám, ügyfél kihagyva: " << id << " (" << tel << ")\n";
            }
        }
    }

    return lista;
}

void FajlKezelo::mentUgyfelek(const DinamikusTomb<Ugyfel*>& ugyfelek, const char* fajlnev) {
    std::ofstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni az ügyfél mentési fájlt: " + std::string(fajlnev));
    }

    fajl << "ugyfelID\tnev\ttelefon\tcim\n";

    for (size_t i = 0; i < ugyfelek.meret(); ++i) {
        Ugyfel* u = ugyfelek[i];
        fajl << u->getID() << "\t" << u->getNev() << "\t" << u->getTelefon() << "\t" << u->getCim() << "\n";
    }
}

void FajlKezelo::mentSzamlak(const DinamikusTomb<Szamla*>& szamlak, const char* fajlnev) {
    std::ofstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni a számlák mentési fájlját: " + std::string(fajlnev));
    }

    fajl << "szamlaszam\ttipus\tugyfelID\tegyenleg\tnyitasDatum\textra\n";

    for (size_t i = 0; i < szamlak.meret(); ++i) {
        Szamla* sz = szamlak[i];
        fajl << sz->getSzamlaSzam() << "\t"
             << sz->getTipus() << "\t"
             << sz->getUgyfelID() << "\t"
             << sz->getEgyenleg() << "\t"
             << sz->getNyitas().toStr() << "\t"
             << sz->getExtraAdat() << "\n";
    }
}

DinamikusTomb<Szamla*> FajlKezelo::betoltSzamlakat(const char* fajlnev) {
    DinamikusTomb<Szamla*> lista;
    std::ifstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni a számla fájlt: " + std::string(fajlnev));
    }

    std::string sor;
    std::getline(fajl, sor); // fejléc

    while (std::getline(fajl, sor)) {
        std::stringstream ss(sor);
        std::string szamlaszam, tipus, ugyfelID, egyenlegStr, datumStr, extra;

        std::getline(ss, szamlaszam, '\t');
        std::getline(ss, tipus, '\t');
        std::getline(ss, ugyfelID, '\t');
        std::getline(ss, egyenlegStr, '\t');
        std::getline(ss, datumStr, '\t');
        std::getline(ss, extra);

        double egyenleg;
        try {
            egyenleg = std::stod(egyenlegStr);
        } catch (...) {
            std::cerr << "Hibás egyenleg: " << egyenlegStr << "\n";
            continue;
        }

        Date nyitas;
        if (!nyitas.parseFromStr(datumStr.c_str())) {
            std::cerr << "Hibás dátumformátum: " << datumStr << "\n";
            continue;
        }

        if (tipus == "DiakSzamla" && extra != "0") {
            lista.hozzaad(new DiakSzamla(szamlaszam, ugyfelID, egyenleg, nyitas, extra));
        } else if (tipus == "FolyoSzamla") {
            lista.hozzaad(new FolyoSzamla(szamlaszam, ugyfelID, egyenleg, nyitas));
        } else if (tipus == "TakarekSzamla") {
            lista.hozzaad(new TakarekSzamla(szamlaszam, ugyfelID, egyenleg, TakarekSzamla::getKamatlab(), nyitas));
        } else if (tipus == "PremiumSzamla") {
            lista.hozzaad(new PremiumSzamla(szamlaszam, ugyfelID, egyenleg, PremiumSzamla::getKamatlab(), nyitas));
        } else {
            throw std::logic_error("Ismeretlen számlatípus: " + tipus);
        }
    }

    return lista;
}

DinamikusTomb<Tranzakcio*> FajlKezelo::betoltTranzakciok(const char* fajlnev, const DinamikusTomb<Szamla*>& szamlak) {
    DinamikusTomb<Tranzakcio*> lista;
    std::ifstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni a tranzakciós fájlt: " + std::string(fajlnev));
    }

    std::string sor;
    std::getline(fajl, sor); // fejléc

    while (std::getline(fajl, sor)) {
        std::stringstream ss(sor);
        std::string id, tipus, szamlaszam, osszegStr, datumStr, celSzamlaStr;

        std::getline(ss, id, '\t');
        std::getline(ss, tipus, '\t');
        std::getline(ss, szamlaszam, '\t');
        std::getline(ss, osszegStr, '\t');
        std::getline(ss, datumStr, '\t');
        std::getline(ss, celSzamlaStr);

        double osszeg;
        try {
            osszeg = std::stod(osszegStr);
        } catch (...) {
            std::cerr << "Hibás összeg: " << osszegStr << "\n";
            continue;
        }

        Date datum;
        if (!datum.parseFromStr(datumStr.c_str())) {
            std::cerr << "Hibás dátum: " << datumStr << "\n";
            continue;
        }

        Szamla* forras = nullptr;
        Szamla* cel = nullptr;

        for (size_t i = 0; i < szamlak.meret(); ++i) {
            Szamla* sz = szamlak[i];
            if (!forras && sz->getSzamlaSzam() == szamlaszam)
                forras = sz;
            if (tipus == "Atutalas" && !cel && sz->getSzamlaSzam() == celSzamlaStr)
                cel = sz;
            if (forras && (tipus != "Atutalas" || cel))
                break;
        }

        if (!forras || (tipus == "Atutalas" && !cel)) {
            std::cerr << "Figyelem: Ismeretlen számlaszám(ok): " << szamlaszam << ", " << celSzamlaStr << "\n";
            continue;
        }

        if (tipus == "Befizetes") {
            lista.hozzaad(new Befizetes(id, forras, osszeg, datum));
        } else if (tipus == "Kivetel") {
            lista.hozzaad(new Kivetel(id, forras, osszeg, datum));
        } else if (tipus == "Atutalas") {
            lista.hozzaad(new Atutalas(id, forras, cel, osszeg, datum));
        } else {
            throw std::logic_error("Ismeretlen tranzakció típus: " + tipus);
        }
    }

    return lista;
}

void FajlKezelo::mentTranzakciok(const DinamikusTomb<Tranzakcio*>& tranzakciok, const char* fajlnev) {
    std::ofstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni a tranzakció mentési fájlt: " + std::string(fajlnev));
    }

    fajl << "tranzakcioID\ttipus\tszamlaszam\tosszeg\tdatum\tcelszamla\n";

    for (size_t i = 0; i < tranzakciok.meret(); ++i) {
        Tranzakcio* t = tranzakciok[i];
        fajl << t->getID() << "\t"
             << stringTranzakcioTipus(t->getTipus()) << "\t"
             << t->getSzamlaSzam() << "\t"
             << t->getOsszeg() << "\t"
             << t->getDatum().toStr() << "\t"
             << t->getCelSzamla() << "\n";
    }
}
