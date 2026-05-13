#pragma once
#include <string>
#include <iostream>

class Equipment {
    std::string type;
    std::string serviceName; // serviciul caruia ii apartine ("none" daca e liber)
    bool        inUse;
    int         usageTimeR;
    int         totalMin;
    std::string cUser;

public:
    explicit Equipment(const std::string& type_, const std::string& service = "none")
        : type{type_}, serviceName{service},
          inUse{false}, usageTimeR{0}, totalMin{0}, cUser{""} {}

    const std::string& getType()        const { return type; }
    const std::string& getServiceName() const { return serviceName; }
    bool               isInUse()        const { return inUse; }
    int                getTotalUsage()  const { return totalMin; }
    const std::string& getcUser()       const { return cUser; }

    void startUsage(int duration, const std::string& user);

    // Avanseaza cu 1 minut; returneaza numele userului daca sesiunea s-a terminat
    std::string update();

    friend std::ostream& operator<<(std::ostream& os, const Equipment& eq);
};
