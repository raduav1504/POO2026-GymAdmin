#pragma once
#include <string>
#include <vector>
#include <iostream>

class Member; // forward declaration - evitam include circular

// Clasa de baza abstracta pentru serviciile oferite de sala
// Ierarhia: GymService <- CardioZone, GroupClass, PersonalTraining
//           (SaunaSession adaugata in commit separat)
class GymService {
protected:
    std::string      name;
    int              durataRamasa;   // minute ramase din sesiunea curenta
    int              durataInitiala; // durata totala configurata
    std::vector<int> idParticipanti; // ID-urile membrilor activi

    // Interfata non-virtuala: operator<< apeleaza afisare()
    virtual void afisare(std::ostream& os) const;

public:
    GymService(const std::string& name_, int durata);
    GymService(const GymService& other);
    GymService& operator=(const GymService& other);
    virtual ~GymService();

    // Constructor virtual - necesar pentru copierea corecta prin pointer de baza
    virtual GymService* clone() const = 0;

    // Verifica daca membrul are dreptul sa acceseze serviciul.
    // Arunca AccesInterzisException sau CapacitatePlinaException daca nu.
    virtual void verificaAcces(const Member& m) const = 0;

    // Calculeaza si returneaza rezultatul sesiunii (specific fiecarui tip)
    virtual std::string calculeazaRezultat() const = 0;

    void adaugaParticipant(int memberID);
    bool esteActiv() const;

    // Avanseaza cu 1 minut; returneaza true daca sesiunea tocmai s-a terminat
    bool update();

    const std::string&      getName()        const { return name; }
    const std::vector<int>& getParticipanti() const { return idParticipanti; }

    // Contor static: cate servicii exista in acest moment
    static int getTotalServiciiActive();

    friend std::ostream& operator<<(std::ostream& os, const GymService& s);

private:
    static int totalServiciiActive;
};
