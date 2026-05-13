#pragma once
#include "MembershipPlan.h"
#include <string>
#include <iostream>

class Member {
    std::string    name;
    int            membershipID;
    MembershipPlan plan;
    int            totalMinutes;
    bool           ocupat;           // true cand e inscris la un serviciu
    std::string    serviciuCurent;   // numele serviciului la care e inscris

public:
    Member(const std::string& name_, int id, const MembershipPlan& plan_)
        : name{name_}, membershipID{id}, plan{plan_},
          totalMinutes{0}, ocupat{false}, serviciuCurent{""} {}

    // Constructor de copiere
    Member(const Member& other)
        : name{other.name}, membershipID{other.membershipID},
          plan{other.plan}, totalMinutes{other.totalMinutes},
          ocupat{other.ocupat}, serviciuCurent{other.serviciuCurent} {}

    // Operator de atribuire
    Member& operator=(const Member& other) {
        if (this != &other) {
            name           = other.name;
            membershipID   = other.membershipID;
            plan           = other.plan;
            totalMinutes   = other.totalMinutes;
            ocupat         = other.ocupat;
            serviciuCurent = other.serviciuCurent;
        }
        return *this;
    }

    ~Member() {}

    const std::string&    getName()           const { return name; }
    int                   getID()             const { return membershipID; }
    const MembershipPlan& getPlan()           const { return plan; }
    int                   getTotalMinutes()   const { return totalMinutes; }
    bool                  isOcupat()          const { return ocupat; }
    const std::string&    getServiciuCurent() const { return serviciuCurent; }

    void addMinutes(int minutes) { totalMinutes += minutes; }

    void setOcupat(bool val, const std::string& serviciu = "") {
        ocupat         = val;
        serviciuCurent = val ? serviciu : "";
    }

    friend std::ostream& operator<<(std::ostream& os, const Member& m);
};
