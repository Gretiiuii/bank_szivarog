/**
* @file Date.h
 * @brief Egyszerű dátumkezelő osztály (év, hónap, nap).
 */

#ifndef DATE_H
#define DATE_H
#include <string>
/**
 * @class Date
 * @brief Dátumot reprezentáló osztály, alapműveletekkel.
 */
class Date {
    int ev, honap, nap; ///< Év, hónap, nap mezők

public:
    Date(); ///< Alapértelmezett konstruktor (üres dátum)
    Date(int ev, int honap, int nap); ///< Év, hónap, nap alapú konstruktor

    int getEv() const;    ///< Év lekérdezése
    int getHonap() const; ///< Hónap lekérdezése
    int getNap() const;   ///< Nap lekérdezése

    void set(int ev, int honap, int nap); ///< Új dátum beállítása

    bool parseFromStr(const char* datumStr); ///< Szöveges dátumból értékek beállítása (ÉÉÉÉ-HH-NN)
    std::string toStr() const; ///< Dátum szöveges formátuma (ÉÉÉÉ-HH-NN)

    static Date ma(); ///< Visszaadja a mai dátumot

    bool operator==(const Date& other) const { ///< Egyenlőségvizsgálat két dátum között
        return ev == other.ev && honap == other.honap && nap == other.nap;
    }
};

#endif //DATE_H
