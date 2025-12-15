#pragma once
#include "Database.h" 

class AdminManager {
public:
    static int login(Database& db, string user, string pass) {
        for (auto& a : db.admins) if (a.username == user && a.password == pass) return a.id;
        return -1;
    }

    static void addAdmin(Database& db) {
        Admin a; a.id = db.admins.size() + 1;
        cout << "New User: "; cin >> a.username; cout << "New Pass: "; cin >> a.password;
        db.admins.push_back(a);
    }

    // --- BASIC ADD FUNCTIONS ---
    static void addShowroom(Database& db) {
        Showroom s; s.id = db.showrooms.size() + 1;
        cout << "Name: "; cin.ignore(); getline(cin, s.name);
        cout << "Location: "; getline(cin, s.location); cout << "Phone: "; cin >> s.phone;
        db.showrooms.push_back(s);
    }
    static void addGarage(Database& db) {
        Garage g; g.id = db.garages.size() + 1;
        cout << "Name: "; cin.ignore(); getline(cin, g.name);
        cout << "Location: "; getline(cin, g.location); cout << "Phone: "; cin >> g.phone;
        db.garages.push_back(g);
    }
    // ... (Keep addCar/Service if you want admin to add them too, handled in prev versions)

    // --- DELETE FUNCTIONS (GOD MODE) ---
    static void deleteShowroom(Database& db) {
        int id; cout << "Delete Showroom ID: "; cin >> id;
        for (auto it = db.showrooms.begin(); it != db.showrooms.end(); ++it) {
            if (it->id == id) {
                // Unlink any seller who owned this
                for (auto& s : db.sellers) if (s.myShowroomID == id) s.myShowroomID = -1;
                db.showrooms.erase(it);
                cout << "Deleted.\n"; return;
            }
        }
        cout << "Not Found.\n";
    }

    static void deleteGarage(Database& db) {
        int id; cout << "Delete Garage ID: "; cin >> id;
        for (auto it = db.garages.begin(); it != db.garages.end(); ++it) {
            if (it->id == id) {
                for (auto& s : db.sellers) if (s.myGarageID == id) s.myGarageID = -1;
                db.garages.erase(it);
                cout << "Deleted.\n"; return;
            }
        }
        cout << "Not Found.\n";
    }

    static void deleteServiceGlobal(Database& db) {
        int gId, sId;
        cout << "Garage ID: "; cin >> gId; cout << "Service ID: "; cin >> sId;
        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (auto it = g.services.begin(); it != g.services.end(); ++it) {
                    if (it->id == sId) { g.services.erase(it); cout << "Deleted.\n"; return; }
                }
            }
        }
        cout << "Not Found.\n";
    }

    // --- VIEW FUNCTIONS ---
    static void viewAppointmentsGlobal(Database& db) {
        int gId; cout << "Enter Garage ID to view appointments: "; cin >> gId;
        cout << "\n--- APPOINTMENTS FOR GARAGE " << gId << " ---\n";
        bool found = false;
        for (const auto& c : db.customers) {
            for (const auto& h : c.serviceHistory) {
                if (h.garageID == gId) {
                    cout << "Cust: " << c.username << " | Day: " << h.dayBooked << " | Date: " << h.date << endl;
                    found = true;
                }
            }
        }
        if (!found) cout << "No appointments.\n";
    }

    static void viewAll(Database& db) {
        cout << "\n=== ADMIN VIEW ===\n";
        for (auto& r : db.showrooms) {
            cout << "SR ID: " << r.id << " | " << r.name << endl;
            for (auto& c : r.cars) cout << "  > Car ID " << c.id << ": " << c.make << " ($" << c.price << ")" << (c.status == 2 ? " [SOLD]" : "") << endl;
        }
        for (auto& g : db.garages) {
            cout << "GR ID: " << g.id << " | " << g.name << endl;
            for (auto& s : g.services) cout << "  > Srv ID " << s.id << ": " << s.name << endl;
        }
    }

    static void changePassword(Database& db, int id) {
        for (auto& a : db.admins) {
            if (a.id == id) {
                string o, n; cout << "Old: "; cin >> o; cout << "New: "; cin >> n;
                if (a.password == o) { a.password = n; cout << "Success.\n"; }
                else cout << "Wrong.\n";
            }
        }
    }
};