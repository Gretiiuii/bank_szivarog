#include "Szamla.h"
#include <cmath>
#include <cctype>
#include <stdexcept>

#include "memtrace.h"

// Szamla
Szamla::Szamla(const string& szamlaID, const string& ugyfel, double egyenleg, const Date& nyitas)
    : szamlaszam(szamlaID), ugyfelID(ugyfel), egyenleg(egyenleg), nyitas(nyitas) {}

bool Szamla::kivesz(double osszeg) {
    if (egyenleg >= osszeg) {
        egyenleg -= osszeg;
        return true;
    }
    return false;
}

void Szamla::befizet(double osszeg) {
    egyenleg += osszeg;
}

const string& Szamla::getSzamlaSzam() const {
    return szamlaszam;
}

const string& Szamla::getUgyfelID() const {
    return ugyfelID;
}

double Szamla::getEgyenleg() const {
    return egyenleg;
}

const Date& Szamla::getNyitas() const {
    return nyitas;
}

// Folyoszamla
FolyoSzamla::FolyoSzamla(const string& szamlaID, const string& ugyfelID, double egyenleg, const Date& nyitas)
    : Szamla(szamlaID, ugyfelID, egyenleg, nyitas) {}

double FolyoSzamla::kamatSzamol(int) const {
    return 0.0;
}

string FolyoSzamla::getTipus() const {
    return "FolyoSzamla";
}

// TakarekSzamla
double TakarekSzamla::kamatlab = 0.02;

TakarekSzamla::TakarekSzamla(const string& szamlaID, const string& ugyfelID, double egyenleg, double kl, const Date& nyitas)
    : Szamla(szamlaID, ugyfelID, egyenleg, nyitas) {
    kamatlab = kl;
}

double TakarekSzamla::kamatSzamol(int ev) const {
    return egyenleg * std::pow(1 + kamatlab, ev);
}

string TakarekSzamla::getTipus() const {
    return "TakarekSzamla";
}

// PremiumSzamla
double PremiumSzamla::kamatlab = 0.05;
double PremiumSzamla::minEgyenleg= 100000.0;

PremiumSzamla::PremiumSzamla(const string& szamlaID, const string& ugyfelID, double egyenleg, double kl, const Date& nyitas)
    : Szamla(szamlaID, ugyfelID, egyenleg, nyitas) {
    kamatlab = kl;
}

double PremiumSzamla::kamatSzamol(int ev) const {
    return egyenleg * std::pow(1 + kamatlab, ev);
}

string PremiumSzamla::getTipus() const {
    return "PremiumSzamla";
}

std::string DiakSzamla::getExtraAdat() const{
    return diakIgazolvany;
}
// DiakSzamla

bool DiakSzamla::ervenyesDiakIgazolvany(const std::string& szam) {
    if (szam.length() != 8) return false;
    for (char c : szam)
        if (!isdigit(c)) return false;
    return true;
}

DiakSzamla::DiakSzamla(const std::string& szamlaID,
                       const std::string& ugyfelID,
                       double egyenleg,
                       const Date& nyitas,
                       const std::string& diakIgazolvany)
    : Szamla(szamlaID, ugyfelID, egyenleg, nyitas)
{
    this->diakIgazolvany = diakIgazolvany;
}

double DiakSzamla::kamatSzamol(int /*ev*/) const {
    return 0.0;
}

std::string DiakSzamla::getTipus() const {
    return "DiakSzamla";
}

const std::string& DiakSzamla::getDiakIgazolvany() const {
    return diakIgazolvany;
}



