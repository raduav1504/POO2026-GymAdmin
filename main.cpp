// Gym Manager - Proiect POO
// Clase: MembershipPlan, Member (compunere cu MembershipPlan),
//        Equipment (regula celor 3 cu char*), Gym (compunere cu toate)
// Date citite din fisiere TXT: plans.txt, members.txt, equipments.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <algorithm>


// ============================================================================
// 1. CLASA MEMBERSHIPPLAN
//    Reprezinta un tip de abonament (ex: "Basic", 30 zile, 100 RON)
//    Simpla, fara memorie dinamica - nu are nevoie de regula celor 3
// ============================================================================
class MembershipPlan {
private:
    std::string planType;    // "Basic", "Standard", "Premium"
    float       priceRON;    // pretul abonamentului
    int         durationDays;// durata in zile

public:
    MembershipPlan(const std::string& type, float price, int days)
        : planType{type}, priceRON{price}, durationDays{days} {}

    const std::string& getPlanType()    const { return planType; }

    // Returneaza true daca abonamentul este premium (pret > 200 RON)
    bool isPremium() const { return priceRON > 200.0f; }

    friend std::ostream& operator<<(std::ostream& os, const MembershipPlan& p) {
        os << "Plan: " << std::left << std::setw(10) << p.planType
           << " | Pret: " << std::setw(7) << p.priceRON << " RON"
           << " | Durata: " << p.durationDays << " zile";
        return os;
    }
};

// ============================================================================
// 2. CLASA MEMBER
//    Un membru al salii cu nume, ID si abonamentul sau (compunere MembershipPlan)
//    Regula celor 3 aplicata pentru a demonstra gestionarea membrilor
// ============================================================================
class Member {
private:
    std::string    name;
    int            membershipID;
    MembershipPlan plan;          // compunere: fiecare membru are un plan
    int            totalMinutes;  // minute totale petrecute la sala

public:
    Member(const std::string& name_, int id, const MembershipPlan& plan_)
        : name{name_}, membershipID{id}, plan{plan_}, totalMinutes{0} {}

    // Constructor de copiere
    Member(const Member& other)
        : name{other.name}, membershipID{other.membershipID},
          plan{other.plan}, totalMinutes{other.totalMinutes} {}

    // Operator de atribuire
    Member& operator=(const Member& other) {
        if (this != &other) {
            name         = other.name;
            membershipID = other.membershipID;
            plan         = other.plan;
            totalMinutes = other.totalMinutes;
        }
        return *this;
    }

    ~Member() {}

    const std::string&    getName()    const { return name; }
    int                   getID()      const { return membershipID; }
    const MembershipPlan& getPlan()    const { return plan; }
    int                   getTotalMinutes() const { return totalMinutes; }

    void addMinutes(int minutes) { totalMinutes += minutes; }

    friend std::ostream& operator<<(std::ostream& os, const Member& m) {
        os << "Membru: " << std::left << std::setw(15) << m.name
           << " | ID: " << std::setw(4) << m.membershipID
           << " | " << m.plan
           << " | Minute totale: " << m.totalMinutes;
        return os;
    }
};

// ============================================================================
// 3. CLASA EQUIPMENT
//    Reprezinta un echipament din sala.
//    Foloseste char* pentru 'type' - demonstreaza regula celor 3 cu memorie dinamica
// ============================================================================
class Equipment {
private:
    char* type;               // numele echipamentului (memorie dinamica)
    bool  inUse;
    int   usageTimeRemaining; // minute ramase din sesiunea curenta
    int   totalUsageMinutes;  // statistici: total minute folosit
    std::string currentUser;

public:
   explicit Equipment(const char* type_, int totalUsage = 0)
        : inUse{false}, usageTimeRemaining{0},
          totalUsageMinutes{totalUsage}, currentUser{""} {
        type = new char[strlen(type_) + 1];
        strcpy(type, type_);
    }

    // Constructor de copiere - copiem manual bufferul char*
    Equipment(const Equipment& other)
        : inUse{other.inUse}, usageTimeRemaining{other.usageTimeRemaining},
          totalUsageMinutes{other.totalUsageMinutes}, currentUser{other.currentUser} {
        type = new char[strlen(other.type) + 1];
        strcpy(type, other.type);
    }

    // Operator de atribuire - eliberam vechiul buffer inainte de a copia
    Equipment& operator=(const Equipment& other) {
        if (this != &other) {
            delete[] type;
            type = new char[strlen(other.type) + 1];
            strcpy(type, other.type);
            inUse              = other.inUse;
            usageTimeRemaining = other.usageTimeRemaining;
            totalUsageMinutes  = other.totalUsageMinutes;
            currentUser        = other.currentUser;
        }
        return *this;
    }

    ~Equipment() { delete[] type; }

    const char* getType()             const { return type; }
    bool        isInUse()             const { return inUse; }
    int         getTotalUsage()       const { return totalUsageMinutes; }

    // Incepe o sesiune de utilizare pentru un membru, cu durata in minute
    void startUsage(int duration, const std::string& user) {
        if (!inUse) {
            inUse              = true;
            usageTimeRemaining = duration;
            currentUser        = user;
            std::cout << "  [" << type << "] inceput de " << user
                      << " pentru " << duration << " min.\n";
        } else {
            std::cout << "  [" << type << "] este deja folosit de "
                      << currentUser << ".\n";
        }
    }

    // Avanseaza simularea cu un pas de timp (1 minut)
    // Returneaza numele userului daca sesiunea s-a terminat, "" altfel
    std::string update() {
        if (!inUse) return "";
        usageTimeRemaining--;
        totalUsageMinutes++;
        if (usageTimeRemaining <= 0) {
            std::string finishedUser = currentUser;
            inUse       = false;
            currentUser = "";
            std::cout << "  [" << type << "] sesiune terminata pentru "
                      << finishedUser << ".\n";
            return finishedUser;
        }
        return "";
    }

    friend std::ostream& operator<<(std::ostream& os, const Equipment& eq) {
        os << "Echipament: " << std::left << std::setw(15) << eq.type
           << " | Status: " << (eq.inUse ? "Ocupat " : "Liber  ")
           << " | Total utilizare: " << eq.totalUsageMinutes << " min";
        if (eq.inUse)
            os << " | Ramas: " << eq.usageTimeRemaining
               << " min | Folosit de: " << eq.currentUser;
        return os;
    }
};

// ============================================================================
// 4. CLASA GYM
//    Sala de fitness - contine echipamente si membri (compunere cu ambele)
//    Functii netriviale: citire din fisiere, raport complet, statistici membri
// ============================================================================
class Gym {
private:
    std::string            name;
    std::vector<Equipment> equipments;
    std::vector<Member>    members;

    // Cauta indexul unui membru dupa ID; returneaza -1 daca nu e gasit
    int findMemberIndex(int id) const {
        for (int i = 0; i < (int)members.size(); i++)
            if (members[i].getID() == id) return i;
        return -1;
    }

public:
    explicit Gym(const std::string& name_) : name{name_} {}

    Gym(const Gym& other)
        : name{other.name}, equipments{other.equipments}, members{other.members} {}

    Gym& operator=(const Gym& other) {
        if (this != &other) {
            name       = other.name;
            equipments = other.equipments;
            members    = other.members;
        }
        return *this;
    }

    ~Gym() {}

    // --- Citire din fisiere TXT ---

    // Citeste planuri de abonament din fisier si le returneaza
    // Format linie: TipPlan PretRON DurataDays
    // Exemplu: Basic 100.0 30
    static std::vector<MembershipPlan> incarcaPlanuri(const std::string& fisier) {
        std::vector<MembershipPlan> planuri;
        std::ifstream fin(fisier);
        if (!fin) {
            std::cerr << "Eroare: Nu am gasit " << fisier << "\n";
            return planuri;
        }
        std::string tip;
        float pret;
        int zile;
        while (fin >> tip >> pret >> zile)
            planuri.emplace_back(tip, pret, zile);
        std::cout << "[Sistem] " << planuri.size() << " planuri incarcate din "
                  << fisier << "\n";
        return planuri;
    }

    // Citeste membri din fisier; are nevoie de planurile deja incarcate
    // Format linie: NumeMembru ID TipPlan
    // Exemplu: Ion 1001 Basic
    void incarcaMembri(const std::string& fisier,
                       const std::vector<MembershipPlan>& planuri) {
        std::ifstream fin(fisier);
        if (!fin) {
            std::cerr << "Eroare: Nu am gasit " << fisier << "\n";
            return;
        }
        std::string numeMembru, tipPlan;
        int id;
        while (fin >> numeMembru >> id >> tipPlan) {
            // Cautam planul corespunzator tipului citit din fisier
            bool gasit = false;
            for (const auto& p : planuri) {
                if (p.getPlanType() == tipPlan) {
                    members.emplace_back(numeMembru, id, p);
                    gasit = true;
                    break;
                }
            }
            if (!gasit)
                std::cerr << "  Atentie: planul '" << tipPlan
                          << "' nu exista pentru membrul " << numeMembru << "\n";
        }
        std::cout << "[Sistem] " << members.size() << " membri incarcati din "
                  << fisier << "\n";
    }

    // Citeste echipamente din fisier
    // Format linie: TipEchipament
    // Exemplu: Treadmill
    void incarcaEchipamente(const std::string& fisier) {
        std::ifstream fin(fisier);
        if (!fin) {
            std::cerr << "Eroare: Nu am gasit " << fisier << "\n";
            return;
        }
        std::string tip;
        while (fin >> tip)
            equipments.emplace_back(tip.c_str());
        std::cout << "[Sistem] " << equipments.size()
                  << " echipamente incarcate din " << fisier << "\n";
    }

    // --- Functii de interactiune ---

    void adaugaMembru(const Member& m)       { members.push_back(m); }
    void adaugaEchipament(const Equipment& e) { equipments.push_back(e); }

    // Porneste o sesiune: membrul cu 'memberID' foloseste echipamentul cu 'eqIndex'
    void startSesiune(int eqIndex, int memberID, int durata) {
        if (eqIndex < 0 || eqIndex >= (int)equipments.size()) {
            std::cout << "Index echipament invalid.\n"; return;
        }
        int mi = findMemberIndex(memberID);
        if (mi == -1) {
            std::cout << "Membrul cu ID " << memberID << " nu exista.\n"; return;
        }
        equipments[eqIndex].startUsage(durata, members[mi].getName());
    }

    // Avanseaza simularea cu un pas: actualizeaza toate echipamentele
    // si acumuleaza minutele pentru membrii care au terminat sesiunea
    void updateSala() {
        for (auto& eq : equipments) {
            std::string userTerminat = eq.update();
            if (!userTerminat.empty()) {
                // Creditam 1 minut membrului care a terminat sesiunea
                for (auto& m : members)
                    if (m.getName() == userTerminat)
                        m.addMinutes(1);
            }
        }
    }

    // --- Functii netriviale ---

    // Raport complet: afiseaza ocuparea salii, membri activi, top echipamente
    void raportComplet() const {
        int echipOcupate = 0;
        for (const auto& eq : equipments)
            if (eq.isInUse()) echipOcupate++;

        int membriActivi = 0;
        for (const auto& m : members)
            if (m.getTotalMinutes() > 0) membriActivi++;

        // Gasim echipamentul cel mai folosit
        const Equipment* topEq = nullptr;
        for (const auto& eq : equipments)
            if (!topEq || eq.getTotalUsage() > topEq->getTotalUsage())
                topEq = &eq;

        // Gasim membrul cu cele mai multe minute
        const Member* topMembru = nullptr;
        for (const auto& m : members)
            if (!topMembru || m.getTotalMinutes() > topMembru->getTotalMinutes())
                topMembru = &m;

        std::cout << "\n====== RAPORT SALA: " << name << " ======\n";
        std::cout << "Echipamente totale : " << equipments.size()
                  << " | Ocupate: " << echipOcupate
                  << " | Libere: " << (equipments.size() - echipOcupate) << "\n";
        std::cout << "Membri totali      : " << members.size()
                  << " | Cu activitate: " << membriActivi << "\n";

        if (topEq)
            std::cout << "Top echipament     : " << topEq->getType()
                      << " (" << topEq->getTotalUsage() << " min)\n";

        if (topMembru)
            std::cout << "Cel mai activ      : " << topMembru->getName()
                      << " (" << topMembru->getTotalMinutes() << " min)\n";

        // Membri cu abonament premium (pret > 200 RON)
        std::cout << "Membri premium     : ";
        bool oricare = false;
        for (const auto& m : members)
            if (m.getPlan().isPremium()) {
                std::cout << m.getName() << " ";
                oricare = true;
            }
        if (!oricare) std::cout << "niciunul";
        std::cout << "\n";
        std::cout << "=====================================\n";
    }

    // Afiseaza toti membrii sortati dupa minutele totale (descrescator)
    void afiseazaTopMembri() const {
        std::vector<const Member*> sortati;
        for (const auto& m : members) sortati.push_back(&m);
        std::sort(sortati.begin(), sortati.end(),
                  [](const Member* a, const Member* b) {
                      return a->getTotalMinutes() > b->getTotalMinutes();
                  });

        std::cout << "\n--- Top Membri dupa activitate ---\n";
        for (size_t i = 0; i < sortati.size(); i++)
            std::cout << i+1 << ". " << *sortati[i] << "\n";
    }

    // operator<< pentru Gym: afiseaza toate echipamentele si toti membrii
    friend std::ostream& operator<<(std::ostream& os, const Gym& g) {
        os << "\n=== Sala: " << g.name << " ===\n";
        os << "Echipamente (" << g.equipments.size() << "):\n";
        for (size_t i = 0; i < g.equipments.size(); i++)
            os << "  [" << i << "] " << g.equipments[i] << "\n";
        os << "Membri (" << g.members.size() << "):\n";
        for (const auto& m : g.members)
            os << "  " << m << "\n";
        return os;
    }
};

// ============================================================================
// MAIN
// ============================================================================
int main() {
    // 1. Incarcam planurile de abonament (necesar inainte de membri)
    std::vector<MembershipPlan> planuri = Gym::incarcaPlanuri("plans.txt");

    // 2. Cream sala si incarcam datele din fisiere
    Gym sala("FitZone");
    sala.incarcaMembri("members.txt", planuri);
    sala.incarcaEchipamente("equipments.txt");

    // 3. Afisam starea initiala
    std::cout << sala;

    // 4. Meniu interactiv
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- Meniu ---\n"
                  << "1. Incepe sesiune (echipament + membru + durata)\n"
                  << "2. Avanseaza simularea cu 1 minut\n"
                  << "3. Afiseaza starea salii\n"
                  << "4. Raport complet\n"
                  << "5. Top membri dupa activitate\n"
                  << "6. Adauga echipament nou\n"
                  << "7. Adauga membru nou\n"
                  << "0. Iesire\n"
                  << "Optiune: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int eqIdx, memberID, durata;
            std::cout << "Index echipament: "; std::cin >> eqIdx;
            std::cout << "ID membru: ";        std::cin >> memberID;
            std::cout << "Durata (min): ";     std::cin >> durata;
            sala.startSesiune(eqIdx, memberID, durata);
            break;
        }
        case 2:
            sala.updateSala();
            std::cout << "Simulare avansata cu 1 minut.\n";
            break;
        case 3:
            std::cout << sala;
            break;
        case 4:
            sala.raportComplet();
            break;
        case 5:
            sala.afiseazaTopMembri();
            break;
        case 6: {
            std::string tip;
            std::cout << "Tip echipament: ";
            std::cin >> tip;
            sala.adaugaEchipament(Equipment(tip.c_str()));
            std::cout << "Echipament adaugat.\n";
            break;
        }
        case 7: {
            std::string numeMembru, tipPlan;
            int id;
            std::cout << "Nume: ";    std::cin >> numeMembru;
            std::cout << "ID: ";      std::cin >> id;
            std::cout << "Tip plan (Basic/Standard/Premium/Annual): ";
            std::cin >> tipPlan;
            // Cautam planul in lista incarcata din fisier
            bool gasit = false;
            for (const auto& p : planuri) {
                if (p.getPlanType() == tipPlan) {
                    sala.adaugaMembru(Member(numeMembru, id, p));
                    std::cout << "Membru adaugat.\n";
                    gasit = true;
                    break;
                }
            }
            if (!gasit)
                std::cout << "Tipul de plan '" << tipPlan << "' nu exista.\n";
            break;
        }
        case 0:
            std::cout << "La revedere!\n";
            break;
        default:
            std::cout << "Optiune invalida.\n";
        }
    }
    return 0;
}
