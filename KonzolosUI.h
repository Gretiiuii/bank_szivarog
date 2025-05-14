/**
* @file KonzolosUI.h
 * @brief Konzolos felhasználói felület a banki műveletekhez.
 */
#ifndef KONZOLOSUI_H
#define KONZOLOSUI_H

#include "Bank.h"
#include <iostream>
#include <string>

/**
 * @class KonzolosUI
 * @brief Menürendszer a banki funkciók konzolos eléréséhez.
 *
 * A főmenü négy fő területre bontja a funkciókat:
 * - Ügyfélkezelés: ügyfél regisztrálás, módosítás, törlés, keresés
 * - Számlakezelés: számlanyitás, lezárás, egyenleg lekérdezés
 * - Tranzakciók: befizetés, kivétel, átutalás
 * - Jelentések: napi tranzakciók listázása és kamatszámítás
 */
class KonzolosUI {
public:
    void foMenu(Bank& bank); ///< Főmenü megjelenítése és vezérlése

    void ugyfelKezelesMenu(Bank& bank); ///< Ügyfélkezelési almenü (regisztráció, módosítás, törlés, keresés)
    void ugyfelRegisztracio(Bank& bank); ///< Új ügyfél adatainak bekérése és regisztrálása
    void ugyfelModositas(Bank& bank); ///< Létező ügyfél adatainak módosítása
    void ugyfelTorles(Bank& bank); ///< Ügyfél törlése ID alapján
    void ugyfelKereses(Bank& bank); ///< Ügyfél keresése ID vagy számlaszám alapján

    void szamlaMuveletekMenu(Bank& bank); ///< Számlaműveleti almenü (nyitás, lezárás, egyenleg lekérdezés)
    void szamlaNyitasMenu(Bank& bank); ///< Új számla nyitása ügyfélhez adott paraméterekkel
    void szamlaZarasMenu(Bank& bank); ///< Számla lezárása (törlése)
    void egyenlegLekerdezMenu(Bank& bank); ///< Egy adott számla aktuális egyenlegének megjelenítése

    void tranzakcioMenu(Bank& bank); ///< Tranzakció típus kiválasztása
    void tranzakcioVegrehajtas(Bank& bank, TranzakcioTipus tipus, bool atutalasos);///< Tranzakció típus alapú végrehajtása (befizetés, kivétel, átutalás)

    void jelentesMenu(Bank& bank); ///< Jelentések almenü (tranzakciók, kamatok)
    void napTranzakciokJelentes(Bank& bank); ///< Adott napra vonatkozó tranzakciók listázása
    void kamatSzamitasJelentes(Bank& bank); ///< Megadott időszakra kamat előrejelzés számlához
};

#endif // KONZOLOSUI_H
