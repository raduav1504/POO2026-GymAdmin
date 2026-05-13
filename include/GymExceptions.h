#pragma once
#include <stdexcept>
#include <string>

// clasa de baza toate expceptiile
class GymException : public std::exception {
    std::string mesaj;
public:
    explicit GymException(const std::string& msg) : mesaj{msg} {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

// membrul nu are un abonament suficient de bun
class AccesInterzisException : public GymException {
public:
    AccesInterzisException(const std::string& numeMembru, const std::string& serviciu) : GymException("Accesul lui " + numeMembru + " la " + serviciu + " este interzis cu planul curent") {}
};

// memmbrul incearca sa acceseze un serviciu ocupat
class MembruOcupatException : public GymException {
public:
    explicit MembruOcupatException(const std::string& numeMembru) : GymException("Membrul " + numeMembru + " este deja ocupat") {}
};

// clasa de grup e full
class CapacitatePlinaException : public GymException {
public:
    explicit CapacitatePlinaException(const std::string& numeServiciu) : GymException("Serviciul " + numeServiciu + " nu mai are locuri disponibile") {}
};

// id membru inexistent
class MembruNegasitException : public GymException {
public:
    explicit MembruNegasitException(int id)
        : GymException("Membrul cu ID " + std::to_string(id) + " nu exista") {}
};

// tip de plan nerecunoscut la citire fisier
class PlanInvalidException : public GymException {
public:
    explicit PlanInvalidException(const std::string& tipPlan) : GymException("Tipul de plan '" + tipPlan + "' nu exista") {}
};
