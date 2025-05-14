#include "Ugyfel.h"
#include <cctype>

#include "memtrace.h"

Ugyfel::Ugyfel(const std::string& id, const std::string& nev, const std::string& tel, const std::string& cim)
    : ugyfelID(id), nev(nev), telefon(tel), cim(cim) {}

const std::string& Ugyfel::getID() const {
    return ugyfelID;
}

const std::string& Ugyfel::getNev() const {
    return nev;
}

const std::string& Ugyfel::getTelefon() const {
    return telefon;
}

const std::string& Ugyfel::getCim() const {
    return cim;
}

void Ugyfel::setNev(const std::string& ujNev) {
    nev = ujNev;
}

void Ugyfel::setTelefon(const std::string& ujTelefon) {
    telefon = ujTelefon;
}

void Ugyfel::setCim(const std::string& ujCim) {
    cim = ujCim;
}

bool Ugyfel::ervenyesTelefon(const std::string& tel) {
    if (tel.length() != 12 || tel.substr(0, 3) != "+36") return false;
    for (size_t i = 3; i < 12; ++i)
        if (!isdigit(tel[i])) return false;
    return true;
}
