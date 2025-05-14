#include "KonzolosUI.h"
#include <iomanip>
#include <string>
#include <limits>

#include "memtrace.h"

void KonzolosUI::foMenu(Bank& bank) {
    int valasztas;
    do {
        std::cout << "\nBANKI RENDSZER\n";
        std::cout << "║ 1. Ugyfelkezeles\n";
        std::cout << "║ 2. Szamlakezeles\n";
        std::cout << "║ 3. Tranzakciok\n";
        std::cout << "║ 4. Jelentesek\n";
        std::cout << "║ 0. Kilepes\n";
        std::cout << "Valasszon menupontot: ";

        while (!(std::cin >> valasztas)) {
            std::cout << "Ervenytelen bemenet. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        switch (valasztas) {
        case 1:
            ugyfelKezelesMenu(bank);
            break;
        case 2:
            szamlaMuveletekMenu(bank);
            break;
        case 3:
            tranzakcioMenu(bank);
            break;
        case 4:
            jelentesMenu(bank);
            break;
        case 0:
            std::cout << "Kilepes...\n";
            break;
        default:
            std::cout << "Hiba: Ervenytelen valasztas!\n";
        }
    } while (valasztas != 0);
}

void KonzolosUI::ugyfelKezelesMenu(Bank& bank) {
    int valasztas = -1;
    while (valasztas != 0) {
        std::cout << "\nUGYFELKEZELES\n";
        std::cout << "│ 1. Uj ugyfel regisztracio\n";
        std::cout << "│ 2. Ugyfel adat modositas\n";
        std::cout << "│ 3. Ugyfel torlese\n";
        std::cout << "│ 4. Kereses\n";
        std::cout << "│ 0. Vissza a fomenube\n";
        std::cout << "Valasszon menupontot: ";

        while (!(std::cin >> valasztas)) {
            std::cout << "Ervenytelen bemenet. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        switch (valasztas) {
        case 1:
            ugyfelRegisztracio(bank);
            break;
        case 2:
            ugyfelModositas(bank);
            break;
        case 3:
            ugyfelTorles(bank);
            break;
        case 4:
            ugyfelKereses(bank);
            break;
        case 0:
            std::cout << "Vissza a fomenube...\n";
            break;
        default:
            std::cout << "Ervenytelen menupont.\n";
        }
    }
}

void KonzolosUI::ugyfelRegisztracio(Bank& bank) {
    std::string nev, tel, cim;
    std::cout << "Nev: ";
    std::getline(std::cin, nev);
    std::cout << "Telefonszam (+36...): ";
    std::getline(std::cin, tel);
    std::cout << "Lakcim: ";
    std::getline(std::cin, cim);

    try {
        bank.ugyfelRegisztral(nev, tel, cim);
        std::cout << "Ugyfel sikeresen regisztralva: CUST"
                  << std::setw(3) << std::setfill('0') << bank.getUgyfelek().meret() << "\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Hiba: " << e.what() << std::endl;
    }
}

void KonzolosUI::ugyfelModositas(Bank& bank) {
    std::string id;
    std::cout << "Melyik ugyfelet szeretne modositani? Adja meg az ugyfel azonositot (pl. CUST001): ";
    std::getline(std::cin, id);

    std::string ujNev, ujTel, ujCim;
    std::cout << "Uj nev (hagyja uresen, ha nem valtozik): ";
    std::getline(std::cin, ujNev);
    std::cout << "Uj telefonszam (hagyja uresen, ha nem valtozik): ";
    std::getline(std::cin, ujTel);
    std::cout << "Uj lakcim (hagyja uresen, ha nem valtozik): ";
    std::getline(std::cin, ujCim);

    try {
        bool siker = bank.modositUgyfel(id, ujNev, ujTel, ujCim);
        if (siker) {
            std::cout << "Ugyfel adatai sikeresen modositva.\n";
        } else {
            std::cout << "Nem talalhato ilyen azonositoju ugyfel.\n";
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Hiba: " << e.what() << std::endl;
    }
}

void KonzolosUI::ugyfelTorles(Bank& bank) {
    std::string id;
    std::cout << "Adja meg a torlendo ugyfel azonositojat: ";
    std::getline(std::cin, id);

    bool siker = bank.torolUgyfel(id);
    if (siker) {
        std::cout << "Ugyfel sikeresen torolve.\n";
    } else {
        std::cout << "Nem talalhato ilyen azonositoju ugyfel.\n";
    }
}

void KonzolosUI::ugyfelKereses(Bank& bank) {
    std::cout << "Kerem adja meg az ugyfelazonositot vagy szamlaszamot (pl. CUST001 vagy ACC001): ";
    std::string azonosito;
    std::getline(std::cin, azonosito);

    Ugyfel* talalt = bank.ugyfelKeres(azonosito);

    if (talalt) {
        std::cout << "Ugyfel megtalalva:\n";
        std::cout << "ID: " << talalt->getID() << "\n";
        std::cout << "Nev: " << talalt->getNev() << "\n";
        std::cout << "Telefon: " << talalt->getTelefon() << "\n";
        std::cout << "Cim: " << talalt->getCim() << "\n";
    } else {
        std::cout << "Nem talalhato ilyen azonositoval rendelkezo ugyfel.\n";
    }
}



void KonzolosUI::szamlaMuveletekMenu(Bank& bank) {
    int valasztas;
    do {
        std::cout << "\nSZAMLAKEZELES\n";
        std::cout << "│ 1. Uj szamla nyitasa\n";
        std::cout << "│ 2. Szamla lezarasa\n";
        std::cout << "│ 3. Egyenleg lekerdezese\n";
        std::cout << "│ 0. Vissza a fomenube\n";
        std::cout << "Valasszon menupontot: ";

        while (!(std::cin >> valasztas)) {
            std::cout << "Ervenytelen bemenet. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        switch (valasztas) {
        case 1:
            szamlaNyitasMenu(bank);
            break;
        case 2:
            szamlaZarasMenu(bank);
            break;
        case 3:
            egyenlegLekerdezMenu(bank);
            break;
        case 0:
            break;
        default:
            std::cout << "Hibas valasztas!\n";
        }
    } while (valasztas != 0);
}

void KonzolosUI::szamlaNyitasMenu(Bank& bank) {
    std::string ugyfelID, tipus, diakIgazolvany;
    double egyenleg;

    std::cout << "Ugyfel ID: ";
    std::getline(std::cin, ugyfelID);
    std::cout << "Szamlatipus (FolyoSzamla / TakarekSzamla / PremiumSzamla / DiakSzamla): ";
    std::getline(std::cin, tipus);
    std::cout << "Kezdo egyenleg: ";
    std::cin >> egyenleg;
    std::cin.ignore();

    if (tipus == "DiakSzamla") {
        std::cout << "Diakigazolvany szama: ";
        std::getline(std::cin, diakIgazolvany);
    }

    try {
        bank.szamlaNyit(ugyfelID, tipus, egyenleg, Date::ma(), diakIgazolvany);
        std::cout << "Szamla sikeresen megnyitva: ACC"
                  << std::setw(3) << std::setfill('0') << bank.getSzamlak().meret() << "\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Hiba: " << e.what() << std::endl;
    }
}

void KonzolosUI::szamlaZarasMenu(Bank& bank) {
    std::string szamlaID;
    std::cout << "Adja meg a torlendo szamla azonositojat: ";
    std::getline(std::cin, szamlaID);

    bool siker = bank.szamlaZaras(szamlaID);
    if (siker) {
        std::cout << "Szamla sikeresen lezarva (torolve).\n";
    } else {
        std::cout << "Nem talalhato ilyen azonositoju szamla.\n";
    }
}

void KonzolosUI::egyenlegLekerdezMenu(Bank& bank) {
    std::string szamlaszam;
    std::cout << "Adja meg a szamlaszamot: ";
    std::getline(std::cin, szamlaszam);

    try {
        double egyenleg = bank.egyenlegLekerdez(szamlaszam);
        std::cout << "Aktualis egyenleg: " << egyenleg << " Ft\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Hiba: " << e.what() << std::endl;
    }
}

void KonzolosUI::tranzakcioMenu(Bank& bank) {
    try {
        int valasztas;
        std::cout << "\nTRANZAKCIOK\n"
                  << "│ 1. Befizetes\n"
                  << "│ 2. Kivetel\n"
                  << "│ 3. Atutalas\n"
                  << "│ 0. Vissza a fomenube\n"
                  << "Valassz: ";

        while (!(std::cin >> valasztas)) {
            std::cout << "Ervenytelen bemenet. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        switch (valasztas) {
            case 1:
                tranzakcioVegrehajtas(bank, TranzakcioTipus::Befizetes, false);
            break;
            case 2:
                tranzakcioVegrehajtas(bank, TranzakcioTipus::Kivetel, false);
            break;
            case 3:
                tranzakcioVegrehajtas(bank, TranzakcioTipus::Atutalas, true);
            break;
            case 0:
                std::cout << "Vissza a fomenube...\n";
            break;
            default:
                std::cout << "Ervenytelen valasztas!\n";
            break;
        }
    } catch (const std::exception& e) {
        std::cout << "Hiba: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Ismeretlen hiba a tranzakcios menu feldolgozasa kozben.\n";
    }
}

void KonzolosUI::tranzakcioVegrehajtas(Bank& bank, TranzakcioTipus tipus, bool atutalasos) {
    std::string szamla1, szamla2;
    double osszeg;

    try {
        if (atutalasos) {
            std::cout << "Forras szamla: ";
            std::getline(std::cin, szamla1);
            std::cout << "Cel szamla: ";
            std::getline(std::cin, szamla2);
        } else {
            std::cout << "Szamlaszam: ";
            std::getline(std::cin, szamla1);
        }

        std::cout << "Osszeg: ";
        while (!(std::cin >> osszeg)) {
            std::cout << "Ervenytelen osszeg. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        if (atutalasos)
            bank.vegrehajtTranzakcio(szamla1, tipus, osszeg, szamla2);
        else
            bank.vegrehajtTranzakcio(szamla1, tipus, osszeg, "");

        std::cout << "Tranzakcio sikeres.\n";
    } catch (const std::exception& e) {
        std::cout << "Hiba: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Ismeretlen hiba tortent a tranzakcio soran.\n";
    }
}


void KonzolosUI::jelentesMenu(Bank& bank) {
    while (true) {
        std::cout << "\nJELENTESEK\n"
                  << "│ 1. Napi tranzakciok\n"
                  << "│ 2. Kamatszamitas\n"
                  << "│ 0. Vissza a fomenube\n"
                  << "Valasszon menupontot: ";

        int valasztas;

        while (!(std::cin >> valasztas)) {
            std::cout << "Ervenytelen bemenet. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        if (valasztas == 0) break;

        if (valasztas == 1) {
            napTranzakciokJelentes(bank);
        } else if (valasztas == 2) {
            kamatSzamitasJelentes(bank);
        } else {
            std::cout << "Hiba: Ervenytelen menupont!\n";
        }
    }
}

void KonzolosUI::napTranzakciokJelentes(Bank& bank) {
    std::cout << "Add meg a datumot (EEEE-HH-NN): ";
    std::string datumStr;
    std::getline(std::cin, datumStr);

    Date d;
    if (d.parseFromStr(datumStr.c_str())) {
        bank.napiJelentes(d);
    } else {
        std::cout << "Hiba: Ervenytelen datum formatum.\n";
    }
}

void KonzolosUI::kamatSzamitasJelentes(Bank& bank) {
    std::cout << "Add meg a szamlaszamot: ";
    std::string szamla;
    std::getline(std::cin, szamla);

    std::cout << "Add meg az evek szamat (pozitiv egesz szam): ";
    int ev;

    while (!(std::cin >> ev)) {
        std::cout << "Ervenytelen bemenet. Kerem, szamot adjon meg: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore();

    if (ev <= 0) {
        std::cout << "Hiba: Az evek szama csak pozitiv lehet.\n";
    } else {
        bank.kamatJelentes(szamla, ev);
    }
}

