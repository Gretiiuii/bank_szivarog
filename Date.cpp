#include "Date.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "memtrace.h"

Date::Date() : ev(1970), honap(1), nap(1) {}

Date::Date(int ev, int honap, int nap) : ev(ev), honap(honap), nap(nap) {}

int Date::getEv() const { return ev; }
int Date::getHonap() const { return honap; }
int Date::getNap() const { return nap; }

void Date::set(int e, int h, int n) {
    ev = e;
    honap = h;
    nap = n;
}

bool Date::parseFromStr(const char* datumStr) {
    if (!datumStr || strlen(datumStr) != 10 || datumStr[4] != '-' || datumStr[7] != '-')
        return false;
    char evStr[5], honapStr[3], napStr[3];
    strncpy(evStr, datumStr, 4); evStr[4] = '\0';
    strncpy(honapStr, datumStr + 5, 2); honapStr[2] = '\0';
    strncpy(napStr, datumStr + 8, 2); napStr[2] = '\0';

    ev = atoi(evStr);
    honap = atoi(honapStr);
    nap = atoi(napStr);

    return (ev > 0 && honap >= 1 && honap <= 12 && nap >= 1 && nap <= 31);
}

std::string Date::toStr() const {
    char buffer[11];
    snprintf(buffer, 11, "%04d-%02d-%02d", ev, honap, nap);
    return std::string(buffer);
}


#include <ctime>

Date Date::ma() {
    time_t t = time(nullptr);
    tm* ido = localtime(&t);
    return Date(ido->tm_year + 1900, ido->tm_mon + 1, ido->tm_mday);
}
