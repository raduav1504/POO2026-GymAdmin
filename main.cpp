#include <iostream>
#include <vector>
#include "include/Gym.h"
#include "include/GymExceptions.h"

int main() {
    std::vector<MembershipPlan> planuri = incarcaPlanuri("plans.txt");

    Gym sala("FitZone");
    try {
        sala.incarcaMembri("members.txt", planuri);
    } catch (const PlanInvalidException& e) {
        std::cerr << "Eroare date: " << e.what() << "\n";
        return 1;
    }
    sala.incarcaEchipamente("equipments.txt");
    sala.incarcaServicii("services.txt");

    std::cout << sala;

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n------------\n"
                  << "1.  Incepe sesiune echipament\n"
                  << "2.  Avanseaza 1 minut\n"
                  << "3.  Afiseaza sala\n"
                  << "4.  Raport complet\n"
                  << "5.  Top membri\n"
                  << "6.  Adauga echipament\n"
                  << "7.  Adauga membru\n"
                  << "8.  Acceseaza serviciu\n"
                  << "9.  Afiseaza servicii\n"
                  << "10. Raport servicii\n"
                  << "0.  Iesire\n"
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
            try {
                sala.startSesiune(eqIdx, memberID, durata);
            } catch (const MembruOcupatException& e) {
                std::cout << "EROARE: " << e.what() << "\n";
            } catch (const MembruNegasitException& e) {
                std::cout << "EROARE: " << e.what() << "\n";
            } catch (const AccesInterzisException& e) {
                std::cout << "ACCES REFUZAT: " << e.what() << "\n";
            }
            break;
        }
        case 2:
            int nMinute;
            std::cout<<" Cate minute sa avansez: ";
            std::cin>> nMinute;
            for (int i = 0; i<=nMinute; i ++)
                sala.updateSala();
            std::cout << "Avansat" <<  nMinute<<" minute.\n";
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
            std::string tip, serviciu;
            std::cout << "Tip echipament: "; 
            std::cin >> tip;
            std::cout << "Serviciu (CardioZone/none/...): "; 
            std::cin >> serviciu;
            sala.adaugaEchipament(Equipment(tip, serviciu));
            std::cout << "Echipament adaugat.\n";
            break;
        }
        case 7: {
            std::string numeMembru, tipPlan;
            int id;
            std::cout << "Nume: "; 
            std::cin >> numeMembru;
            std::cout << "ID: ";       
            std::cin >> id;
            std::cout << "Tip plan: ";
            std::cin >> tipPlan;
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
        case 8: {
            std::string numeServiciu;
            int memberID;
            std::cout << "Nume serviciu (ex: Yoga, CardioZone, PersonalTraining-Radu): ";
            std::cin >> numeServiciu;
            std::cout << "ID membru: "; std::cin >> memberID;
            try {
                sala.acceseazaServiciu(numeServiciu, memberID);
            } catch (const AccesInterzisException& e) {
                std::cout << "ACCES REFUZAT: " << e.what() << "\n";
            } catch (const MembruOcupatException& e) {
                std::cout << "EROARE: " << e.what() << "\n";
            } catch (const CapacitatePlinaException& e) {
                std::cout << "EROARE: " << e.what() << "\n";
            } catch (const MembruNegasitException& e) {
                std::cout << "EROARE: " << e.what() << "\n";
            }
            break;
        }
        case 9:
            sala.afiseazaServicii();
            break;
        case 10:
            sala.raportServicii();
            break;
        case 0:
            std::cout << "La revedere!\n";
            break;
        default:
            std::cout << "Optiune invalida.\n";
        }
    }
    return 0;
}
