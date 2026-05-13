#pragma once
#include <string>
#include <iostream>

class MembershipPlan {
    std::string planType;
    float priceR;
    int durationD;

public:
    MembershipPlan(const std::string& type, float price, int days)
        : planType{type}, priceR{price}, durationD{days} {}
    const std::string& getPlanType() const { return planType; }
    bool isPremium() const { return priceR > 200.0f; }
    friend std::ostream& operator<<(std::ostream& os, const MembershipPlan& p);
};
