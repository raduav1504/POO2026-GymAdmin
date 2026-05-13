#pragma once
#include "Member.h"
#include "Equipment.h"
#include "GymService.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

class Gym {
    std::string name;
    std::vector<Equipment> equipments;
    std::vector<Member> members;
    std::vector<GymService*> services;  // ppointerul virtual

    std::map<std::string, int> serviceIndex; //map pt cautare rapida a unui serviciu
    int findMemberIndex(int id) const;
    GymService* findService(const std::string& serviceName) const;

public:
    explicit Gym(const std::string& name_);
    // regula celor 3 - Gym are vector<GymService*> (pointeri raw)
    Gym(const Gym& other);
    Gym& operator=(Gym other); // copy-and-swap
    ~Gym();

    friend void swap(Gym& a, Gym& b);

    void incarcaMembri(const std::string& fisier, const std::vector<MembershipPlan>& planuri);
    void incarcaEchipamente(const std::string& fisier);
    void incarcaServicii(const std::string& fisier);

    // fcuntiile din T1
    void adaugaMembru(const Member& m);
    void adaugaEchipament(const Equipment& e);
    void startSesiune(int eqIndex, int memberID, int durata);
    void updateSala();
    void raportComplet()    const;
    void afiseazaTopMembri() const;

    //functiile noi T2
    void acceseazaServiciu(const std::string& numeServiciu, int memberID);
    void afiseazaServicii() const; //toate serviciile 
    void raportServicii() const; //dinamic cast pt groupClass

    friend std::ostream& operator<<(std::ostream& os, const Gym& g);
};


std::vector<MembershipPlan> incarcaPlanuri(const std::string& fisier);
