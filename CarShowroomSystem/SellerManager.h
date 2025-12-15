#pragma once
#include "Database.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class SellerManager {
public:
    static int login(Database& db, string user, string pass) {
        for (auto& s : db.sellers) if (s.username == user && s.password == pass) return s.id;
        return -1;
    }

    static void registerSeller(Database& db) {
        Seller s; s.id = db.sellers.size() + 1;
        cout << "User: "; cin >> s.username; cout << "Pass: "; cin >> s.password;
        db.sellers.push_back(s);
        cout << "Registered! ID: " << s.id << endl;
    }

    static void changePassword(Database& db, int id) {
        for (auto& s : db.sellers) {
            if (s.id == id) {
                string o, n; cout << "Old Pass: "; cin >> o; cout << "New Pass: "; cin >> n;
                if (s.password == o) { s.password = n; cout << "Success.\n"; }
                else cout << "Wrong old password.\n";
            }
        }
    }

    // ==========================================
    //           SHOWROOM MANAGEMENT
    // ==========================================

    static void createShowroom(Database& db, int sellerID) {
        for (auto& s : db.sellers) {
            if (s.id == sellerID) {
                if (s.myShowroomID != -1) { cout << "You already have a showroom!\n"; return; }
                Showroom r; r.id = db.showrooms.size() + 1;
                cout << "Name: "; cin.ignore(); getline(cin, r.name);
                cout << "Location: "; getline(cin, r.location); cout << "Phone: "; cin >> r.phone;
                db.showrooms.push_back(r);
                s.myShowroomID = r.id;
                cout << "Created.\n"; return;
            }
        }
    }

    static void deleteMyShowroom(Database& db, int sellerID) {
        for (auto& s : db.sellers) {
            if (s.id == sellerID) {
                if (s.myShowroomID == -1) { cout << "No showroom to delete.\n"; return; }

                cout << "Are you sure you want to delete your ENTIRE Showroom? (1.Yes 0.No): ";
                int c; cin >> c;
                if (c != 1) return;

                for (auto it = db.showrooms.begin(); it != db.showrooms.end(); ++it) {
                    if (it->id == s.myShowroomID) {
                        db.showrooms.erase(it);
                        s.myShowroomID = -1;
                        cout << "Showroom deleted successfully.\n"; return;
                    }
                }
            }
        }
    }

    static void addCar(Database& db, int sellerID) {
        int rId = -1;
        for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) { cout << "Create showroom first.\n"; return; }

        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                int cid = r.cars.size() + 1;
                string mk, md; double p, rp; int y;
                cout << "Make: "; cin >> mk; cout << "Model: "; cin >> md;
                cout << "Year: "; cin >> y; cout << "Price: "; cin >> p; cout << "Rent Price: "; cin >> rp;
                r.cars.push_back(Car(cid, mk, md, y, p, rp));
                cout << "Car Added.\n"; return;
            }
        }
    }

    // --- UPDATED: DELETE CAR (With Confirmation) ---
    static void deleteCar(Database& db, int sellerID) {
        int rId = -1;
        for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) { cout << "No showroom.\n"; return; }

        int cId; cout << "Enter Car ID to remove: "; cin >> cId;

        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                for (auto it = r.cars.begin(); it != r.cars.end(); ++it) {
                    if (it->id == cId) {
                        // Show Car Details
                        cout << "\n--- FOUND CAR ---\n";
                        cout << it->make << " " << it->model << " (" << it->year << ")\n";
                        cout << "Price: $" << it->price << endl;
                        cout << "-----------------\n";
                        cout << "Are you sure you want to delete this car? (1. Yes / 0. No): ";

                        int conf; cin >> conf;
                        if (conf == 1) {
                            r.cars.erase(it);
                            cout << "Car Deleted Successfully.\n";
                        }
                        else {
                            cout << "Deletion Cancelled.\n";
                        }
                        return;
                    }
                }
            }
        }
        cout << "Car ID Not found.\n";
    }

    static void manageSoldCars(Database& db, int sellerID) {
        int rId = -1;
        for (auto& s : db.sellers) if (s.id == sellerID) rId = s.myShowroomID;
        if (rId == -1) { cout << "No showroom.\n"; return; }

        for (auto& r : db.showrooms) {
            if (r.id == rId) {
                cout << "\n--- SOLD CARS ---\n";
                bool found = false;
                for (const auto& c : r.cars) {
                    if (c.status == 2) {
                        cout << "ID: " << c.id << " | " << c.make << " " << c.model << " | $" << c.price << endl;
                        found = true;
                    }
                }
                if (!found) { cout << "No sold cars history.\n"; return; }

                cout << "\n1. Delete a Sold Car Log\n2. Back\nChoice: ";
                int ch; cin >> ch;
                if (ch == 1) {
                    int cId; cout << "Enter Car ID to delete: "; cin >> cId;
                    // Note: Sold cars are usually kept for accounting, but if you want to delete the log:
                    for (auto it = r.cars.begin(); it != r.cars.end(); ++it) {
                        if (it->id == cId && it->status == 2) {
                            r.cars.erase(it);
                            cout << "Log deleted.\n"; return;
                        }
                    }
                    cout << "ID not found or car not sold.\n";
                }
            }
        }
    }

    // ==========================================
    //             GARAGE MANAGEMENT
    // ==========================================

    static void createGarage(Database& db, int sellerID) {
        for (auto& s : db.sellers) {
            if (s.id == sellerID) {
                if (s.myGarageID != -1) { cout << "Already have a garage.\n"; return; }
                Garage g; g.id = db.garages.size() + 1;
                cout << "Name: "; cin.ignore(); getline(cin, g.name);
                cout << "Location: "; getline(cin, g.location); cout << "Phone: "; cin >> g.phone;
                db.garages.push_back(g);
                s.myGarageID = g.id;
                cout << "Garage Created.\n"; return;
            }
        }
    }

    static void deleteMyGarage(Database& db, int sellerID) {
        for (auto& s : db.sellers) {
            if (s.id == sellerID) {
                if (s.myGarageID == -1) { cout << "No garage to delete.\n"; return; }

                cout << "Are you sure you want to delete your ENTIRE Garage? (1.Yes 0.No): ";
                int c; cin >> c;
                if (c != 1) return;

                for (auto it = db.garages.begin(); it != db.garages.end(); ++it) {
                    if (it->id == s.myGarageID) {
                        db.garages.erase(it);
                        s.myGarageID = -1; // Unlink
                        cout << "Garage deleted.\n"; return;
                    }
                }
            }
        }
    }

    static void addService(Database& db, int sellerID) {
        int gId = -1;
        for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) { cout << "Create garage first.\n"; return; }

        for (auto& g : db.garages) {
            if (g.id == gId) {
                string n; double p;
                cout << "Service Name: "; cin.ignore(); getline(cin, n);
                cout << "Price: "; cin >> p;
                int sId = g.services.size() + 1;
                g.services.push_back(Service(sId, n, p));
                cout << "Service Added!\n"; return;
            }
        }
    }

    static void deleteService(Database& db, int sellerID) {
        int gId = -1;
        for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) { cout << "No garage.\n"; return; }

        cout << "Enter Service ID to delete: "; int sId; cin >> sId;
        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (auto it = g.services.begin(); it != g.services.end(); ++it) {
                    if (it->id == sId) {
                        cout << "Delete '" << it->name << "' ($" << it->price << ")? (1.Yes 0.No): ";
                        int c; cin >> c;
                        if (c == 1) {
                            g.services.erase(it);
                            cout << "Service deleted.\n";
                        }
                        else {
                            cout << "Cancelled.\n";
                        }
                        return;
                    }
                }
                cout << "Service ID not found.\n"; return;
            }
        }
    }

    // --- UPDATED: VIEW APPOINTMENTS (With Service Name) ---
    static void viewAppointments(Database& db, int sellerID) {
        int gId = -1;
        for (auto& s : db.sellers) if (s.id == sellerID) gId = s.myGarageID;
        if (gId == -1) { cout << "You don't own a garage.\n"; return; }

        cout << "\n================ UPCOMING APPOINTMENTS ================\n";
        bool found = false;

        // Find the garage object to lookup service names
        Garage* myGarage = nullptr;
        for (auto& g : db.garages) if (g.id == gId) myGarage = &g;

        if (!myGarage) { cout << "Error finding garage data.\n"; return; }

        for (const auto& c : db.customers) {
            for (const auto& h : c.serviceHistory) {
                if (h.garageID == gId) {
                    // Lookup Service Name
                    string sName = "Unknown Service";
                    for (const auto& s : myGarage->services) {
                        if (s.id == h.serviceID) {
                            sName = s.name;
                            break;
                        }
                    }

                    cout << " > Customer: " << c.username
                        << " | Day: " << h.dayBooked
                        << " | Service: " << sName << " (ID " << h.serviceID << ")"
                        << " | Date: " << h.date << endl;
                    found = true;
                }
            }
        }
        if (!found) cout << "No upcoming appointments found.\n";
        else cout << "=======================================================\n";
    }

    static void viewMyBusiness(Database& db, int sellerID) {
        int rId = -1, gId = -1;
        for (const auto& s : db.sellers) {
            if (s.id == sellerID) { rId = s.myShowroomID; gId = s.myGarageID; break; }
        }

        cout << "\n=== MY BUSINESS ===\n";
        if (rId == -1) cout << "[No Showroom]\n";
        else {
            for (const auto& r : db.showrooms) {
                if (r.id == rId) {
                    cout << "SHOWROOM: " << r.name << " (ID: " << r.id << ")\n";
                    if (r.cars.empty()) cout << "  No cars listed.\n";
                    else for (const auto& c : r.cars) {
                        string st = (c.status == 2) ? "SOLD" : ((c.status == 1) ? "RESERVED" : "AVAILABLE");
                        cout << "  > ID: " << c.id << " | " << c.make << " " << c.model << " | $" << c.price << " | [" << st << "]\n";
                    }
                }
            }
        }
        cout << "-------------------\n";
        if (gId == -1) cout << "[No Garage]\n";
        else {
            for (const auto& g : db.garages) {
                if (g.id == gId) {
                    cout << "GARAGE: " << g.name << " (ID: " << g.id << ")\n";
                    if (g.services.empty()) cout << "  No services listed.\n";
                    else for (const auto& s : g.services) cout << "  > ID: " << s.id << " | " << s.name << " | $" << s.price << endl;
                }
            }
        }
        cout << "===================\n";
    }
};