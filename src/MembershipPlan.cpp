#include "../include/MembershipPlan.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const MembershipPlan& p) {
    os << "Plan: " << std::left << std::setw(10) << p.planType<< " | Pret: " << p.priceR << " RON"<< " | Durata: " << p.durationD << " zile";
    return os;
}
