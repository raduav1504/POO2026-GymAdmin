#include "../include/GymService.h"

int GymService::totalServiciiActive = 0;

GymService::GymService(const std::string& name_, int durata)
    : name{name_}, durataRamasa{0}, durataInitiala{durata} {
    ++totalServiciiActive;
}

GymService::GymService(const GymService& other)
    : name{other.name}, durataRamasa{other.durataRamasa},
      durataInitiala{other.durataInitiala}, idParticipanti{other.idParticipanti} {
    ++totalServiciiActive;
}

GymService& GymService::operator=(const GymService& other) {
    if (this != &other) {
        name            = other.name;
        durataRamasa    = other.durataRamasa;
        durataInitiala  = other.durataInitiala;
        idParticipanti  = other.idParticipanti;
    }
    return *this;
}

GymService::~GymService() {
    --totalServiciiActive;
}

int GymService::getTotalServiciiActive() {
    return totalServiciiActive;
}

void GymService::adaugaParticipant(int memberID) {
    // Prima persoana care se inscrie porneste timer-ul sesiunii
    if (!esteActiv()) durataRamasa = durataInitiala;
    idParticipanti.push_back(memberID);
}

bool GymService::esteActiv() const {
    return durataRamasa > 0;
}

bool GymService::update() {
    if (!esteActiv()) return false;
    durataRamasa--;
    if (durataRamasa <= 0) {
        durataRamasa = 0;
        idParticipanti.clear();
        std::cout << "  [Serviciu] " << name << " s-a incheiat.\n";
        return true; // sesiunea s-a terminat
    }
    return false;
}

// Interfata non-virtuala: operator<< apeleaza afisare() virtuala
void GymService::afisare(std::ostream& os) const {
    os << "Serviciu: " << name
       << " | Ramas: " << durataRamasa << " min"
       << " | Participanti: " << idParticipanti.size();
}

std::ostream& operator<<(std::ostream& os, const GymService& s) {
    s.afisare(os); // dispatch virtual
    return os;
}
