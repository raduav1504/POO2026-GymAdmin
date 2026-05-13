#include "../include/CardioZone.h"
#include "../include/Member.h"
#include "../include/GymExceptions.h"

CardioZone::CardioZone(int intensitate_, int durata)
    : GymService("CardioZone", durata), intensitate{intensitate_} {}

GymService* CardioZone::clone() const {
    return new CardioZone(*this);
}

void CardioZone::verificaAcces(const Member& m) const {
    if (m.isOcupat()) {
        throw MembruOcupatException(m.getName());
    }
}

std::string CardioZone::calculeazaRezultat() const {
    int durata= durataInitiala - durataRamasa;
    int calorii = durata * intensitate * 5;
    return "Calorii arse estimat: " + std::to_string(calorii) + " kcal"+ " (intensitate " + std::to_string(intensitate) + "/10)";
}
void CardioZone::afisare(std::ostream& os) const {
    GymService::afisare(os); 
    os << " | Intensitate: " << intensitate << "/10";
}
