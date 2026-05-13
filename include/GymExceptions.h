#pragma once
#include <stdexcept>
#include <string>

// Baza ierarhiei de exceptii - independenta de ierarhia GymService
class GymException : public std::exception {
    std::string mesaj;
public:
    explicit GymException(const std::string& msg) : mesaj{msg} {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

// Membrul nu are tipul de abonament necesar pentru serviciu
class AccesInterzisException : public GymException {
public:
    AccesInterzisException(const std::string& numeMembru, const std::string& serviciu)
        : GymException("Accesul lui " + numeMembru + " la " + serviciu +
                       " este interzis cu planul curent") {}
};

// Membrul incearca sa acceseze un serviciu dar e deja ocupat
class MembruOcupatException : public GymException {
public:
    explicit MembruOcupatException(const std::string& numeMembru)
        : GymException("Membrul " + numeMembru + " este deja ocupat") {}
};

// Clasa de grup nu mai are locuri
class CapacitatePlinaException : public GymException {
public:
    explicit CapacitatePlinaException(const std::string& numeServiciu)
        : GymException("Serviciul " + numeServiciu + " nu mai are locuri disponibile") {}
};

// ID de membru inexistent
class MembruNegasitException : public GymException {
public:
    explicit MembruNegasitException(int id)
        : GymException("Membrul cu ID " + std::to_string(id) + " nu exista") {}
};

// Tip de plan necunoscut la citire din fisier
class PlanInvalidException : public GymException {
public:
    explicit PlanInvalidException(const std::string& tipPlan)
        : GymException("Tipul de plan '" + tipPlan + "' nu exista") {}
};
