#pragma once
#include "GymService.h"

class PersonalTraining : public GymService {
    std::string numeAntrenor;
    float costPerOra;

protected:
    void afisare(std::ostream& os) const override;

public:
    PersonalTraining(const std::string& antrenor, float cost, int durata);
    GymService* clone() const override;
    void verificaAcces(const Member& m) const override;

    std::string calculeazaRezultat() const override;

};
