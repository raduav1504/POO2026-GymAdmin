#include "../include/Member.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const Member& m) {
    os << "Membru: " << std::left << std::setw(15) << m.name << " | ID: " << std::setw(4) << m.membershipID<< " | " << m.plan << " | Minute: " << m.totalMinutes;
    if (m.ocupat)
        os << " | Ocupat la: " << m.serviciuCurent;
    return os;
}
