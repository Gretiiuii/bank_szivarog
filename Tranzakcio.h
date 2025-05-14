/**
 * @file Tranzakcio.h
 * @brief Tranzakciók kezelése a banki rendszerben.
 */

#ifndef TRANZAKCIO_H
#define TRANZAKCIO_H

#include "Date.h"
#include "Szamla.h"
#include <string>

#include "memtrace.h"

/**
 * @enum TranzakcioTipus
 * @brief A tranzakció típusok felsorolása.
 */
enum class TranzakcioTipus {
    Befizetes, ///< Pénz befizetése egy számlára
    Kivetel,   ///< Pénz kivétele egy számláról
    Atutalas   ///< Pénz átutalása egyik számláról a másikra
};

/**
 * @brief Visszaadja a tranzakciótípus szöveges reprezentációját.
 * @param tipus A tranzakció típusa
 * @return Sztring formában a típus neve
 */
inline std::string stringTranzakcioTipus(TranzakcioTipus tipus) {
    switch (tipus) {
        case TranzakcioTipus::Befizetes: return "Befizetes";
        case TranzakcioTipus::Kivetel: return "Kivetel";
        case TranzakcioTipus::Atutalas: return "Atutalas";
        default: return "Ismeretlen";
    }
}

/**
 * @class Tranzakcio
 * @brief Absztrakt tranzakció osztály, amelyből a konkrét tranzakciótípusok származnak.
 */
class Tranzakcio {
protected:
    std::string id;  ///< Tranzakció azonosító
    Szamla* szamla;  ///< Érintett számla (forrás)
    double osszeg;   ///< Tranzakció összege
    Date datum;      ///< Tranzakció dátuma

public:
    Tranzakcio(const std::string& id, Szamla* sz, double osszeg, const Date& datum); ///< Tranzakció konstruktor
    virtual ~Tranzakcio() = default;

    virtual bool vegrehajt() = 0; ///< Tranzakció végrehajtása (sikertől függően true/false)
    virtual TranzakcioTipus getTipus() const = 0; ///< Tranzakció típusának lekérdezése

    const std::string& getID() const { return id; } ///< Tranzakció azonosítójának lekérdezése
    const Date& getDatum() const { return datum; } ///< Dátum lekérdezése
    const std::string& getSzamlaSzam() const { return szamla->getSzamlaSzam(); } ///< Számlaszám lekérdezése
    double getOsszeg() const { return osszeg; } ///< Tranzakció összege

    /**
     * @brief Cél számla lekérdezése (alapértelmezett: nincs).
     * @return Üres string (nem átutalás esetén)
     */
    virtual std::string getCelSzamla() const;
};

/**
 * @class Befizetes
 * @brief Számlára történő pénzbefizetés tranzakciója.
 */
class Befizetes : public Tranzakcio {
public:
    Befizetes(const std::string& id, Szamla* sz, double osszeg, const Date& datum); ///< Konstruktor
    bool vegrehajt() override; ///< Tranzakció végrehajtása
    TranzakcioTipus getTipus() const override; ///< Típus lekérdezése
};

/**
 * @class Kivetel
 * @brief Számláról történő pénzkivét tranzakciója.
 */
class Kivetel : public Tranzakcio {
public:
    Kivetel(const std::string& id, Szamla* sz, double osszeg, const Date& datum);  ///< Konstruktor
    bool vegrehajt() override; ///< Tranzakció végrehajtása
    TranzakcioTipus getTipus() const override; ///< Típus lekérdezése
};

/**
 * @class Atutalas
 * @brief Pénz átutalása két számla között.
 */
class Atutalas : public Tranzakcio {
    Szamla* cel; ///< Cél számla

public:
    Atutalas(const std::string& id, Szamla* f, Szamla* c, double osszeg, const Date& datum); ///< Konstruktor
    bool vegrehajt() override; ///< Tranzakció végrehajtása (átutalás)
    std::string getCelSzamla() const override; ///< Cél számla lekérdezése
    TranzakcioTipus getTipus() const override; ///< Típus lekérdezése
};

#endif // TRANZAKCIO_H
