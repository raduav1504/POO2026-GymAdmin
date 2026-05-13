#include "../include/SaunaSession.h"
#include "../include/Member.h"
#include "../include/GymExceptions.h"

SaunaSession::SaunaSession(int durataMax, int durata)
    : GymService("SaunaSession", durata), durataMaxPermisa{durataMax} {}

GymService* SaunaSession::clone() const {
    return new SaunaSession(*this);
}

void SaunaSession::verificaAcces(const Member& m) const {
    if (m.isOcupat()) {
        throw MembruOcupatException(m.getName());
    }
    if (!m.getPlan().isPremium()) {
        throw AccesInterzisException(m.getName(), name);
    }
}

std::string SaunaSession::calculeazaRezultat() const {
    int minuteUtilizate = durataInitiala - durataRamasa;
    return "Sauna: " + std::to_string(minuteUtilizate) + "/" + std::to_string(durataMaxPermisa) + " minute utilizate";
}

void SaunaSession::afisare(std::ostream& os) const {
    GymService::afisare(os);
    os << " | Durata max permisa: " << durataMaxPermisa << " min";
}