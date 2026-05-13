#pragma once
#include "GymService.h"

//zona de cardio- toti membrii au voie

class CardioZone : public GymService {
    int intensitate;        //din fisier

protected:
    void afisare(std::ostream& os) const override;

public:
    CardioZone(int intensitate_, int durata);

    GymService* clone() const override;

    void verificaAcces(const Member& m) const override;
    // caloriile arse
    std::string calculeazaRezultat() const override;
};
