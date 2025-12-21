#pragma once
#include "Database.h"
#include <iostream>
#include <string>

using namespace std;

class SellerManager {
public:
    static int login(Database& db, string user, string pass) {
        for (auto& s : db.sellers) if (s.username == user && s.password == pass) return s.id;
        return -1;
    }

    static void registerSeller(Database& db) {
        int maxId = 0; for (const auto& x : db.sellers) if (x.id > maxId) maxId = x.id;
        Seller s; s.id = maxId + 1;
        cout << "User: "; cin >> s.username; cout << "Pass: "; cin >> s.password;
        db.sellers.push_back(s); db.saveAll();
        cout << "Registered! ID: " << s.id << endl;
    }

    // --- HELPERS ---
    static void showMyCars(Database& db, int sellerID) {
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) { cout << "No Showroom.\n"; return; }
        cout << "\n--- MY CARS ---\n";
        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                if (r.cars.empty()) cout << "No cars.\n";
                for (auto& c : r.cars) {
                    string st = (c.status == 2) ? "SOLD" : (c.status == 1 ? "RESERVED" : "AVAILABLE");
                    cout << "ID: " << c.id << " | " << c.year << " " << c.make << " " << c.model
                        << " | $" << c.price << " | Rent: $" << c.rentPrice << " | " << st << endl;
                }
            }
        }
        cout << "---------------\n";
    }

    static void showMyServices(Database& db, int sellerID) {
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) { cout << "No Garage.\n"; return; }
        cout << "\n--- MY SERVICES ---\n";
        for (auto& g : db.garages) {
            if (g.id == gId) {
                if (g.services.empty()) cout << "No services.\n";
                for (auto& s : g.services) cout << "ID: " << s.id << " | " << s.name << " | $" << s.price << endl;
            }
        }
        cout << "-------------------\n";
    }

    // --- SHOWROOM MANAGEMENT ---
    static void createShowroom(Database& db, int sellerID) {
        for (auto& s : db.sellers) {
            if (s.id == sellerID) {
                if (s.myShowroomID != -1) { cout << "Already have a showroom!\n"; return; }
                int maxId = 0; for (const auto& x : db.showrooms) if (x.id > maxId) maxId = x.id;
                Showroom r; r.id = maxId + 1;
                cout << "Name: "; cin.ignore(); getline(cin, r.name);
                cout << "Location: "; getline(cin, r.location); cout << "Phone: "; cin >> r.phone;
                db.showrooms.push_back(r); s.myShowroomID = r.id; db.saveAll();
                cout << "Created.\n";
            }
        }
    }

    static void addCar(Database& db, int sellerID) {
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) { cout << "Create showroom first.\n"; return; }
        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                int maxId = 0; for (const auto& c : r.cars) if (c.id > maxId) maxId = c.id;
                int cid = maxId + 1;
                string mk, md; double p, rp; int y;
                cout << "Make: "; cin >> mk; cout << "Model: "; cin >> md;
                cout << "Year: "; cin >> y; cout << "Price: "; cin >> p; cout << "Rent Price: "; cin >> rp;
                r.cars.push_back(Car(cid, mk, md, y, p, rp)); db.saveAll();
                cout << "Car Added.\n";
            }
        }
    }

    static void deleteCar(Database& db, int sellerID) {
        showMyCars(db, sellerID);
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) return;
        int cId; cout << "Car ID to delete: "; cin >> cId;
        for (auto& r : db.showrooms) if (r.id == rId) {
            for (auto it = r.cars.begin(); it != r.cars.end(); ++it) {
                if (it->id == cId) { r.cars.erase(it); db.saveAll(); cout << "Deleted.\n"; return; }
            }
        }
        cout << "Car not found.\n";
    }

    
    static void deleteAllCars(Database& db, int sellerID) {
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) return;

        cout << "Are you sure you want to delete ALL cars? (1.Yes 0.No): "; int c; cin >> c;
        if (c != 1) return;

        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                r.cars.clear(); db.saveAll(); cout << "All Cars Deleted.\n"; return;
            }
        }
    }

    // ** DELETE SHOWROOM **
    static void deleteMyShowroom(Database& db, int sellerID) {
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) return;

        cout << "Delete ENTIRE Showroom? This cannot be undone. (1.Yes 0.No): "; int c; cin >> c;
        if (c != 1) return;

        for (auto it = db.showrooms.begin(); it != db.showrooms.end(); ++it) {
            if (it->id == rId) {
                db.showrooms.erase(it);
                for (auto& s : db.sellers) if (s.id == sellerID) s.myShowroomID = -1;
                db.saveAll(); cout << "Showroom Deleted.\n"; return;
            }
        }
    }

    static void editCarPrice(Database& db, int sellerID) {
        showMyCars(db, sellerID);
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) return;
        int cId; cout << "Enter Car ID to edit: "; cin >> cId;
        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                for (auto& c : r.cars) {
                    if (c.id == cId) {
                        cout << "Current Price: $" << c.price << ". New Price: "; cin >> c.price;
                        cout << "Current Rent: $" << c.rentPrice << ". New Rent: "; cin >> c.rentPrice;
                        db.saveAll(); cout << "Prices Updated.\n"; return;
                    }
                }
            }
        }
        cout << "Car not found.\n";
    }

    // --- GARAGE MANAGEMENT ---
    static void createGarage(Database& db, int sellerID) {
        for (auto& s : db.sellers) {
            if (s.id == sellerID) {
                if (s.myGarageID != -1) { cout << "Already have a garage.\n"; return; }
                int maxId = 0; for (const auto& x : db.garages) if (x.id > maxId) maxId = x.id;
                Garage g; g.id = maxId + 1;
                cout << "Name: "; cin.ignore(); getline(cin, g.name);
                cout << "Location: "; getline(cin, g.location); cout << "Phone: "; cin >> g.phone;
                db.garages.push_back(g); s.myGarageID = g.id; db.saveAll();
                cout << "Garage Created.\n";
            }
        }
    }

    static void addService(Database& db, int sellerID) {
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) { cout << "Create garage first.\n"; return; }
        for (auto& g : db.garages) {
            if (g.id == gId) {
                string n; double p; cout << "Service Name: "; cin.ignore(); getline(cin, n); cout << "Price: "; cin >> p;
                int maxId = 0; for (const auto& s : g.services) if (s.id > maxId) maxId = s.id;
                int sId = maxId + 1;
                g.services.push_back(Service(sId, n, p)); db.saveAll();
                cout << "Service Added!\n";
            }
        }
    }

    static void deleteService(Database& db, int sellerID) {
        showMyServices(db, sellerID);
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) return;
        int sId; cout << "Service ID to delete: "; cin >> sId;
        for (auto& g : db.garages) if (g.id == gId) {
            for (auto it = g.services.begin(); it != g.services.end(); ++it) {
                if (it->id == sId) { g.services.erase(it); db.saveAll(); cout << "Deleted.\n"; return; }
            }
        }
        cout << "Service not found.\n";
    }

    // ** NEW: DELETE ALL SERVICES **
    static void deleteAllServices(Database& db, int sellerID) {
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) return;

        cout << "Delete ALL Services? (1.Yes 0.No): "; int c; cin >> c;
        if (c != 1) return;

        for (auto& g : db.garages) {
            if (g.id == gId) { g.services.clear(); db.saveAll(); cout << "All Services Deleted.\n"; return; }
        }
    }

    // ** NEW: DELETE GARAGE **
    static void deleteMyGarage(Database& db, int sellerID) {
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) return;

        cout << "Delete ENTIRE Garage? (1.Yes 0.No): "; int c; cin >> c;
        if (c != 1) return;

        for (auto it = db.garages.begin(); it != db.garages.end(); ++it) {
            if (it->id == gId) {
                db.garages.erase(it);
                for (auto& s : db.sellers) if (s.id == sellerID) s.myGarageID = -1;
                db.saveAll(); cout << "Garage Deleted.\n"; return;
            }
        }
    }

    static void editServicePrice(Database& db, int sellerID) {
        showMyServices(db, sellerID);
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) return;
        int sId; cout << "Enter Service ID: "; cin >> sId;
        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (auto& s : g.services) {
                    if (s.id == sId) {
                        cout << "Current: " << s.price << ". New Price: "; cin >> s.price;
                        db.saveAll(); cout << "Updated.\n"; return;
                    }
                }
            }
        }
        cout << "Service not found.\n";
    }

    static void viewMyBusiness(Database& db, int sellerID) {
        showMyCars(db, sellerID);
        showMyServices(db, sellerID);
    }

    static void viewSoldCars(Database& db, int sellerID) {
        int rId = -1; for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) { cout << "No showroom.\n"; return; }

        cout << "\n--- SOLD CARS HISTORY ---\n";
        bool found = false;
        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                for (const auto& c : r.cars) {
                    if (c.status == 2) {
                        cout << "ID: " << c.id << " | " << c.year << " " << c.make << " " << c.model << " | Sold For: $" << c.price << endl;
                        found = true;
                    }
                }
                if (found) {
                    cout << "\n1. Delete a Sold Record  2. Back\nChoice: ";
                    int ch; cin >> ch;
                    if (ch == 1) {
                        int cId; cout << "Enter Car ID to delete record: "; cin >> cId;
                        for (auto it = r.cars.begin(); it != r.cars.end(); ++it) {
                            if (it->id == cId && it->status == 2) {
                                r.cars.erase(it); db.saveAll(); cout << "Record Deleted.\n"; return;
                            }
                        }
                        cout << "Car ID not found in sold list.\n";
                    }
                }
                else cout << "No sold cars yet.\n";
            }
        }
    }

    static void viewServicesProvided(Database& db, int sellerID) {
        int gId = -1; for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) { cout << "No garage.\n"; return; }

        cout << "\n--- SERVICES PROVIDED HISTORY ---\n";
        bool found = false;
        for (const auto& c : db.customers) {
            for (const auto& h : c.serviceHistory) {
                if (h.garageID == gId) {
                    string sName = "Unknown";
                    for (auto& g : db.garages) if (g.id == gId) for (auto& s : g.services) if (s.id == h.serviceID) sName = s.name;
                    cout << "Cust: " << c.username << " | Srv: " << sName << " | Date: " << h.date << " | $" << h.cost << endl;
                    found = true;
                }
            }
        }
        if (!found) cout << "No services performed yet.\n";
    }
};