#include "../include/PersonalTraining.h"
#include "../include/Member.h"
#include "../include/GymExceptions.h"

PersonalTraining::PersonalTraining(const std::string& antrenor, float cost, int durata)
    : GymService("PersonalTraining-" + antrenor, durata),
      numeAntrenor{antrenor}, costPerOra{cost} {}

GymService* PersonalTraining::clone() const {
    return new PersonalTraining(*this);
}

// Doar Premium si Annual au acces la personal training
void PersonalTraining::verificaAcces(const Member& m) const {
    if (m.isOcupat()) {
        throw MembruOcupatException(m.getName());
    }
    if (!m.getPlan().isPremium()) {
        throw AccesInterzisException(m.getName(), name);
    }
}

std::string PersonalTraining::calculeazaRezultat() const {
    float cost = (durataInitiala / 60.0f) * costPerOra;
    return "Cost sesiune: " + std::to_string(cost) + " RON"
           + " (antrenor: " + numeAntrenor + ")";
}

void PersonalTraining::afisare(std::ostream& os) const {
    GymService::afisare(os);
    os << " | Antrenor: " << numeAntrenor
       << " | Cost/ora: " << costPerOra << " RON";
}
