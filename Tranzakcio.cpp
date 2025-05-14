#include "Tranzakcio.h"
#include "memtrace.h"

// Tranzakcio
Tranzakcio::Tranzakcio(const std::string& id, Szamla* sz, double osszeg, const Date& datum)
    : id(id), szamla(sz), osszeg(osszeg), datum(datum) {}

std::string Tranzakcio::getCelSzamla() const {
    return "";
}

// Befizetes
Befizetes::Befizetes(const std::string& id, Szamla* sz, double osszeg, const Date& datum)
    : Tranzakcio(id, sz, osszeg, datum) {}

bool Befizetes::vegrehajt() {
    if (osszeg < 500 || osszeg > 10000000) return false;
    szamla->befizet(osszeg);
    return true;
}

TranzakcioTipus Befizetes::getTipus() const {
    return TranzakcioTipus::Befizetes;
}

// Kivetel
Kivetel::Kivetel(const std::string& id, Szamla* sz, double osszeg, const Date& datum)
    : Tranzakcio(id, sz, osszeg, datum) {}

bool Kivetel::vegrehajt() {
    if (osszeg < 1000 || osszeg > 2000000)
        return false;
    return szamla->kivesz(osszeg);
}

TranzakcioTipus Kivetel::getTipus() const {
    return TranzakcioTipus::Kivetel;
}

// Atutalas
Atutalas::Atutalas(const std::string& id, Szamla* f, Szamla* c, double osszeg, const Date& datum)
    : Tranzakcio(id, f, osszeg, datum), cel(c) {}

bool Atutalas::vegrehajt() {
    if (osszeg < 100 || osszeg > 5000000)
        return false;
    if (!szamla->kivesz(osszeg))
        return false;
    cel->befizet(osszeg);
    return true;
}

std::string Atutalas::getCelSzamla() const {
    return cel->getSzamlaSzam();
}

TranzakcioTipus Atutalas::getTipus() const {
    return TranzakcioTipus::Atutalas;
}
