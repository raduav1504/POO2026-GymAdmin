#pragma once
#include <string>
#include <vector>
#include <iostream>

class Member;

// clasa pt toate serviciile
class GymService {
protected:
    std::string name;
    int durataRamasa;   
    int durataInitiala; 
    std::vector<int> idParticipanti; 

    virtual void afisare(std::ostream& os) const;     // interfata non-virtuala: operator<< apeleaza afisare()

public:
    GymService(const std::string& name_, int durata);
    GymService(const GymService& other);
    GymService& operator=(const GymService& other);
    virtual ~GymService();

    virtual GymService* clone() const = 0; // constructor virtual pt copiere pointer de baza

    virtual void verificaAcces(const Member& m) const = 0;
    virtual std::string calculeazaRezultat() const = 0;

    void adaugaParticipant(int memberID);
    bool esteActiv() const;
    bool update();

    const std::string& getName() const { return name; }
    const std::vector<int>& getParticipanti() const { return idParticipanti; }

    static int getTotalServiciiActive(); // contorul static pt cate servicii exista la un moment dat
    
    friend std::ostream& operator<<(std::ostream& os, const GymService& s);

private:
    static int totalServiciiActive;
};
