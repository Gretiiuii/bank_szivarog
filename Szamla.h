/**
 * @file Szamla.h
 * @brief Számla osztályok deklarációi (absztrakt ősosztály és leszármazott típusok).
 */

#ifndef SZAMLA_H
#define SZAMLA_H

#include <stdexcept>
#include <string>
using std::string;
#include "Date.h"

#include "memtrace.h"

/**
 * @enum SzamlaTipus
 * @brief A számlák típusait felsoroló enum.
 */
enum class SzamlaTipus {
    FolyoSzamla,     ///< Nem kamatozik, túlléphető keret
    TakarekSzamla,   ///< Kamatozik, nincs túllépés
    PremiumSzamla,   ///< Magas kamat, min. nyitóegyenleg
    DiakSzamla       ///< Nem kamatozik, diákigazolvány szükséges
};

inline SzamlaTipus szamlaTipusFromString(const std::string& tipusStr) {
    if (tipusStr == "FolyoSzamla") return SzamlaTipus::FolyoSzamla;
    if (tipusStr == "TakarekSzamla") return SzamlaTipus::TakarekSzamla;
    if (tipusStr == "PremiumSzamla") return SzamlaTipus::PremiumSzamla;
    if (tipusStr == "DiakSzamla") return SzamlaTipus::DiakSzamla;
    throw std::invalid_argument("Ismeretlen szamla tipusa: " + tipusStr);
}

/**
 * @class Szamla
 * @brief Absztrakt számla osztály, amely az összes számlatípus alapja.
 */
class Szamla {
protected:
    string szamlaszam;     ///< Számlaszám
    string ugyfelID;       ///< Tulajdonos ügyfél ID
    double egyenleg;       ///< Aktuális egyenleg
    Date nyitas;           ///< Nyitás dátuma

public:
    /**
     * @brief Számla konstruktor.
     * @param szamlaID A számlaszám
     * @param ugyfel Az ügyfél ID
     * @param egyenleg Kezdő egyenleg
     * @param nyitas A számla nyitásának dátuma
     */
    Szamla(const string& szamlaID, const string& ugyfel, double egyenleg, const Date& nyitas);
    virtual ~Szamla() = default;

    virtual bool kivesz(double osszeg); ///< Kivon egy összeget, ha lehetséges
    virtual void befizet(double osszeg); ///< Befizet egy összeget

    /**
     * @brief Absztrakt függvény a kamat számításához.
     * @param ev Évek száma
     * @return A kamattal növelt érték
     */
    virtual double kamatSzamol(int ev) const = 0;

    /**
     * @brief A számlatípus visszaadása string formában.
     * @return A típus neve
     */
    virtual string getTipus() const = 0;

    const string& getSzamlaSzam() const;
    const string& getUgyfelID() const;
    double getEgyenleg() const;
    const Date& getNyitas() const;

    /**
     * @brief Extra adat visszaadása, pl. diákigazolvány
     * @return Extra adatként tárolt string
     */
    virtual std::string getExtraAdat() const { return "0"; }
};

/**
 * @class Folyoszamla
 * @brief Egyszerű, nem kamatozó számlatípus.
 */
class FolyoSzamla : public Szamla {
public:
    FolyoSzamla(const string& szamlaID, const string& ugyfelID, double egyenleg, const Date& nyitas);
    double kamatSzamol(int ev) const override;
    string getTipus() const override;
};

/**
 * @class TakarekSzamla
 * @brief Kamatozó számla típus.
 */
class TakarekSzamla : public Szamla {
    static double kamatlab; ///< Kamatláb értéke

public:
    TakarekSzamla(const string& szamlaID, const string& ugyfelID, double egyenleg, double kamatlab, const Date& nyitas);
    double kamatSzamol(int ev) const override;
    string getTipus() const override;

    /**
     * @brief Kamatláb lekérdezése.
     * @return A kamatláb értéke
     */
    static double getKamatlab() { return kamatlab; }
};

/**
 * @class PremiumSzamla
 * @brief Magas kamatozású számla minimális nyitóegyenleggel.
 */
class PremiumSzamla : public Szamla {
    static double kamatlab; ///< Kamatláb
    static double minEgyenleg; ///< Minimális nyitóegyenleg

public:
    PremiumSzamla(const string& szamlaID, const string& ugyfelID, double egyenleg, double kamatlab, const Date& nyitas);
    double kamatSzamol(int ev) const override;
    string getTipus() const override;

    /** @brief Kamatláb lekérdezése */
    static double getKamatlab() { return kamatlab; }

    /** @brief Minimális nyitóegyenleg lekérdezése */
    static double getMinEgyenleg() { return minEgyenleg; }
};

/**
 * @class DiakSzamla
 * @brief Diákoknak szánt speciális számla, diákigazolvánnyal.
 */
class DiakSzamla : public Szamla {
    std::string diakIgazolvany;

public:
    DiakSzamla(const std::string& szamlaID, const std::string& ugyfelID, double egyenleg, const Date& nyitas, const std::string& diakIgazolvany);
    double kamatSzamol(int ev) const override;
    std::string getTipus() const override;

    /** @brief Diákigazolvány szám lekérdezése */
    const std::string& getDiakIgazolvany() const;

    /** @brief Igazolja, hogy a megadott szám egy érvényes diákigazolvány-e */
    static bool ervenyesDiakIgazolvany(const std::string& szam);

    /** @brief Visszaadja a diákigazolványt extra adatként */
    std::string getExtraAdat() const override;
};

#endif // SZAMLA_H
