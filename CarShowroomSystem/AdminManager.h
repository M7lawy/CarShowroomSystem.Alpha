#pragma once
#include "Database.h"
#include <iostream>
#include <string>

using namespace std;

class AdminManager {
public:
    static int login(Database& db, string user, string pass) {
        for (auto& a : db.admins) if (a.username == user && a.password == pass) return a.id;
        return -1;
    }

    static void addAdmin(Database& db) {
        int maxId = 0; for (const auto& a : db.admins) if (a.id > maxId) maxId = a.id;
        Admin a; a.id = maxId + 1;
        cout << "New User: "; cin >> a.username; 
        cout << "New Pass: "; cin >> a.password;
        db.admins.push_back(a); 
        db.saveAll();
        cout << "Admin Added.\n";
    }

    static void listShowrooms(Database& db) {
        cout << "\n--- CURRENT SHOWROOMS ---\n";
        for (auto& r : db.showrooms) {
            cout << "ID: " << r.id << " | Name: " << r.name << " | Loc: " << r.location << endl;
            if (!r.cars.empty()) {
                for (auto& c : r.cars) {
                    string st = (c.status == 2) ? "SOLD" : (c.status == 1 ? "RESERVED" : "AVAILABLE");
                    cout << "   -> Car ID: " << c.id << " | " << c.year << " " << c.make << " " << c.model << " | $" << c.price << " | Rent: $" << c.rentPrice << " | " << st << endl;
                }
            }
        }
        cout << "-------------------------\n";
    }

    static void listGarages(Database& db) {
        cout << "\n--- CURRENT GARAGES ---\n";
        for (auto& g : db.garages) {
            cout << "ID: " << g.id << " | Name: " << g.name << " | Loc: " << g.location << endl;
            if (!g.services.empty()) {
                for (auto& s : g.services)
                    cout << "   -> Srv ID: " << s.id << " | " << s.name << " ($" << s.price << ")" << endl;
            }
        }
        cout << "-----------------------\n";
    }

    
    static void addShowroom(Database& db) {
        int maxId = 0; for (const auto& x : db.showrooms) if (x.id > maxId) maxId = x.id;
        Showroom s; s.id = maxId + 1;
        cout << "Name: "; cin.ignore(); getline(cin, s.name);
        cout << "Location: "; getline(cin, s.location); cout << "Phone: "; cin >> s.phone;
        db.showrooms.push_back(s); db.saveAll(); cout << "Showroom Added.\n";
    }

    static void deleteShowroom(Database& db) {
        listShowrooms(db);
        int id; cout << "Enter Showroom ID to DELETE: "; cin >> id;
        for (auto it = db.showrooms.begin(); it != db.showrooms.end(); ++it) {
            if (it->id == id) {
                for (auto& s : db.sellers) if (s.myShowroomID == id) s.myShowroomID = -1;
                db.showrooms.erase(it); db.saveAll(); cout << "Deleted.\n"; return;
            }
        }
        cout << "Not Found.\n";
    }

    static void addCarGlobal(Database& db) {
        listShowrooms(db);
        int sId; cout << "Enter Showroom ID to add car to: "; cin >> sId;
        for (auto& r : db.showrooms) {
            if (r.id == sId) {
                int maxId = 0; for (const auto& c : r.cars) if (c.id > maxId) maxId = c.id;
                int cid = maxId + 1;
                string mk, md; double p, rp; int y;
                cout << "Make: "; cin >> mk; cout << "Model: "; cin >> md;
                cout << "Year: "; cin >> y; cout << "Price: "; cin >> p; cout << "Rent Price: "; cin >> rp;
                r.cars.push_back(Car(cid, mk, md, y, p, rp));
                db.saveAll(); cout << "Car Added.\n"; return;
            }
        }
        cout << "Showroom not found.\n";
    }

    static void deleteCarGlobal(Database& db) {
        listShowrooms(db);
        int sId, cId; cout << "Showroom ID: "; cin >> sId; cout << "Car ID: "; cin >> cId;
        for (auto& r : db.showrooms) {
            if (r.id == sId) {
                for (auto it = r.cars.begin(); it != r.cars.end(); ++it) {
                    if (it->id == cId) { r.cars.erase(it); db.saveAll(); cout << "Car Deleted.\n"; return; }
                }
            }
        }
        cout << "Not Found.\n";
    }

    static void editCarPriceGlobal(Database& db) {
        listShowrooms(db);
        int sId, cId; cout << "Showroom ID: "; cin >> sId; cout << "Car ID: "; cin >> cId;
        for (auto& r : db.showrooms) {
            if (r.id == sId) {
                for (auto& c : r.cars) {
                    if (c.id == cId) {
                        cout << "Editing: " << c.year << " " << c.make << " " << c.model << endl;
                        cout << "Current Price: $" << c.price << ". New Price: "; cin >> c.price;
                        cout << "Current Rent: $" << c.rentPrice << ". New Rent: "; cin >> c.rentPrice;
                        db.saveAll(); cout << "Prices Updated.\n"; return;
                    }
                }
            }
        }
        cout << "Not Found.\n";
    }

    static void addGarage(Database& db) {
        int maxId = 0; for (const auto& x : db.garages) if (x.id > maxId) maxId = x.id;
        Garage g; g.id = maxId + 1;
        cout << "Name: "; cin.ignore(); getline(cin, g.name);
        cout << "Location: "; getline(cin, g.location); cout << "Phone: "; cin >> g.phone;
        db.garages.push_back(g); db.saveAll(); cout << "Garage Added.\n";
    }

    static void deleteGarage(Database& db) {
        listGarages(db);
        int id; cout << "Enter Garage ID to DELETE: "; cin >> id;
        for (auto it = db.garages.begin(); it != db.garages.end(); ++it) {
            if (it->id == id) {
                for (auto& s : db.sellers) if (s.myGarageID == id) s.myGarageID = -1;
                db.garages.erase(it); db.saveAll(); cout << "Deleted.\n"; return;
            }
        }
        cout << "Not Found.\n";
    }

    static void addServiceGlobal(Database& db) {
        listGarages(db);
        int gId; cout << "Enter Garage ID to add service to: "; cin >> gId;
        for (auto& g : db.garages) {
            if (g.id == gId) {
                string n; double p;
                cout << "Service Name: "; cin.ignore(); getline(cin, n); cout << "Price: "; cin >> p;
                int maxId = 0; for (const auto& s : g.services) if (s.id > maxId) maxId = s.id;
                int sId = maxId + 1;
                g.services.push_back(Service(sId, n, p));
                db.saveAll(); cout << "Service Added.\n"; return;
            }
        }
        cout << "Garage not found.\n";
    }

    static void deleteServiceGlobal(Database& db) {
        listGarages(db);
        int gId, sId; cout << "Garage ID: "; cin >> gId; cout << "Service ID: "; cin >> sId;
        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (auto it = g.services.begin(); it != g.services.end(); ++it) {
                    if (it->id == sId) { g.services.erase(it); db.saveAll(); cout << "Deleted.\n"; return; }
                }
            }
        }
        cout << "Not Found.\n";
    }

    static void editServicePriceGlobal(Database& db) {
        listGarages(db);
        int gId, sId; cout << "Garage ID: "; cin >> gId; cout << "Service ID: "; cin >> sId;
        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (auto& s : g.services) {
                    if (s.id == sId) {
                        cout << "Editing: " << s.name << endl;
                        cout << "Current Price: $" << s.price << ". New Price: "; cin >> s.price;
                        db.saveAll(); cout << "Updated.\n"; return;
                    }
                }
            }
        }
        cout << "Not Found.\n";
    }

    static void viewAll(Database& db) {
        listShowrooms(db);
        listGarages(db);
    }
};