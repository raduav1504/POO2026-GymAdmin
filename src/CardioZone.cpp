#include "../include/CardioZone.h"
#include "../include/Member.h"
#include "../include/GymExceptions.h"

CardioZone::CardioZone(int intensitate_, int durata)
    : GymService("CardioZone", durata),
      intensitate{intensitate_}, caloriileTotaleArse{0} {}

GymService* CardioZone::clone() const {
    return new CardioZone(*this);
}

// Toti membrii au acces la CardioZone - nu aruncam exceptie
// Verificam doar daca membrul e ocupat (verificarea se face in Gym)
void CardioZone::verificaAcces(const Member& m) const {
    if (m.isOcupat()) {
        throw MembruOcupatException(m.getName());
    }
}

std::string CardioZone::calculeazaRezultat() const {
    // Formula: durata * intensitate * 5 calorii/minut
    int durata   = durataInitiala - durataRamasa; // minute efectuate
    int calorii  = durata * intensitate * 5;
    return "Calorii arse estimat: " + std::to_string(calorii) + " kcal"
           + " (intensitate " + std::to_string(intensitate) + "/10)";
}

void CardioZone::afisare(std::ostream& os) const {
    GymService::afisare(os); // apelam afisarea bazei
    os << " | Intensitate: " << intensitate << "/10";
}
