#include "Bank.h"
#include "FajlKezelo.h"
#include "KonzolosUI.h"
#include <iostream>



#include "gtest_lite.h"
#include "Bank.h"
#include "Ugyfel.h"
#include "Szamla.h"
#include "Date.h"

#include <sstream>


int main() {
    // Ügyfelek tesztek:

    //Új ügyfél sikeres regisztrációja
    TEST(bank, ugyfel_regisztracio) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Elek", "+36123456789", "Teszt utca 1");

        Ugyfel* talalt = bank.ugyfelKeres("CUST001"); // ID alapján keresünk
        EXPECT_TRUE(talalt != nullptr); // Van ilyen ügyfél
        EXPECT_EQ(talalt->getNev(), "Teszt Elek");
        EXPECT_EQ(talalt->getTelefon(), "+36123456789");
    }
    END

    //Hibás formátumú telefonszámnál kivétel dobódik-e
    TEST(bank, hibas_telefonszam_nem_regisztralodik) {
        Bank bank;
        bool kiveteltDobott = false;
        try {
            bank.ugyfelRegisztral("Teszt Elek", "123456", "Teszt utca 2");
        } catch (const std::invalid_argument&) {
            kiveteltDobott = true;
        }
        EXPECT_TRUE(kiveteltDobott);
    }
    END

    //Ügyfél adatok módosítása sikeres-e (név,telefonszám,cím)
    TEST(bank, ugyfel_modositas_tel_cim_nev) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Elek", "+36123456789", "Teszt utca 1");

        bool siker = bank.modositUgyfel("CUST001", "Új Név", "+36111222333", "Új utca 42");

        EXPECT_TRUE(siker);
        const DinamikusTomb<Ugyfel*>& lista = bank.getUgyfelek();
        EXPECT_EQ(lista[0]->getNev(), "Új Név");
        EXPECT_EQ(lista[0]->getTelefon(), "+36111222333");
        EXPECT_EQ(lista[0]->getCim(), "Új utca 42");
    }
    END

    //Ügyfél törlése és újrakeresés , hogy már nincs benne.
    TEST(bank, ugyfel_torles) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Elek", "+36123456789", "Teszt utca 1");

        Ugyfel* elsoKeres = bank.ugyfelKeres("CUST001");
        EXPECT_TRUE(elsoKeres != nullptr);

        bool siker = bank.torolUgyfel("CUST001");
        EXPECT_TRUE(siker);

        Ugyfel* masodikKeres = bank.ugyfelKeres("CUST001"); // Újra ID alapján keresünk
        EXPECT_TRUE(masodikKeres == nullptr); // Helyes ha nincs többé ilyen ügyfél
    }
    END

    //Folyószámla nyitás sikeresen
    TEST(bank, szamla_nyitas) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Simaszamla", "+36123456789", "Teszt utca 1");

        bank.szamlaNyit("CUST001", "FolyoSzamla", 100000.0);

        Szamla* sz = bank.szamlaKeres("ACC001");
        EXPECT_TRUE(sz != nullptr);
        EXPECT_TRUE(sz->getTipus() == "FolyoSzamla");
        EXPECT_TRUE(sz->getUgyfelID() == "CUST001");
        EXPECT_EQ(sz->getEgyenleg(), 100000.0);
    }
    END

    //Diákszámla nyitása helyes diákigazolvánnyal
    TEST(bank, szamla_nyitas_diakszamla) {
        Bank bank;

        bank.ugyfelRegisztral("Teszt Diakszamla", "+36123456789", "Schonherz");
        bank.szamlaNyit("CUST001", "DiakSzamla", 5000.0, Date::ma(), "12345678");

        Szamla* sz = bank.szamlaKeres("ACC001");

        EXPECT_TRUE(sz != nullptr);
        EXPECT_EQ(sz->getTipus(), "DiakSzamla");
        EXPECT_EQ(sz->getUgyfelID(), "CUST001");
        EXPECT_EQ(sz->getEgyenleg(), 5000.0);

        // dynamic_cast nélkül
        EXPECT_EQ(sz->getExtraAdat(), "12345678");
    }
    END

    //Számla törlése sikeres-e
    TEST(bank, szamla_torles_sikeres) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Elek", "+36123456789", "Teszt utca 1");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 1000.0);

        EXPECT_EQ(bank.getSzamlak().meret(), 1u);

        bool siker = bank.szamlaZaras("ACC001");
        EXPECT_TRUE(siker);
        EXPECT_EQ(bank.getSzamlak().meret(), 0u);
    }
    END

    // Nem létező számla törlése, sikeretlen-e
    TEST(bank, szamla_torles_nem_letezo) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Elek", "+36123456789", "Teszt utca 1");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 1000.0);

        bool siker = bank.szamlaZaras("ACC999");
        EXPECT_FALSE(siker);
        EXPECT_EQ(bank.getSzamlak().meret(), 1u);
    }
    END

    // Egyenleg lekérdezése
    TEST(bank, egyenleg_lekerdezes) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Egyenleg", "+36123456789", "Szamla utca 7");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 42000.0);

        double egyenleg = bank.egyenlegLekerdez("ACC001");

        EXPECT_EQ(egyenleg, 42000.0);
    }
    END

    // Tranzakció tesztek:

    // Sikeres befizetés számlára
    TEST(tranzakcio, befizetes_sikeres) {
        FolyoSzamla sz("ACC001", "CUST001", 1000.0, Date::ma());
        Befizetes b("TRANS001", &sz, 10000.0, Date::ma());

        EXPECT_TRUE(b.vegrehajt());
        EXPECT_EQ(sz.getEgyenleg(), 11000.0);
    }
    END

    // Túl kicsi befizetés sikeretelen-e
    TEST(tranzakcio, befizetes_tul_kicsi) {
        FolyoSzamla sz("ACC002", "CUST001", 1000.0, Date::ma());
        Befizetes b("TRANS002", &sz, 100.0, Date::ma());

        EXPECT_FALSE(b.vegrehajt());
        EXPECT_EQ(sz.getEgyenleg(), 1000.0);
    }
    END

    // Túl nagy befizetés sikertelen-e
    TEST(tranzakcio, befizetes_tul_nagy) {
        FolyoSzamla sz("ACC003", "CUST001", 1000.0, Date::ma());
        Befizetes b("TRANS003", &sz, 15000000.0, Date::ma());

        EXPECT_FALSE(b.vegrehajt());
        EXPECT_EQ(sz.getEgyenleg(), 1000.0);
    }
    END

    //Sikeres kivétel számláról
    TEST(tranzakcio, kivetel_sikeres) {
        FolyoSzamla sz("ACC004", "CUST001", 50000.0, Date::ma());
        Kivetel k("TRANS004", &sz, 10000.0, Date::ma());

        EXPECT_TRUE(k.vegrehajt());
        EXPECT_EQ(sz.getEgyenleg(), 40000.0);
    }
    END

    // Túl kicsi összeg kivétele
    TEST(tranzakcio, kivetel_tul_kicsi) {
        FolyoSzamla sz("ACC005", "CUST001", 50000.0, Date::ma());
        Kivetel k("TRANS005", &sz, 500.0, Date::ma());

        EXPECT_FALSE(k.vegrehajt());
        EXPECT_EQ(sz.getEgyenleg(), 50000.0);
    }
    END

    // Nincs fedezet kivételnél
    TEST(tranzakcio, kivetel_nincs_fedezet) {
        FolyoSzamla sz("ACC006", "CUST001", 2000.0, Date::ma());
        Kivetel k("TRANS006", &sz, 3000.0, Date::ma());

        EXPECT_FALSE(k.vegrehajt());
        EXPECT_EQ(sz.getEgyenleg(), 2000.0);
    }
    END

    //Sikeres átutalás
    TEST(tranzakcio, atutalas_sikeres) {
        FolyoSzamla from("ACC007", "CUST001", 50000.0, Date::ma());
        FolyoSzamla to("ACC008", "CUST002", 1000.0, Date::ma());

        Atutalas a("TRANS007", &from, &to, 20000.0, Date::ma());

        EXPECT_TRUE(a.vegrehajt());
        EXPECT_EQ(from.getEgyenleg(), 30000.0);
        EXPECT_EQ(to.getEgyenleg(), 21000.0);
    }
    END

    // Nincs fedezet átutalásra
    TEST(tranzakcio, atutalas_nincs_fedezet) {
        FolyoSzamla from("ACC009", "CUST001", 1000.0, Date::ma());
        FolyoSzamla to("ACC010", "CUST002", 0.0, Date::ma());

        Atutalas a("TRANS008", &from, &to, 2000.0, Date::ma());

        EXPECT_FALSE(a.vegrehajt());
        EXPECT_EQ(from.getEgyenleg(), 1000.0);
        EXPECT_EQ(to.getEgyenleg(), 0.0);
    }
    END

    // Jelentések tesztek:

    //Kamatjelentés kamatozó számlára
    TEST(bank, kamatjelentes_takarekszamla) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Kamatozo", "+36123456789", "Kamat utca 1");
        bank.szamlaNyit("CUST001", "TakarekSzamla", 100000);

        std::stringstream ss;
        std::streambuf* eredeti = std::cout.rdbuf(ss.rdbuf());

        bank.kamatJelentes("ACC001", 2);

        std::cout.rdbuf(eredeti);

        EXPECT_TRUE(ss.str().find("Varhato egyenleg") != std::string::npos);
    }
    END

    // Folyószámlára kamatjelentés (ami ugye nem kamatozik) sikertlen-e
    TEST(bank, kamatjelentes_FolyoSzamla) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Simaszamla", "+36123456789", "Sima utca 2");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 50000.0);

        std::stringstream ss;
        std::streambuf* eredeti = std::cout.rdbuf(ss.rdbuf());

        bank.kamatJelentes("ACC001", 3);

        std::cout.rdbuf(eredeti);

        EXPECT_TRUE(ss.str().find("nem kamatozik") != std::string::npos);
    }
    END

    // Napi tranzakció jelentése helyes-e
    TEST(bank, napi_jelentes) {
        Bank bank;
        bank.ugyfelRegisztral("Teszt Tranzakció", "+36123456789", "Napi utca 3");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 5000.0);

        bank.vegrehajtTranzakcio("ACC001", TranzakcioTipus::Befizetes, 10000.0);

        std::stringstream ss;
        std::streambuf* eredeti = std::cout.rdbuf(ss.rdbuf());

        bank.napiJelentes(Date::ma());

        std::cout.rdbuf(eredeti);

        EXPECT_TRUE(ss.str().find("Befizetes") != std::string::npos);
    }
    END

    // Konzol vezérlés

    TEST(KonzolosUI, FoMenu_UgyfelKezeles) {
        Bank bank;
        KonzolosUI ui;
        std::stringstream input("1\n1\nTeszt Elek\n+36301234567\nBudapest\n0\n0\n");
        std::streambuf* cin_backup = std::cin.rdbuf(input.rdbuf());
        ui.foMenu(bank);
        std::cin.rdbuf(cin_backup);
        EXPECT_EQ(bank.getUgyfelek().meret(), 1u);
    }
    END

    TEST(KonzolosUI, FoMenu_SzamlaMuveletek) {
        Bank bank;
        KonzolosUI ui;
        bank.ugyfelRegisztral("Teszt", "+36301234567", "Budapest");
        std::stringstream input("2\n1\nCUST001\nFolyoSzamla\n1000\n0\n0\n");
        std::streambuf* cin_backup = std::cin.rdbuf(input.rdbuf());
        ui.foMenu(bank);
        std::cin.rdbuf(cin_backup);
        EXPECT_EQ(bank.getSzamlak().meret(), 1u);
    }
    END

    TEST(KonzolosUI, FoMenu_Tranzakciok) {
        Bank bank;
        KonzolosUI ui;
        bank.ugyfelRegisztral("Teszt", "+36301234567", "Budapest");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 1000, Date::ma(), "");
        std::stringstream input("3\n1\nACC001\n500\n0\n0\n");
        std::streambuf* cin_backup = std::cin.rdbuf(input.rdbuf());
        ui.foMenu(bank);
        std::cin.rdbuf(cin_backup);
        EXPECT_EQ(bank.egyenlegLekerdez("ACC001"), 1500);
    }
    END

    // Fájlkezelő tesztek

    // ugyfelek.tsv
    TEST(fajlkezelo, ugyfelek_betoltes_letezo_fajl_keres) {
        Bank bank;
        DinamikusTomb<Ugyfel*> ugyfelek = FajlKezelo::betoltUgyfeleket("ugyfelek.tsv");
        bank.beallitottUgyfelek(ugyfelek);


        Ugyfel* talalt1 = bank.ugyfelKeres("CUST001");
        EXPECT_TRUE(talalt1 != nullptr);
        EXPECT_EQ(talalt1->getNev(), "Kovacs Janos");
    }
    END

    TEST(fajlkezelo, ugyfelek_betoltes_nem_letezo_fajl) {
        bool kiveteltDobott = false;
        try {
            DinamikusTomb<Ugyfel*> ugyfelek = FajlKezelo::betoltUgyfeleket("nem_letezik.tsv");
        } catch (const std::runtime_error&) {
            kiveteltDobott = true;
        }
        EXPECT_TRUE(kiveteltDobott);
    }
    END

    TEST(fajlkezelo, ugyfelek_mentes) {
        Bank bank;
        bank.ugyfelRegisztral("Mentendo Ügyfel", "+36111222333", "Mentendo utca");
        FajlKezelo::mentUgyfelek(bank.getUgyfelek(), "mentes_ugyfelek.tsv");

        std::ifstream fajl("mentes_ugyfelek.tsv");
        std::string sor;
        std::getline(fajl, sor);
        std::getline(fajl, sor);
        EXPECT_TRUE(sor.find("Mentendo Ügyfel") != std::string::npos);

        fajl.close();
        std::remove("mentes_ugyfelek.tsv");
    }
    END

    TEST(fajlkezelo, szamlak_betoltes_letezo_fajl) {
        DinamikusTomb<Szamla*> szamlak = FajlKezelo::betoltSzamlakat("szamlak.tsv");
        EXPECT_NE(szamlak.meret(), 0u);
        EXPECT_EQ(szamlak[0]->getTipus(), "FolyoSzamla");
        EXPECT_EQ(szamlak[1]->getUgyfelID(), "CUST002");
    }
    END

    TEST(fajlkezelo, tranzakciok_mentes) {
        Bank bank;
        bank.ugyfelRegisztral("Tulajdonos", "+36309661300", "Cim");
        bank.szamlaNyit("CUST001", "FolyoSzamla", 1000, Date::ma());
        bank.vegrehajtTranzakcio("ACC001", TranzakcioTipus::Befizetes, 10000);
        FajlKezelo::mentTranzakciok(bank.getTranzakciok(), "mentes_tranzakciok.tsv");

        std::ifstream fajl("mentes_tranzakciok.tsv");
        std::string sor;
        std::getline(fajl, sor);
        std::getline(fajl, sor);
        EXPECT_TRUE(sor.find("Befizetes") != std::string::npos);

        fajl.close();
        std::remove("mentes_tranzakciok.tsv");
    }
    END


    // Date osztály tesztek

    TEST(date, ervenyes_datum_string) {
        Date d;
        EXPECT_TRUE(d.parseFromStr("2025-05-10"));
        EXPECT_EQ(d.getEv(), 2025);
        EXPECT_EQ(d.getHonap(), 5);
        EXPECT_EQ(d.getNap(), 10);
    }
    END

    TEST(date, ervenytelen_datum_string) {
        Date d;
        EXPECT_FALSE(d.parseFromStr("2025/05/10"));
        EXPECT_FALSE(d.parseFromStr("25-05-10"));
    }
    END

    Bank bank;

    // Ügyfelek betöltése fájlból
    DinamikusTomb<Ugyfel*> betoltott = FajlKezelo::betoltUgyfeleket("ugyfelek.tsv");
    bank.beallitottUgyfelek(betoltott);

    // Számlák betöltése fájlból
    DinamikusTomb<Szamla*> betoltottSzamlak = FajlKezelo::betoltSzamlakat("szamlak.tsv");
    bank.beallitottSzamlak(betoltottSzamlak);


    // Tranzakciók betöltése fájlból
    DinamikusTomb<Tranzakcio*> betoltottTranzakciok =
        FajlKezelo::betoltTranzakciok("tranzakciok.tsv", bank.getSzamlak());

    bank.beallitottTranzakciok(betoltottTranzakciok);


    // Konzolos menü
    KonzolosUI ui;
    ui.foMenu(bank);

    // Mentés
    FajlKezelo::mentUgyfelek(bank.getUgyfelek(), "ugyfelek.tsv");
    FajlKezelo::mentSzamlak(bank.getSzamlak(), "szamlak.tsv");
    FajlKezelo::mentTranzakciok(bank.getTranzakciok(), "tranzakciok.tsv");

    return 0;
}

