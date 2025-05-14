/**
 * @file DinamikusTomb.h
 * @brief Sablon alapú dinamikus tömb implementáció, amely automatikusan bővül szükség esetén.
 * @tparam T A tárolt típus
 */
#ifndef DINAMIKUSTOMB_H
#define DINAMIKUSTOMB_H

#include <cstddef>
#include <stdexcept>
/**
 * @class DinamikusTomb
 * @brief Sablon alapú automatikusan bővülő tömbosztály, amely pointerek és érték típusok tárolására egyaránt alkalmas.
 *
 * Fő funkciók:
 * - hozzaad(): új elem hozzáadása
 * - eltavolit(): elem törlése index alapján
 * - operator[]: indexelés olvasáshoz és íráshoz
 * - meret(): aktuálisan tárolt elemek száma
 * - kap(): kapacitás lekérdezése
 *
 * A háttérben dinamikusan allokált tömböt használ, és ha a kapacitás betelt, duplázza azt.
 */
template<typename T>
class DinamikusTomb {
    T* tomb;               ///< Belső tömb
    size_t kapacitas;      ///< Aktuális kapacitás
    size_t db;             ///< Tárolt elemek száma

    void bovit() { ///< Kapacitás duplázása, ha betelt a tömb
        size_t ujKapacitas = kapacitas * 2;
        T* ujTomb = new T[ujKapacitas];
        for (size_t i = 0; i < db; ++i) {
            ujTomb[i] = tomb[i];
        }
        delete[] tomb;
        tomb = ujTomb;
        kapacitas = ujKapacitas;
    }

public:
    DinamikusTomb(size_t kezdoKapacitas = 8) ///< Konstruktor kezdeti kapacitással
        : kapacitas(kezdoKapacitas), db(0) {
        tomb = new T[kapacitas];
    }

    DinamikusTomb(const DinamikusTomb& masik) ///< Másoló konstruktor
        : kapacitas(masik.kapacitas), db(masik.db) {
        tomb = new T[kapacitas];
        for (size_t i = 0; i < db; ++i) {
            tomb[i] = masik.tomb[i];
        }
    }

    DinamikusTomb& operator=(const DinamikusTomb& masik) { ///< Értékadó operátor
        if (this != &masik) {
            delete[] tomb;
            kapacitas = masik.kapacitas;
            db = masik.db;
            tomb = new T[kapacitas];
            for (size_t i = 0; i < db; ++i) {
                tomb[i] = masik.tomb[i];
            }
        }
        return *this;
    }

    ~DinamikusTomb() { delete[] tomb; } ///< Destruktor

    void hozzaad(const T& elem) { ///< Új elem hozzáadása a végére
        if (db == kapacitas) {
            bovit();
        }
        tomb[db++] = elem;
    }

    void eltavolit(size_t index) { ///< Elem eltávolítása adott indexről
        if (index >= db) return;
        for (size_t i = index; i < db - 1; ++i) {
            tomb[i] = tomb[i + 1];
        }
        --db;
    }

    T& operator[](size_t index) { ///< Indexelő operátor íráshoz
        if (index >= db) throw std::out_of_range("Index hiban határon túl");
        return tomb[index];
    }

    const T& operator[](size_t index) const { ///< Indexelő operátor olvasáshoz
        if (index >= db) throw std::out_of_range("Index hiban határon túl");
        return tomb[index];
    }

    size_t meret() const { return db; } ///< Aktuális elemszám lekérdezése
    size_t kap() const { return kapacitas; } ///< Kapacitás lekérdezése
};

#endif // DINAMIKUSTOMB_H
