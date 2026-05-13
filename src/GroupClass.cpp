#include "../include/GroupClass.h"
#include "../include/Member.h"
#include "../include/GymExceptions.h"

GroupClass::GroupClass(const std::string& name_, int capacitate,
                       const std::string& instructor_, int durata)
    : GymService(name_, durata),
      capacitateMax{capacitate}, instructor{instructor_} {}

GymService* GroupClass::clone() const {
    return new GroupClass(*this);
}

// Basic nu are acces la clase de grup
void GroupClass::verificaAcces(const Member& m) const {
    if (m.isOcupat()) {
        throw MembruOcupatException(m.getName());
    }
    if (m.getPlan().getPlanType() == "Basic") {
        throw AccesInterzisException(m.getName(), name);
    }
    if (getLocuriLibere() <= 0) {
        throw CapacitatePlinaException(name);
    }
}

std::string GroupClass::calculeazaRezultat() const {
    int participanti = (int)idParticipanti.size();
    int rata = (capacitateMax > 0) ? (participanti * 100 / capacitateMax) : 0;
    return "Ocupare: " + std::to_string(participanti) + "/" +
           std::to_string(capacitateMax) + " (" + std::to_string(rata) + "%)";
}

int GroupClass::getLocuriLibere() const {
    return capacitateMax - (int)idParticipanti.size();
}

void GroupClass::afisare(std::ostream& os) const {
    GymService::afisare(os);
    os << " | Instructor: " << instructor
       << " | Locuri libere: " << getLocuriLibere() << "/" << capacitateMax;
}
