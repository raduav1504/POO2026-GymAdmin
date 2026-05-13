#pragma once
#include "GymService.h"

// Clasa de grup (Yoga, Zumba, etc.) - accesibila doar Standard si mai sus
// calculeazaRezultat() returneaza rata de ocupare
class GroupClass : public GymService {
    int         capacitateMax;
    std::string instructor;

protected:
    void afisare(std::ostream& os) const override;

public:
    GroupClass(const std::string& name_, int capacitate,
               const std::string& instructor_, int durata);

    GymService* clone() const override;

    // Basic nu are acces - arunca AccesInterzisException
    // Daca e plin - arunca CapacitatePlinaException
    void verificaAcces(const Member& m) const override;

    // Returneaza rata de ocupare: participanti/capacitate * 100%
    std::string calculeazaRezultat() const override;

    int                getCapacitateMax()  const { return capacitateMax; }
    const std::string& getInstructor()     const { return instructor; }
    int                getLocuriLibere()   const;
};
