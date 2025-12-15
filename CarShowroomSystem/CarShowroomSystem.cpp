#include <iostream>
#include "Database.h"
#include "AdminManager.h"
#include "SellerManager.h"
#include "CustomerManager.h"

using namespace std;

int main() {
    Database db;
    int choice;

    while (true) {
        cout << "\n====================================\n";
        cout << "1. ADMIN | 2. SELLER | 3. CUSTOMER | 4. SAVE & EXIT\n";
        cout << "Choice: "; cin >> choice;

        // --- ADMIN ---
        if (choice == 1) {
            string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p;
            int aId = AdminManager::login(db, u, p);
            if (aId != -1) {
                while (true) {
                    cout << "\n[ADMIN] 1.Add 2.Delete 3.View Appointments 4.View All Data 5.Back\nChoice: ";
                    int op; cin >> op;
                    if (op == 1) {
                        cout << "1.Showroom 2.Garage: "; int t; cin >> t;
                        if (t == 1) AdminManager::addShowroom(db); else AdminManager::addGarage(db);
                    }
                    else if (op == 2) {
                        cout << "1.Showroom 2.Garage 3.Service: "; int t; cin >> t;
                        if (t == 1) AdminManager::deleteShowroom(db);
                        else if (t == 2) AdminManager::deleteGarage(db);
                        else AdminManager::deleteServiceGlobal(db);
                    }
                    else if (op == 3) AdminManager::viewAppointmentsGlobal(db);
                    else if (op == 4) AdminManager::viewAll(db);
                    else if (op == 5) break;
                }
            }
            else cout << "Failed.\n";
        }

        // --- SELLER ---
        else if (choice == 2) {
            cout << "1. Login 2. Register: "; int op; cin >> op;
            if (op == 2) SellerManager::registerSeller(db);
            else {
                string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p;
                int sId = SellerManager::login(db, u, p);
                if (sId != -1) {
                    while (true) {
                        cout << "\n[SELLER] \n1. My Showroom (Add/Del Car)\n2. My Garage (Add/Del Srv)\n3. View My Inventory/Services\n4. Sold Cars\n5. View Appointments\n6. Back\nChoice: ";
                        int sop; cin >> sop;

                        if (sop == 1) {
                            cout << "1. Create 2. Add Car 3. Del Car 4. DELETE SHOWROOM: "; int k; cin >> k;
                            if (k == 1) SellerManager::createShowroom(db, sId);
                            else if (k == 2) SellerManager::addCar(db, sId);
                            else if (k == 3) SellerManager::deleteCar(db, sId);
                            else if (k == 4) SellerManager::deleteMyShowroom(db, sId);
                        }
                        else if (sop == 2) {
                            cout << "1. Create 2. Add Srv 3. Del Srv 4. DELETE GARAGE: "; int k; cin >> k;
                            if (k == 1) SellerManager::createGarage(db, sId);
                            else if (k == 2) SellerManager::addService(db, sId);
                            else if (k == 3) SellerManager::deleteService(db, sId);
                            else if (k == 4) SellerManager::deleteMyGarage(db, sId);
                        }
                        else if (sop == 3) SellerManager::viewMyBusiness(db, sId);
                        else if (sop == 4) SellerManager::manageSoldCars(db, sId);
                        else if (sop == 5) SellerManager::viewAppointments(db, sId);
                        else if (sop == 6) break;
                    }
                }
                else cout << "Failed.\n";
            }
        }

        // --- CUSTOMER ---
        else if (choice == 3) {
            cout << "1. Login 2. Register: "; int op; cin >> op;
            if (op == 2) CustomerManager::registerCustomer(db);
            else {
                string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p;
                int cId = CustomerManager::login(db, u, p);
                if (cId != -1) {
                    while (true) {
                        cout << "\n[CUSTOMER MENU]\n";
                        cout << "1. View Entire Market (Cars & Services)\n";
                        cout << "2. Search Specific Car\n";
                        cout << "3. Buy / Rent / Installment (With Confirm)\n";
                        cout << "4. Search Service\n";
                        cout << "5. Book Service\n";
                        cout << "6. View History\n";
                        cout << "7. Logout\nChoice: ";
                        int cop; cin >> cop;

                        if (cop == 1) CustomerManager::viewMarket(db);
                        else if (cop == 2) CustomerManager::searchCars(db);
                        else if (cop == 3) CustomerManager::buyCarOrRent(db, cId);
                        else if (cop == 4) CustomerManager::searchServices(db);
                        else if (cop == 5) CustomerManager::bookService(db, cId);
                        else if (cop == 6) CustomerManager::viewHistory(db, cId);
                        else if (cop == 7) break;
                    }
                }
                else cout << "Failed.\n";
            }
        }

        else if (choice == 4) { db.saveAll(); break; }
    }
    return 0;
}