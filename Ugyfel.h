#ifndef UGYFEL_H
#define UGYFEL_H

#include <string>

/**
 * @class Ugyfel
 * @brief Egy banki ügyfelet reprezentáló osztály, amely tárolja az alapvető azonosító és elérhetőségi adatokat.
 */
class Ugyfel {
    std::string ugyfelID;  ///< Egyedi ügyfélazonosító
    std::string nev;       ///< Ügyfél neve
    std::string telefon;   ///< Telefonszám
    std::string cim;       ///< Lakcím

public:
    /**
     * @brief Létrehoz egy új ügyfél objektumot a megadott adatokkal.
     * @param id Egyedi azonosító
     * @param nev Az ügyfél neve
     * @param tel Telefonszám
     * @param cim Lakcím
     */
    Ugyfel(const std::string& id, const std::string& nev, const std::string& tel, const std::string& cim);

    const std::string& getID() const;      ///< Az ügyfél azonosítójának lekérdezése
    const std::string& getNev() const;     ///< Az ügyfél nevének lekérdezése
    const std::string& getTelefon() const; ///< Az ügyfél telefonszámának lekérdezése
    const std::string& getCim() const;     ///< Az ügyfél lakcímének lekérdezése

    void setNev(const std::string& ujNev);         ///< Az ügyfél nevének módosítása
    void setTelefon(const std::string& ujTelefon); ///< Az ügyfél telefonszámának módosítása
    void setCim(const std::string& ujCim);         ///< Az ügyfél lakcímének módosítása

    /**
     * @brief Ellenőrzi, hogy a telefonszám megfelel-e a +36-os formátumnak és tartalmaz-e pontosan 9 számjegyet.
     * @param tel A vizsgálandó telefonszám
     * @return true, ha a formátum megfelelő, egyébként false
     */
    static bool ervenyesTelefon(const std::string& tel);
};

#endif // UGYFEL_H
