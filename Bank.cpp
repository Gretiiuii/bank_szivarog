#include "Bank.h"
#include "Szamla.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>

#include "memtrace.h"

Bank::Bank() {}

Bank::~Bank() {
    for (size_t i = 0; i < ugyfelek.meret(); ++i) {
        delete ugyfelek[i];
    }
    for (size_t i = 0; i < szamlak.meret(); ++i) {
        delete szamlak[i];
    }
    for (size_t i = 0; i < tranzakciok.meret(); ++i) {
        delete tranzakciok[i];
    }
}

std::string Bank::generalID(const std::string& prefix, size_t szam) {
    std::ostringstream oss;
    oss << prefix << std::setw(3) << std::setfill('0') << szam;
    return oss.str();
}

// Ügyfelek
void Bank::ugyfelRegisztral(const std::string& nev, const std::string& tel, const std::string& cim) {
    if (!Ugyfel::ervenyesTelefon(tel)) {
        throw std::invalid_argument("Ervenytelen telefonszam formatum.");
    }

    std::string id = generalID("CUST", ugyfelek.meret() + 1);

    Ugyfel* uj = new Ugyfel(id, nev, tel, cim);
    ugyfelek.hozzaad(uj);
}

Ugyfel* Bank::ugyfelKeres(const std::string& azonosito) {
    if (azonosito.rfind("CUST", 0) == 0) {
        // Keresés ügyfél ID alapján
        for (size_t i = 0; i < ugyfelek.meret(); ++i) {
            if (azonosito == ugyfelek[i]->getID()) {
                return ugyfelek[i];
            }
        }
    } else if (azonosito.rfind("ACC", 0) == 0) {
        // Keresés számlaszám alapján
        for (size_t i = 0; i < szamlak.meret(); ++i) {
            if (azonosito == szamlak[i]->getSzamlaSzam()) {
                // számlához tartozó ügyfél keresése
                const char* tulajID = szamlak[i]->getUgyfelID().c_str();
                for (size_t j = 0; j < ugyfelek.meret(); ++j) {
                    if (strcmp(ugyfelek[j]->getID().c_str(), tulajID) == 0) {
                        return ugyfelek[j];
                    }
                }
            }
        }
    }

    return nullptr;
}


const DinamikusTomb<Ugyfel*>& Bank::getUgyfelek() const {
    return ugyfelek;
}

void Bank::beallitottUgyfelek(DinamikusTomb<Ugyfel*>& lista) {
    for (size_t i = 0; i < lista.meret(); ++i) {
        ugyfelek.hozzaad(lista[i]);
    }
}

bool Bank::modositUgyfel(const std::string& id, const std::string& ujNev, const std::string& ujTel, const std::string& ujCim) {
    Ugyfel* u = nullptr;
    for (size_t i = 0; i < ugyfelek.meret(); ++i) {
        if (ugyfelek[i]->getID() == id) {
            u = ugyfelek[i];
            break;
        }
    }
    if (!u) {
        return false;
    }
    if (!ujTel.empty() && !Ugyfel::ervenyesTelefon(ujTel)) {
        throw std::invalid_argument("Ervenytelen uj telefonszam formatum.");
    }
    if (!ujNev.empty()) u->setNev(ujNev);
    if (!ujTel.empty()) u->setTelefon(ujTel);
    if (!ujCim.empty()) u->setCim(ujCim);
    return true;
}

bool Bank::torolUgyfel(const std::string& id) {
    for (size_t i = 0; i < ugyfelek.meret(); ++i) {
        if (ugyfelek[i]->getID() == id) {
            delete ugyfelek[i];
            ugyfelek.eltavolit(i);
            return true;
        }
    }
    return false;
}

void Bank::ellenorizSzamlaAdatok(SzamlaTipus tipus, double egyenleg, const std::string& diakIgazolvany) const {
    if (tipus == SzamlaTipus::PremiumSzamla && egyenleg < PremiumSzamla::getMinEgyenleg()) {
        throw std::invalid_argument("Nem eri el a minimum nyitasi egyenleget.");
    }
    if (tipus == SzamlaTipus::DiakSzamla && !DiakSzamla::ervenyesDiakIgazolvany(diakIgazolvany)) {
        throw std::invalid_argument("Ervenytelen diakigazolvany szam.");
    }
}


Szamla* Bank::letrehozSzamlat(SzamlaTipus tipus, const std::string& szamlaszam, const std::string& ugyfelID, double egyenleg, const Date& nyitas, const std::string& diakIgazolvany) {
    switch (tipus) {
        case SzamlaTipus::FolyoSzamla:
            return new FolyoSzamla(szamlaszam, ugyfelID, egyenleg, nyitas);
        case SzamlaTipus::TakarekSzamla:
            return new TakarekSzamla(szamlaszam, ugyfelID, egyenleg, TakarekSzamla::getKamatlab(), nyitas);
        case SzamlaTipus::PremiumSzamla:
            return new PremiumSzamla(szamlaszam, ugyfelID, egyenleg, PremiumSzamla::getKamatlab(), nyitas);
        case SzamlaTipus::DiakSzamla:
            return new DiakSzamla(szamlaszam, ugyfelID, egyenleg, nyitas, diakIgazolvany);
        default:
            throw std::invalid_argument("Ismeretlen szamlatipus");
    }
}


void Bank::szamlaNyit(const std::string& ugyfelID, const std::string& tipus, double egyenleg, const Date& nyitas, const std::string& diakIgazolvany) {
    std::string szamlaszam = generalID("ACC", szamlak.meret() + 1);
    SzamlaTipus tipusEnum = szamlaTipusFromString(tipus);
    ellenorizSzamlaAdatok(tipusEnum, egyenleg, diakIgazolvany);
    Szamla* uj = letrehozSzamlat(tipusEnum, szamlaszam, ugyfelID, egyenleg, nyitas, diakIgazolvany);
    szamlak.hozzaad(uj);
}


Szamla* Bank::szamlaKeres(const std::string& szamlaszam) const{
    for (size_t i = 0; i < szamlak.meret(); ++i) {
        if (szamlak[i]->getSzamlaSzam() == szamlaszam) {
            return szamlak[i];
        }
    }
    return nullptr;
}

const DinamikusTomb<Szamla*>& Bank::getSzamlak() const {
    return szamlak;
}

void Bank::beallitottSzamlak(DinamikusTomb<Szamla*>& lista) {
    for (size_t i = 0; i < lista.meret(); ++i) {
        szamlak.hozzaad(lista[i]);
    }
}

bool Bank::szamlaZaras(const std::string& szamlaszam) {
    for (size_t i = 0; i < szamlak.meret(); ++i) {
        if (szamlak[i]->getSzamlaSzam() == szamlaszam) {
            delete szamlak[i];
            szamlak.eltavolit(i);
            return true;
        }
    }
    return false;
}

double Bank::egyenlegLekerdez(const std::string& szamlaszam) const {
    Szamla* sz = szamlaKeres(szamlaszam);
    if (!sz) {
        throw std::out_of_range("A megadott szamla nem talalhato.");
    }
    return sz->getEgyenleg();
}

Tranzakcio* Bank::letrehozTranzakcio(TranzakcioTipus tipus, const std::string& szamlaszam, double osszeg, Szamla* celSzamla) {
    Szamla* forrasSzamla = szamlaKeres(szamlaszam);
    std::string id = generalID("TRANS", tranzakciok.meret() + 1);
    Date datum = Date::ma();

    if (!forrasSzamla) throw std::invalid_argument("Ervenytelen forras szamla");

    switch (tipus) {
        case TranzakcioTipus::Befizetes:
            return new Befizetes(id, forrasSzamla, osszeg, datum);
        case TranzakcioTipus::Kivetel:
            return new Kivetel(id, forrasSzamla, osszeg, datum);
        case TranzakcioTipus::Atutalas:
            return new Atutalas(id, forrasSzamla, celSzamla, osszeg, datum);
        default:
            throw std::invalid_argument("Ismeretlen tranzakcio tipus.");
    }
}


void Bank::vegrehajtTranzakcio(const std::string& szamlaszam, TranzakcioTipus tipus, double osszeg, const std::string& celSzamla) {
    Szamla* forrasSzamla = szamlaKeres(szamlaszam);
    if (!forrasSzamla) {
        throw std::out_of_range("A forras szamla nem talalhato.");
    }

    Szamla* celSzamlaPtr = (tipus == TranzakcioTipus::Atutalas) ? szamlaKeres(celSzamla) : nullptr;
    if (tipus == TranzakcioTipus::Atutalas && !celSzamlaPtr) {
        throw std::out_of_range("A cel szamla nem talalhato.");
    }

    Tranzakcio* tranzakcio = letrehozTranzakcio(tipus, szamlaszam, osszeg, celSzamlaPtr);
    if (!tranzakcio->vegrehajt()) {
        delete tranzakcio;
        throw std::runtime_error("A tranzakcio nem hajthato vegre.");
    }

    tranzakciok.hozzaad(tranzakcio);
}

const DinamikusTomb<Tranzakcio*>& Bank::getTranzakciok() const {
    return tranzakciok;
}

void Bank::beallitottTranzakciok(DinamikusTomb<Tranzakcio*>& lista) {
    for (size_t i = 0; i < lista.meret(); ++i) {
        tranzakciok.hozzaad(lista[i]);
    }
}

// Jelentések
void Bank::napiJelentes(const Date& datum) const {
    std::cout << "Napi tranzakciok (" << datum.toStr() << "):\n";

    bool talalat = false;
    for (size_t i = 0; i < tranzakciok.meret(); ++i) {
        if (tranzakciok[i]->getDatum() == datum) {
            talalat = true;
            std::cout << "ID: " << tranzakciok[i]->getID()
                      << ", Szamlaszam: " << tranzakciok[i]->getSzamlaSzam()
                      << ", Tipus: " << stringTranzakcioTipus(tranzakciok[i]->getTipus())
                      << ", Osszeg: " << tranzakciok[i]->getOsszeg()
                      << ", Datum: " << tranzakciok[i]->getDatum().toStr()
                      << "\n";
        }
    }

    if (!talalat) {
        std::cout << "Ezen a napon nem tortent tranzakcio.\n";
    }
}

void Bank::kamatJelentes(const std::string& szamlaszam, int ev) const {
    const Szamla* sz = szamlaKeres(szamlaszam);
    if (!sz) {
        throw std::out_of_range("A megadott szamla nem talalhato.");
    }

    double jovobeli = sz->kamatSzamol(ev);

    if (sz->getTipus() == "FolyoSzamla" || sz->getTipus() == "DiakSzamla") {
        std::cout << "Ez a szamlatipus nem kamatozik.\n";
    } else {
        std::cout << "Varhato egyenleg " << ev << " ev mulva: " << static_cast<int>(jovobeli) << " Ft\n";
    }
}
