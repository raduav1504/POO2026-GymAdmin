#pragma once
#include "GymService.h"

// Sesiune la sauna - accesibila doar Premium si Annual
// calculeazaRezultat() returneaza minutele utilizate din maximul permis
class SaunaSession : public GymService {
    int durataMaxPermisa; // minute maxime recomandate

protected:
    void afisare(std::ostream& os) const override;

public:
    SaunaSession(int durataMax, int durata);

    GymService* clone() const override;

    // Doar Premium/Annual - arunca AccesInterzisException pentru Basic/Standard
    void verificaAcces(const Member& m) const override;

    // Returneaza minutele utilizate din maximul permis
    std::string calculeazaRezultat() const override;
};