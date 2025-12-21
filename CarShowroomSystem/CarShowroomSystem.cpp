#include <iostream>
#include <cstdlib> 
#include <limits> 
#include "Database.h"
#include "AdminManager.h"
#include "SellerManager.h"
#include "CustomerManager.h"

using namespace std;

void clearScreen() { system("cls"); }

int main() {
    Database db;
    int choice;

    while (true) {
        clearScreen();
        cout << "====================================\n";
        cout << "       CAR GRABBER SYSTEM           \n";
        cout << "====================================\n";
        cout << " 1. ADMIN \n";
        cout << " 2. SELLER LOGIN\n";
        cout << " 3. CUSTOMER LOGIN\n";
        cout << " 4. EXIT APP\n";
        cout << "====================================\n";
        cout << " Choice: ";

        if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

        if (choice == 1) { // ADMIN
            string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p;
            if (AdminManager::login(db, u, p) != -1) {
                while (true) {
                    clearScreen();
                    cout << "======= ADMIN GOD MODE =======\n";
                    cout << "1. Manage Showrooms\n";
                    cout << "2. Manage Garages\n";
                    cout << "3. View All Data\n";
                    cout << "4. Add New Admin\n";
                    cout << "5. Logout\nChoice: ";
                    int op; cin >> op;

                    if (op == 1) {
                        while (true) {
                            clearScreen();
                            cout << "\n-- ADMIN: SHOWROOM OPS --\n";
                            cout << "1. Create Showroom\n2. Delete Showroom\n3. Add Car (Global)\n4. Delete Car (Global)\n5. Edit Car Price (Global)\n6. BACK\nChoice: ";
                            int k; cin >> k;
                            if (k == 1) AdminManager::addShowroom(db);
                            else if (k == 2) AdminManager::deleteShowroom(db);
                            else if (k == 3) AdminManager::addCarGlobal(db);
                            else if (k == 4) AdminManager::deleteCarGlobal(db);
                            else if (k == 5) AdminManager::editCarPriceGlobal(db);
                            else break;
                            system("pause");
                        }
                    }
                    else if (op == 2) {
                        while (true) {
                            clearScreen();
                            cout << "\n-- ADMIN: GARAGE OPS --\n";
                            cout << "1. Create Garage\n2. Delete Garage\n3. Add Service (Global)\n4. Delete Service (Global)\n5. Edit Service Price (Global)\n6. BACK\nChoice: ";
                            int k; cin >> k;
                            if (k == 1) AdminManager::addGarage(db);
                            else if (k == 2) AdminManager::deleteGarage(db);
                            else if (k == 3) AdminManager::addServiceGlobal(db);
                            else if (k == 4) AdminManager::deleteServiceGlobal(db);
                            else if (k == 5) AdminManager::editServicePriceGlobal(db);
                            else break;
                            system("pause");
                        }
                    }
                    else if (op == 3) { AdminManager::viewAll(db); system("pause"); }
                    else if (op == 4) { AdminManager::addAdmin(db); system("pause"); }
                    else break;
                }
            }
            else { cout << "Wrong Credentials!\n"; system("pause"); }
        }

        else if (choice == 2) { // SELLER
            cout << "1. Login  2. Register: "; int op; cin >> op;
            if (op == 2) SellerManager::registerSeller(db);
            else {
                string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p;
                int sId = SellerManager::login(db, u, p);
                if (sId != -1) {
                    while (true) {
                        clearScreen();
                        cout << "======= SELLER DASHBOARD =======\n";
                        cout << "1. Manage Showroom (Cars)\n";
                        cout << "2. Manage Garage (Services)\n";
                        cout << "3. View My Business Summary\n";
                        cout << "4. History (Sold Cars / Services)\n";
                        cout << "5. Logout\nChoice: ";
                        int k; cin >> k;

                        if (k == 1) { // MANAGE SHOWROOM SUB-MENU
                            while (true) {
                                clearScreen();
                                SellerManager::showMyCars(db, sId);
                                cout << "\n-- SHOWROOM OPS --\n";
                                cout << "1. Create Showroom\n2. Add Car\n3. Delete Car\n4. Edit Car Prices\n5. DELETE ALL CARS\n6. DELETE SHOWROOM\n7. BACK\nChoice: ";
                                int x; cin >> x;
                                if (x == 1) SellerManager::createShowroom(db, sId);
                                else if (x == 2) SellerManager::addCar(db, sId);
                                else if (x == 3) SellerManager::deleteCar(db, sId);
                                else if (x == 4) SellerManager::editCarPrice(db, sId);
                                else if (x == 5) SellerManager::deleteAllCars(db, sId);
                                else if (x == 6) { SellerManager::deleteMyShowroom(db, sId); break; }
                                else break; // Back
                                system("pause");
                            }
                        }
                        else if (k == 2) { // MANAGE GARAGE SUB-MENU
                            while (true) {
                                clearScreen();
                                SellerManager::showMyServices(db, sId);
                                cout << "\n-- GARAGE OPS --\n";
                                cout << "1. Create Garage\n2. Add Service\n3. Delete Service\n4. Edit Service Price\n5. DELETE ALL SERVICES\n6. DELETE GARAGE\n7. BACK\nChoice: ";
                                int x; cin >> x;
                                if (x == 1) SellerManager::createGarage(db, sId);
                                else if (x == 2) SellerManager::addService(db, sId);
                                else if (x == 3) SellerManager::deleteService(db, sId);
                                else if (x == 4) SellerManager::editServicePrice(db, sId);
                                else if (x == 5) SellerManager::deleteAllServices(db, sId);
                                else if (x == 6) { SellerManager::deleteMyGarage(db, sId); break; }
                                else break; // Back
                                system("pause");
                            }
                        }
                        else if (k == 3) { SellerManager::viewMyBusiness(db, sId); system("pause"); }
                        else if (k == 4) {
                            cout << "\n-- HISTORY --\n";
                            cout << "1. Sold Cars Log (View/Delete)\n2. Services Provided\nChoice: ";
                            int x; cin >> x;
                            if (x == 1) SellerManager::viewSoldCars(db, sId);
                            else SellerManager::viewServicesProvided(db, sId);
                            system("pause");
                        }
                        else break;
                    }
                }
                else { cout << "Failed.\n"; system("pause"); }
            }
        }

        else if (choice == 3) { // CUSTOMER
            cout << "1. Login  2. Register: "; int op; cin >> op;
            if (op == 2) CustomerManager::registerCustomer(db);
            else {
                string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p;
                int cId = CustomerManager::login(db, u, p);
                if (cId != -1) {
                    while (true) {
                        clearScreen();
                        cout << "======= CUSTOMER MENU =======\n";
                        cout << "1. View Market\n";
                        cout << "2. Buy / Rent / Reserve Car\n";
                        cout << "3. Book Service Appointment\n";
                        cout << "4. My Reservations (Buy/Cancel)\n";
                        cout << "5. View My History\n";
                        cout << "6. Logout\nChoice: ";
                        int k; cin >> k;

                        if (k == 1) { CustomerManager::viewMarket(db); system("pause"); }
                        else if (k == 2) { CustomerManager::buyCarOrRent(db, cId); system("pause"); }
                        else if (k == 3) { CustomerManager::bookService(db, cId); system("pause"); }
                        else if (k == 4) {
                            CustomerManager::viewMyReservations(db, cId);
                            cout << "1. Buy Reserved Car  2. Cancel Reservation  3. Back: "; int z; cin >> z;
                            if (z == 1) CustomerManager::buyCarOrRent(db, cId);
                            else if (z == 2) CustomerManager::cancelReservation(db, cId);
                        }
                        else if (k == 5) { CustomerManager::viewHistory(db, cId); system("pause"); }
                        else break;
                    }
                }
                else { cout << "Failed.\n"; system("pause"); }
            }
        }
        else break;
    }
    return 0;
}