#pragma once
#include "GymService.h"

// Clase de grup- accesibile doar Standard si mai sus
class GroupClass : public GymService {
    int capacitateMax;
    std::string instructor;

protected:
    void afisare(std::ostream& os) const override;

public:
    GroupClass(const std::string& name_, int capacitate, const std::string& instructor_, int durata);

    GymService* clone() const override;

    void verificaAcces(const Member& m) const override; // basic nu are acces- verifica si daca e plin (exceptii)

    // rata de ocupare
    std::string calculeazaRezultat() const override;

    int getCapacitateMax()  const { return capacitateMax; }
    const std::string& getInstructor() const { return instructor; }
    int getLocuriLibere() const;
};
