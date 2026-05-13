#include "../include/Equipment.h"
#include <iomanip>

void Equipment::startUsage(int duration, const std::string& user) {
    if (!inUse) {
        inUse     = true;
        usageTimeR = duration;
        cUser     = user;
        std::cout << "  [" << type << "] inceput de " << user
                  << " pentru " << duration << " min\n";
    } else {
        std::cout << "  [" << type << "] este deja folosit de " << cUser << "\n";
    }
}

std::string Equipment::update() {
    if (!inUse) return "";
    usageTimeR--;
    totalMin++;
    if (usageTimeR <= 0) {
        std::string finishedUser = cUser;
        inUse = false;
        cUser = "";
        std::cout << "  [" << type << "] sesiune terminata pentru "
                  << finishedUser << "\n";
        return finishedUser;
    }
    return "";
}

std::ostream& operator<<(std::ostream& os, const Equipment& eq) {
    os << "Echipament: " << std::left << std::setw(15) << eq.type
       << " | Serviciu: " << std::setw(18) << eq.serviceName
       << " | Status: " << (eq.inUse ? "Ocupat" : "Liber ")
       << " | Total: " << eq.totalMin << " min";
    if (eq.inUse)
        os << " | Ramas: " << eq.usageTimeR << " min | User: " << eq.cUser;
    return os;
}
