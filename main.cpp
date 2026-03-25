#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class MembershipPlan {
    std::string planType;
    float priceRON;
    int durationDays;
public:
    MembershipPlan(const std::string& type, float price, int days):  planType{type}, priceRON{price}, durationDays{days} {}

    const std::string& getPlanType() const { return planType; }
    bool isPremium() const { return priceRON > 200.0f; }

    friend std::ostream& operator<<(std::ostream& os, const MembershipPlan& p) {
        os << "Plan: " << p.planType
           << " | Pret: " << p.priceRON << " RON"
           << " | Durata: " << p.durationDays << " zile";
        return os;
    }
};

class Member {
    std::string name;
    int membershipID;
    MembershipPlan plan;
    int totalMinutes;
public:
    Member(const std::string& name_, int id, const MembershipPlan& plan_)
        : name{name_}, membershipID{id}, plan{plan_}, totalMinutes{0} {}

    Member(const Member& other)
        : name{other.name}, membershipID{other.membershipID},
          plan{other.plan}, totalMinutes{other.totalMinutes} {}

    Member& operator=(const Member& other) {
        if (this != &other) {
            name = other.name;
            membershipID = other.membershipID;
            plan = other.plan;
            totalMinutes = other.totalMinutes;
        }
        return *this;
    }

    ~Member() {}

    const std::string& getName() const { return name; }
    int getID() const { return membershipID; }
    const MembershipPlan& getPlan() const { return plan; }
    int getTotalMinutes() const { return totalMinutes; }
    void addMinutes(int minutes) { totalMinutes += minutes; }

    friend std::ostream& operator<<(std::ostream& os, const Member& m) {
        os << "Membru: " << m.name
           << " | ID: " << m.membershipID
           << " | " << m.plan
           << " | Minute totale: " << m.totalMinutes;
        return os;
    }
};

class Equipment {
    std::string type;
    bool inUse;
    int usageTimeRemaining;
    int totalUsageMinutes;
    std::string currentUser;
public:
    explicit Equipment(const std::string& type_)
        : type{type_}, inUse{false}, usageTimeRemaining{0},
          totalUsageMinutes{0}, currentUser{""} {}

    const std::string& getType() const { return type; }
    bool isInUse() const { return inUse; }
    int getTotalUsage() const { return totalUsageMinutes; }
    const std::string& getCurrentUser() const { return currentUser; }

    void startUsage(int duration, const std::string& user) {
        if (!inUse) {
            inUse = true;
            usageTimeRemaining = duration;
            currentUser = user;
            std::cout << "  [" << type << "] inceput de " << user
                      << " pentru " << duration << " min.\n";
        } else {
            std::cout << "  [" << type << "] este deja folosit de "
                      << currentUser << ".\n";
        }
    }

    // returneaza numele userului daca sesiunea s-a terminat, "" altfel
    std::string update() {
        if (!inUse) return "";
        usageTimeRemaining--;
        totalUsageMinutes++;
        if (usageTimeRemaining <= 0) {
            std::string finishedUser = currentUser;
            inUse = false;
            currentUser = "";
            std::cout << "  [" << type << "] sesiune terminata pentru "
                      << finishedUser << ".\n";
            return finishedUser;
        }
        return "";
    }

    friend std::ostream& operator<<(std::ostream& os, const Equipment& eq) {
        os << "Echipament: " << eq.type
           << " | Status: " << (eq.inUse ? "Ocupat" : "Liber")
           << " | Total: " << eq.totalUsageMinutes << " min";
        if (eq.inUse)
            os << " | Ramas: " << eq.usageTimeRemaining
               << " min | User: " << eq.currentUser;
        return os;
    }
};

class Gym {
    std::string name;
    std::vector<Equipment> equipments;
    std::vector<Member> members;

    int findMemberIndex(int id) const {
        for (int i = 0; i < (int)members.size(); i++)
            if (members[i].getID() == id) return i;
        return -1;
    }
public:
    explicit Gym(const std::string& name_) : name{name_} {}

    void incarcaMembri(const std::string& fisier,
                       const std::vector<MembershipPlan>& planuri) {
        std::ifstream fin(fisier);
        if (!fin) { std::cerr << "Eroare: Nu am gasit " << fisier << "\n"; return; }
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
            if (!gasit)
                std::cerr << "  Atentie: planul '" << tipPlan
                          << "' nu exista pentru " << numeMembru << "\n";
        }
        std::cout << "[Sistem] " << members.size() << " membri incarcati.\n";
    }

    void incarcaEchipamente(const std::string& fisier) {
        std::ifstream fin(fisier);
        if (!fin) { std::cerr << "Eroare: Nu am gasit " << fisier << "\n"; return; }
        std::string tip;
        while (fin >> tip)
            equipments.push_back(Equipment(tip));
        std::cout << "[Sistem] " << equipments.size() << " echipamente incarcate.\n";
    }

    void adaugaMembru(const Member& m) { members.push_back(m); }
    void adaugaEchipament(const Equipment& e) { equipments.push_back(e); }

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

    void updateSala() {
        for (int i = 0; i < (int)equipments.size(); i++) {
            std::string userActiv = equipments[i].getCurrentUser();
            equipments[i].update();
            if (!userActiv.empty()) {
                for (int j = 0; j < (int)members.size(); j++)
                    if (members[j].getName() == userActiv)
                        members[j].addMinutes(1);
            }
        }
    }

    void raportComplet() const {
        int echipOcupate = 0;
        for (int i = 0; i < (int)equipments.size(); i++)
            if (equipments[i].isInUse()) echipOcupate++;

        int membriActivi = 0;
        for (int i = 0; i < (int)members.size(); i++)
            if (members[i].getTotalMinutes() > 0) membriActivi++;

        std::cout << "\n====== RAPORT: " << name << " ======\n";
        std::cout << "Echipamente: " << equipments.size()
                  << " | Ocupate: " << echipOcupate
                  << " | Libere: " << (equipments.size() - echipOcupate) << "\n";
        std::cout << "Membri: " << members.size()
                  << " | Activi: " << membriActivi << "\n";

        if (!equipments.empty()) {
            int topEqIdx = 0;
            for (int i = 1; i < (int)equipments.size(); i++)
                if (equipments[i].getTotalUsage() > equipments[topEqIdx].getTotalUsage())
                    topEqIdx = i;
            std::cout << "Top echipament: " << equipments[topEqIdx].getType()
                      << " (" << equipments[topEqIdx].getTotalUsage() << " min)\n";
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
        std::cout << "\n==============================\n";
    }

    void afiseazaTopMembri() const {
        std::vector<int> ordine;
        for (int i = 0; i < (int)members.size(); i++)
            ordine.push_back(i);

        for (int i = 0; i < (int)ordine.size(); i++)
            for (int j = i + 1; j < (int)ordine.size(); j++)
                if (members[ordine[j]].getTotalMinutes() > members[ordine[i]].getTotalMinutes()) {
                    int tmp = ordine[i];
                    ordine[i] = ordine[j];
                    ordine[j] = tmp;
                }

        std::cout << "\n--- Top Membri ---\n";
        for (int i = 0; i < (int)ordine.size(); i++)
            std::cout << i + 1 << ". " << members[ordine[i]] << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Gym& g) {
        os << "\n=== Sala: " << g.name << " ===\n";
        os << "Echipamente (" << g.equipments.size() << "):\n";
        for (int i = 0; i < (int)g.equipments.size(); i++)
            os << "  [" << i << "] " << g.equipments[i] << "\n";
        os << "Membri (" << g.members.size() << "):\n";
        for (int i = 0; i < (int)g.members.size(); i++)
            os << "  " << g.members[i] << "\n";
        return os;
    }
};

// planurile exista independent de o sala, deci functie libera
std::vector<MembershipPlan> incarcaPlanuri(const std::string& fisier) {
    std::vector<MembershipPlan> planuri;
    std::ifstream fin(fisier);
    if (!fin) { std::cerr << "Eroare: Nu am gasit " << fisier << "\n"; return planuri; }
    std::string tip;
    float pret;
    int zile;
    while (fin >> tip >> pret >> zile)
        planuri.push_back(MembershipPlan(tip, pret, zile));
    std::cout << "[Sistem] " << planuri.size() << " planuri incarcate.\n";
    return planuri;
}

int main() {
    std::vector<MembershipPlan> planuri = incarcaPlanuri("plans.txt");

    Gym sala("Radu's Gym");
    sala.incarcaMembri("members.txt", planuri);
    sala.incarcaEchipamente("equipments.txt");

    std::cout<<sala;

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n------------\n"
                  << "1. Incepe sesiune\n"
                  << "2. Avanseaza 1 minut\n"
                  << "3. Afiseaza sala\n"
                  << "4. Raport complet\n"
                  << "5. Top membri\n"
                  << "6. Adauga echipament\n"
                  << "7. Adauga membru\n"
                  << "0. Iesire\n"
                  << "Optiune: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int eqIdx, memberID, durata;
            std::cout << "Index echipament: "; 
            std::cin >> eqIdx;
            std::cout << "ID membru: ";        
            std::cin >> memberID;
            std::cout << "Durata (min): ";     
            std::cin >> durata;
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
            std::cout << "Tip echipament: "; std::cin >> tip;
            sala.adaugaEchipament(Equipment(tip));
            std::cout << "Echipament adaugat.\n";
            break;
        }
        case 7: {
            std::string numeMembru, tipPlan;
            int id;
            std::cout << "Nume: ";     std::cin >> numeMembru;
            std::cout << "ID: ";       std::cin >> id;
            std::cout << "Tip plan: "; std::cin >> tipPlan;
            bool gasit = false;
            for (int i = 0; i < (int)planuri.size(); i++) {
                if (planuri[i].getPlanType() == tipPlan) {
                    sala.adaugaMembru(Member(numeMembru, id, planuri[i]));
                    std::cout << "Membru adaugat.\n";
                    gasit = true;
                    break;
                }
            }
            if (!gasit)
                std::cout << "Planul '" << tipPlan << "' nu exista.\n";
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
