#pragma once
#include "GymService.h"


class SaunaSession : public GymService {
    int durataMaxPermisa; 

protected:
    void afisare(std::ostream& os) const override;

public:
    SaunaSession(int durataMax, int durata);
    GymService* clone() const override;
    void verificaAcces(const Member& m) const override;
    std::string calculeazaRezultat() const override;
};