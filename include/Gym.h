#pragma once
#include "Member.h"
#include "Equipment.h"
#include "GymService.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

class Gym {
    std::string            name;
    std::vector<Equipment> equipments;
    std::vector<Member>    members;
    std::vector<GymService*> services;  // pointeri la baza - apelam virtual prin ei

    // STL map: numele serviciului -> indexul in vectorul services
    // Permite cautare rapida a serviciului cand un echipament e folosit
    std::map<std::string, int> serviceIndex;

    // Functie privata: cauta indexul unui membru dupa ID, -1 daca nu exista
    int findMemberIndex(int id) const;

    // Functie privata: gaseste serviciul asociat unui echipament
    // Returneaza nullptr daca echipamentul nu are serviciu asociat
    GymService* findService(const std::string& serviceName) const;

public:
    explicit Gym(const std::string& name_);

    // Regula celor 3 - Gym are vector<GymService*> (pointeri raw)
    Gym(const Gym& other);
    Gym& operator=(Gym other); // copy-and-swap
    ~Gym();

    friend void swap(Gym& a, Gym& b);

    // --- Citire din fisiere ---
    void incarcaMembri(const std::string& fisier,
                       const std::vector<MembershipPlan>& planuri);
    void incarcaEchipamente(const std::string& fisier);
    // Format: CardioZone <intensitate> <durata>
    //         GroupClass <nume> <capacitate> <instructor> <durata>
    //         PersonalTraining <antrenor> <costPerOra> <durata>
    void incarcaServicii(const std::string& fisier);

    // --- Functii din T1 ---
    void adaugaMembru(const Member& m);
    void adaugaEchipament(const Equipment& e);

    // Porneste sesiune pe echipament - verifica si accesul la serviciu
    void startSesiune(int eqIndex, int memberID, int durata);

    // Avanseaza simularea cu 1 minut (echipamente + servicii)
    void updateSala();

    void raportComplet()    const;
    void afiseazaTopMembri() const;

    // --- Functii noi T2 ---

    // Inscrie membrul la un serviciu (GroupClass sau PersonalTraining)
    // Arunca exceptie daca nu are acces sau serviciul e plin
    void acceseazaServiciu(const std::string& numeServiciu, int memberID);

    // Afiseaza toate serviciile disponibile
    void afiseazaServicii() const;

    // Raport servicii - foloseste dynamic_cast pentru GroupClass
    void raportServicii() const;

    friend std::ostream& operator<<(std::ostream& os, const Gym& g);
};

// Functie libera - citeste planuri din fisier (nu depinde de o sala anume)
std::vector<MembershipPlan> incarcaPlanuri(const std::string& fisier);
