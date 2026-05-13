#pragma once
#include "GymService.h"

// Zona de cardio - accesibila tuturor membrilor
// calculeazaRezultat() returneaza caloriile arse estimat
class CardioZone : public GymService {
    int intensitate;         // nivel 1-10, citit din fisier

protected:
    void afisare(std::ostream& os) const override;

public:
    CardioZone(int intensitate_, int durata);

    GymService* clone() const override;

    // Toti membrii au acces la CardioZone
    void verificaAcces(const Member& m) const override;

    // Returneaza caloriile arse: durata * intensitate * 5 cal/min
    std::string calculeazaRezultat() const override;
};
