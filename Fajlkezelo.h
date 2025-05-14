/**
* @file FajlKezelo.h
 * @brief Fájlkezelési segédosztály, amely be- és kiment adatokat TSV formátumban.
 */
#ifndef FAJLKEZELO_H
#define FAJLKEZELO_H

#include "Dinamikustomb.h"
#include "Ugyfel.h"
#include "Szamla.h"
#include "Tranzakcio.h"

/**
 * @class FajlKezelo
 * @brief Adatok (ügyfelek, számlák, tranzakciók) TSV fájlba való mentéséért és onnan való betöltéséért felel.
 *
 * Használt fájlformátumok:
 * - ugyfelek.tsv: ügyfélID, név, telefon, cím
 * - szamlak.tsv: számlaszám, típus, ügyfélID, egyenleg, nyitásDátum, extra adat
 * - tranzakciok.tsv: tranzakcióID, típus, számlaszám, összeg, dátum, cél számla
 */
class FajlKezelo {
public:
    static DinamikusTomb<Ugyfel*> betoltUgyfeleket(const char* fajlnev); ///< Ügyféladatok betöltése TSV fájlból
    static void mentUgyfelek(const DinamikusTomb<Ugyfel*>& ugyfelek, const char* fajlnev); ///< Ügyféladatok mentése TSV fájlba

    static DinamikusTomb<Szamla*> betoltSzamlakat(const char* fajlnev); ///< Számlaadatok betöltése TSV fájlból
    static void mentSzamlak(const DinamikusTomb<Szamla*>& szamlak, const char* fajlnev); ///< Számlaadatok mentése TSV fájlba

    static DinamikusTomb<Tranzakcio*> betoltTranzakciok(const char* fajlnev, const DinamikusTomb<Szamla*>& szamlak); ///< Tranzakciók betöltése TSV fájlból
    static void mentTranzakciok(const DinamikusTomb<Tranzakcio*>& tranzakciok, const char* fajlnev); ///< Tranzakciók mentése TSV fájlba
};

#endif //FAJLKEZELO_H
