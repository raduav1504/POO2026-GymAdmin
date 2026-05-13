#pragma once
#include <string>
#include <iostream>

// Plan de abonament - clasa concreta, nu face parte din ierarhia cu virtuale
class MembershipPlan {
    std::string planType;
    float       priceR;
    int         durationD;

public:
    MembershipPlan(const std::string& type, float price, int days)
        : planType{type}, priceR{price}, durationD{days} {}

    const std::string& getPlanType() const { return planType; }

    // Returneaza true daca pretul depaseste 200 RON
    bool isPremium() const { return priceR > 200.0f; }

    friend std::ostream& operator<<(std::ostream& os, const MembershipPlan& p);
};
