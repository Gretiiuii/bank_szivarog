/**
 * @file Bank.h
 * @brief A Bank osztály, amely kezeli az ügyfeleket, számlákat és tranzakciókat.
 */
#ifndef BANK_H
#define BANK_H

#include "Dinamikustomb.h"
#include "Ugyfel.h"
#include "Szamla.h"
#include "Tranzakcio.h"

/**
 * @class Bank
 * @brief A banki rendszer központi osztálya.
 *
 * Tárolja és kezeli az ügyfeleket, számlákat, tranzakciókat, és fájlműveleteket végez.
 */
class Bank {
    DinamikusTomb<Ugyfel*> ugyfelek; ///< Ügyfelek dinamikus tömbje
    DinamikusTomb<Szamla*> szamlak; ///< Számlák dinamikus tömbje
    DinamikusTomb<Tranzakcio*> tranzakciok; ///< Tranzakciók dinamikus tömbje

    /**
     * @brief Egyedi azonosító generálása megadott prefix alapján.
     * @param prefix Az azonosító előtagja (pl. "CUST", "ACC")
     * @param szam A sorszám
     * @return Egyedi azonosító string
     */
    std::string generalID(const std::string& prefix, size_t szam);

    // Belső segédfüggvények
    void ellenorizSzamlaAdatok(SzamlaTipus tipus, double egyenleg, const std::string& diakIgazolvany) const;
    Szamla* letrehozSzamlat(SzamlaTipus tipus, const std::string& szamlaszam, const std::string& ugyfelID, double egyenleg, const Date& nyitas, const std::string& diakIgazolvany);
    Tranzakcio* letrehozTranzakcio(TranzakcioTipus tipus, const std::string& szamlaszam, double osszeg, Szamla* celSzamla);


public:
    Bank(); ///< Konstruktor
    ~Bank(); ///< Destruktor

    // Ügyfelek
    void beallitottUgyfelek(DinamikusTomb<Ugyfel*>& lista); ///< Betöltött ügyfelek beállítása
    void ugyfelRegisztral(const std::string& nev, const std::string& tel, const std::string& cim); ///< Új ügyfél regisztrálása
    Ugyfel* ugyfelKeres(const std::string& id); ///< Ügyfél keresése azonosító alapján
    const DinamikusTomb<Ugyfel*>& getUgyfelek() const; ///< Ügyfél lista lekérdezése
    bool modositUgyfel(const std::string& id, const std::string& ujNev, const std::string& ujTel, const std::string& ujCim); ///< Ügyfél módosítása
    bool torolUgyfel(const std::string& id); ///< Ügyfél törlése

    // Számlák
    void beallitottSzamlak(DinamikusTomb<Szamla*>& lista); ///< Betöltött számlák beállítása
    void szamlaNyit(const std::string& ugyfelID, const std::string& tipus, double egyenleg, const Date& nyitas = Date::ma(), const std::string& diakIgazolvany = ""); ///< Új számla nyitása
    Szamla* szamlaKeres(const std::string& szamlaszam) const; ///< Számla keresése azonosító alapján
    const DinamikusTomb<Szamla*>& getSzamlak() const; ///< Számlalisták lekérdezése
    bool szamlaZaras(const std::string& szamlaszam); ///< Számla lezárása
    double egyenlegLekerdez(const std::string& szamlaszam) const; ///< Számlaegyenleg lekérdezése

    // Tranzakciók
    void beallitottTranzakciok(DinamikusTomb<Tranzakcio*>& lista); ///< Betöltött tranzakciók beállítása
    void vegrehajtTranzakcio(const std::string& szamlaszam, TranzakcioTipus tipus, double osszeg, const std::string& celSzamla = ""); ///< Tranzakció végrehajtása
    const DinamikusTomb<Tranzakcio*>& getTranzakciok() const; ///< Tranzakciók listájának lekérdezése

    // Jelentések
    void napiJelentes(const Date& datum) const; ///< Napi tranzakció jelentés
    void kamatJelentes(const std::string& szamlaszam, int ev) const; ///< Kamat előrejelzés
};

#endif // BANK_H
