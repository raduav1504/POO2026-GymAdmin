#include "../include/Gym.h"
#include "../include/GymExceptions.h"
#include "../include/CardioZone.h"
#include "../include/GroupClass.h"
#include "../include/PersonalTraining.h"
#include "../include/SaunaSession.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>

// Gym are vector<GymService*> deci avem nevoie de regula celor 3

Gym::Gym(const std::string& name_) : name{name_} {}

Gym::Gym(const Gym& other)
    : name{other.name}, equipments{other.equipments},
      members{other.members}, serviceIndex{other.serviceIndex} {
    for (int i = 0; i < (int)other.services.size(); i++) {
        services.push_back(other.services[i]->clone());
    }
}

// copy-and-swap: primim other prin valoare, swapuim
Gym& Gym::operator=(Gym other) {
    swap(*this, other);
    return *this;
}

Gym::~Gym() {
    for (int i = 0; i < (int)services.size(); i++) {
        delete services[i];
    }
}

void swap(Gym& a, Gym& b) {
    std::swap(a.name, b.name);
    std::swap(a.equipments, b.equipments);
    std::swap(a.members, b.members);
    std::swap(a.services, b.services);
    std::swap(a.serviceIndex, b.serviceIndex);
}

// cauta membrul dupa ID, returneaza -1 daca nu exista
int Gym::findMemberIndex(int id) const {
    for (int i = 0; i < (int)members.size(); i++) {
        if (members[i].getID() == id) return i;
    }
    return -1;
}

// cautare in map O(log n) in loc de parcurgere liniara
GymService* Gym::findService(const std::string& serviceName) const {
    std::map<std::string, int>::const_iterator it = serviceIndex.find(serviceName);
    if (it != serviceIndex.end()) {
        return services[it->second];
    }
    return nullptr;
}

// format: TipPlan PretRON DurataDays
std::vector<MembershipPlan> incarcaPlanuri(const std::string& fisier) {
    std::vector<MembershipPlan> planuri;
    std::ifstream fin(fisier);
    if (!fin) { std::cerr << "Nu am gasit " << fisier << "\n"; return planuri; }
    std::string tip;
    float pret;
    int zile;
    while (fin >> tip >> pret >> zile) {
        planuri.push_back(MembershipPlan(tip, pret, zile));
    }
    std::cout << "[Sistem] " << planuri.size() << " planuri incarcate\n";
    return planuri;
}

// format: NumeMembru ID TipPlan
void Gym::incarcaMembri(const std::string& fisier,
                        const std::vector<MembershipPlan>& planuri) {
    std::ifstream fin(fisier);
    if (!fin) { std::cerr << "Nu am gasit " << fisier << "\n"; return; }
    std::string numeMembru, tipPlan;
    int id;
    while (fin >> numeMembru >> id >> tipPlan) {
        bool gasit = false;
        for (int i = 0; i < (int)planuri.size(); i++) {
            if (planuri[i].getPlanType() == tipPlan) {
                members.push_back(Member(numeMembru, id, planuri[i]));
                gasit = true;
                break;
            }
        }
        if (!gasit) {
            throw PlanInvalidException(tipPlan);
        }
    }
    std::cout << "[Sistem] " << members.size() << " membri incarcati\n";
}

// format: TipEchipament Serviciu (sau "none")
void Gym::incarcaEchipamente(const std::string& fisier) {
    std::ifstream fin(fisier);
    if (!fin) { std::cerr << "Nu am gasit " << fisier << "\n"; return; }
    std::string tip, serviciu;
    while (fin >> tip >> serviciu) {
        equipments.push_back(Equipment(tip, serviciu));
    }
    std::cout << "[Sistem] " << equipments.size() << " echipamente incarcate\n";
}

// format: CardioZone <intensitate> <durata>
//         GroupClass <nume> <capacitate> <instructor> <durata>
//         PersonalTraining <antrenor> <costPerOra> <durata>
//         SaunaSession <durataMax> <durata>
void Gym::incarcaServicii(const std::string& fisier) {
    std::ifstream fin(fisier);
    if (!fin) { std::cerr << "Nu am gasit " << fisier << "\n"; return; }
    std::string tip;
    while (fin >> tip) {
        GymService* s = nullptr;
        if (tip == "CardioZone") {
            int intensitate, durata;
            fin >> intensitate >> durata;
            s = new CardioZone(intensitate, durata);
        } else if (tip == "GroupClass") {
            std::string nume, instructor;
            int capacitate, durata;
            fin >> nume >> capacitate >> instructor >> durata;
            s = new GroupClass(nume, capacitate, instructor, durata);
        } else if (tip == "PersonalTraining") {
            std::string antrenor;
            float cost;
            int durata;
            fin >> antrenor >> cost >> durata;
            s = new PersonalTraining(antrenor, cost, durata);
        } else if (tip == "SaunaSession") {
            int durataMax, durata;
            fin >> durataMax >> durata;
            s = new SaunaSession(durataMax, durata);
        }
        if (s != nullptr) {
            serviceIndex[s->getName()] = (int)services.size();
            services.push_back(s);
        }
    }
    std::cout << "[Sistem] " << services.size() << " servicii incarcate\n";
}

void Gym::adaugaMembru(const Member& m)        { members.push_back(m); }
void Gym::adaugaEchipament(const Equipment& e) { equipments.push_back(e); }

void Gym::startSesiune(int eqIndex, int memberID, int durata) {
    if (eqIndex < 0 || eqIndex >= (int)equipments.size()) {
        std::cout << "Index echipament invalid\n"; return;
    }
    int mi = findMemberIndex(memberID);
    if (mi == -1) throw MembruNegasitException(memberID);
    if (members[mi].isOcupat()) throw MembruOcupatException(members[mi].getName());

    // daca echipamentul apartine unui serviciu, verificam accesul
    const std::string& serviciu = equipments[eqIndex].getServiceName();
    if (serviciu != "none") {
        const GymService* s = findService(serviciu);
        if (s != nullptr) s->verificaAcces(members[mi]);
    }
    equipments[eqIndex].startUsage(durata, members[mi].getName());
    members[mi].setOcupat(true, equipments[eqIndex].getType());
}

void Gym::updateSala() {
    for (int i = 0; i < (int)equipments.size(); i++) {
        std::string userActiv = equipments[i].getcUser();
        std::string finished  = equipments[i].update();
        if (!userActiv.empty()) {
            for (int j = 0; j < (int)members.size(); j++) {
                if (members[j].getName() == userActiv) {
                    members[j].addMinutes(1);
                    if (!finished.empty()) members[j].setOcupat(false);
                }
            }
        }
    }
    for (int i = 0; i < (int)services.size(); i++) {
        if (!services[i]->esteActiv()) continue;
        // salvam participantii inainte de update - update() ii sterge la final
        std::vector<int> participanti = services[i]->getParticipanti();
        bool terminat = services[i]->update();
        for (int j = 0; j < (int)participanti.size(); j++) {
            int mi = findMemberIndex(participanti[j]);
            if (mi != -1) {
                members[mi].addMinutes(1);
                if (terminat) members[mi].setOcupat(false);
            }
        }
    }
}

void Gym::raportComplet() const {
    int echipOcupate = 0;
    for (int i = 0; i < (int)equipments.size(); i++)
        if (equipments[i].isInUse()) echipOcupate++;

    int membriActivi = 0;
    for (int i = 0; i < (int)members.size(); i++)
        if (members[i].getTotalMinutes() > 0) membriActivi++;

    std::cout << "\n====== " << name << " ======\n";
    std::cout << "Echipamente: " << equipments.size()
              << " | Ocupate: " << echipOcupate
              << " | Libere: " << (equipments.size() - echipOcupate) << "\n";
    std::cout << "Membri: " << members.size()
              << " | Activi: " << membriActivi << "\n";
    std::cout << "Servicii active (global): "
              << GymService::getTotalServiciiActive() << "\n";

    if (!equipments.empty()) {
        int topIdx = 0;
        for (int i = 1; i < (int)equipments.size(); i++)
            if (equipments[i].getTotalUsage() > equipments[topIdx].getTotalUsage())
                topIdx = i;
        std::cout << "Top echipament: " << equipments[topIdx].getType()
                  << " (" << equipments[topIdx].getTotalUsage() << " min)\n";
    }
    if (!members.empty()) {
        int topIdx = 0;
        for (int i = 1; i < (int)members.size(); i++)
            if (members[i].getTotalMinutes() > members[topIdx].getTotalMinutes())
                topIdx = i;
        std::cout << "Cel mai activ: " << members[topIdx].getName()
                  << " (" << members[topIdx].getTotalMinutes() << " min)\n";
    }
    std::cout << "Membri premium: ";
    bool oricare = false;
    for (int i = 0; i < (int)members.size(); i++)
        if (members[i].getPlan().isPremium()) {
            std::cout << members[i].getName() << " ";
            oricare = true;
        }
    if (!oricare) std::cout << "niciunul";
    std::cout << "\n";
}

void Gym::afiseazaTopMembri() const {
    std::vector<int> ordine;
    for (int i = 0; i < (int)members.size(); i++)
        ordine.push_back(i);

    // selection sort descrescator dupa totalMinutes
    for (int i = 0; i < (int)ordine.size(); i++) {
        for (int j = i + 1; j < (int)ordine.size(); j++) {
            if (members[ordine[j]].getTotalMinutes() >
                members[ordine[i]].getTotalMinutes()) {
                int tmp = ordine[i];
                ordine[i] = ordine[j];
                ordine[j] = tmp;
            }
        }
    }
    std::cout << "\n--- Top Membri ---\n";
    for (int i = 0; i < (int)ordine.size(); i++)
        std::cout << i + 1 << ". " << members[ordine[i]] << "\n";
}

void Gym::acceseazaServiciu(const std::string& numeServiciu, int memberID) {
    int mi = findMemberIndex(memberID);
    if (mi == -1) throw MembruNegasitException(memberID);

    GymService* s = findService(numeServiciu);
    if (s == nullptr) {
        std::cout << "Serviciul '" << numeServiciu << "' nu exista.\n";
        return;
    }
    // verificaAcces e virtuala - fiecare derivata stie ce reguli aplica
    s->verificaAcces(members[mi]);

    s->adaugaParticipant(memberID);
    members[mi].setOcupat(true, numeServiciu);
    std::cout << "  " << members[mi].getName()
              << " s-a inscris la " << numeServiciu << "\n";
    std::cout << "  Rezultat estimat: " << s->calculeazaRezultat() << "\n";
}

void Gym::afiseazaServicii() const {
    std::cout << "\n--- Servicii disponibile ---\n";
    for (int i = 0; i < (int)services.size(); i++)
        std::cout << "[" << i << "] " << *services[i] << "\n";
    std::cout << "Total servicii (static): "
              << GymService::getTotalServiciiActive() << "\n";
}

// dynamic_cast pentru a accesa informatii specifice GroupClass
void Gym::raportServicii() const {
    std::cout << "\n--- Raport Servicii ---\n";
    for (int i = 0; i < (int)services.size(); i++) {
        std::cout << *services[i] << "\n";
        std::cout << "  Rezultat: " << services[i]->calculeazaRezultat() << "\n";
        const GroupClass* gc = dynamic_cast<const GroupClass*>(services[i]);
        if (gc != nullptr) {
            std::cout << "  Locuri libere: " << gc->getLocuriLibere()
                      << "/" << gc->getCapacitateMax() << "\n";
            std::cout << "  Instructor: " << gc->getInstructor() << "\n";
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Gym& g) {
    os << "\n=== Sala: " << g.name << " ===\n";
    os << "Echipamente (" << g.equipments.size() << "):\n";
    for (int i = 0; i < (int)g.equipments.size(); i++)
        os << "  [" << i << "] " << g.equipments[i] << "\n";
    os << "Membri (" << g.members.size() << "):\n";
    for (int i = 0; i < (int)g.members.size(); i++)
        os << "  " << g.members[i] << "\n";
    os << "Servicii (" << g.services.size() << "):\n";
    for (int i = 0; i < (int)g.services.size(); i++)
        os << "  [" << i << "] " << *g.services[i] << "\n";
    return os;
}