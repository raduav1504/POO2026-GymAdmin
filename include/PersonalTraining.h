#pragma once
#include "GymService.h"

// Sedinta de personal training - accesibila doar Premium si mai sus
// calculeazaRezultat() returneaza costul total al sesiunii
class PersonalTraining : public GymService {
    std::string numeAntrenor;
    float       costPerOra;

protected:
    void afisare(std::ostream& os) const override;

public:
    PersonalTraining(const std::string& antrenor, float cost, int durata);

    GymService* clone() const override;

    // Doar Premium/Annual - arunca AccesInterzisException pentru Basic/Standard
    void verificaAcces(const Member& m) const override;

    // Returneaza costul: durataInitiala / 60.0 * costPerOra
    std::string calculeazaRezultat() const override;

};
