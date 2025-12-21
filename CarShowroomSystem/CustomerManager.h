#pragma once
#include "Database.h"
#include <ctime>
#include <iomanip>

class CustomerManager {
private:
    static string getDate() {
        time_t now = time(0); string dt = ctime(&now); if (!dt.empty()) dt.pop_back(); return dt;
    }

    static void checkTimers(Database& db) {
        time_t now = time(0);
        bool changed = false;
        for (auto& r : db.showrooms) {
            for (auto& c : r.cars) {
                if (c.status == 1 && difftime(now, c.reservationStartTime) > 60.0) {
                    c.status = 0; c.reservationStartTime = 0; c.reservedByCustomerID = -1;
                    changed = true;
                }
            }
        }
        if (changed) db.saveAll();
    }

public:
    static int login(Database& db, string user, string pass) {
        for (auto& c : db.customers) if (c.username == user && c.password == pass) return c.id;
        return -1;
    }

    static void registerCustomer(Database& db) {
        int maxId = 0; for (const auto& x : db.customers) if (x.id > maxId) maxId = x.id;
        Customer c; c.id = maxId + 1;
        cout << "User: "; cin >> c.username; cout << "Pass: "; cin >> c.password;
        db.customers.push_back(c); db.saveAll();
        cout << "Registered!\n";
    }

    // --- VIEW ALL (Marketplace) ---
    static void viewMarket(Database& db) {
        checkTimers(db);
        cout << "\n======= MARKETPLACE =======\n";
        cout << "--- CARS ---\n";
        for (const auto& r : db.showrooms) {
            cout << "SHOWROOM: " << r.name << " (ID " << r.id << ")\n";
            for (const auto& c : r.cars) {
                string st = (c.status == 0) ? "AVAIL" : ((c.status == 1) ? "RESERVED" : "SOLD");
                cout << "   ID:" << c.id << " | " << c.year << " " << c.make << " " << c.model
                    << " | Buy: $" << c.price << " | Rent: $" << c.rentPrice << "/day | " << st << endl;
            }
            cout << "---------------------------\n";
        }
        cout << "--- SERVICES ---\n";
        for (const auto& g : db.garages) {
            cout << "GARAGE: " << g.name << " (ID " << g.id << ")\n";
            for (const auto& s : g.services)
                cout << "   ID:" << s.id << " | " << s.name << " | $" << s.price << endl;
            cout << "---------------------------\n";
        }
    }

    // --- VIEW SHOWROOMS ONLY (For Buy Menu) ---
    static void viewShowroomsOnly(Database& db) {
        checkTimers(db);
        cout << "\n======= AVAILABLE CARS =======\n";
        for (const auto& r : db.showrooms) {
            cout << "SHOWROOM: " << r.name << " (ID " << r.id << ")\n";
            for (const auto& c : r.cars) {
                string st = (c.status == 0) ? "AVAIL" : ((c.status == 1) ? "RESERVED" : "SOLD");
                cout << "   ID:" << c.id << " | " << c.year << " " << c.make << " " << c.model
                    << " | Buy: $" << c.price << " | Rent: $" << c.rentPrice << "/day | " << st << endl;
            }
            cout << "------------------------------\n";
        }
    }

    // --- VIEW SERVICES ONLY (For Service Menu) ---
    static void viewServicesOnly(Database& db) {
        cout << "\n======= AVAILABLE SERVICES =======\n";
        for (const auto& g : db.garages) {
            cout << "GARAGE: " << g.name << " (ID " << g.id << ") - " << g.location << endl;
            for (const auto& s : g.services)
                cout << "   > ID:" << s.id << " | " << s.name << " | $" << s.price << endl;
            cout << "----------------------------------\n";
        }
    }

    static void buyCarOrRent(Database& db, int custID) {
        viewShowroomsOnly(db);
        int sId, cId; cout << "\nEnter Showroom ID: "; cin >> sId; cout << "Enter Car ID: "; cin >> cId;

        for (auto& r : db.showrooms) {
            if (r.id == sId) {
                for (auto& c : r.cars) {
                    if (c.id == cId) {
                        if (c.status == 2) { cout << "SOLD.\n"; return; }

                        if (c.status == 1 && c.reservedByCustomerID != custID) {
                            cout << "ERROR: Reserved by another customer.\n"; return;
                        }

                        cout << "\nSELECTED: " << c.year << " " << c.make << " " << c.model << endl;
                        cout << "Buy Price: $" << c.price << " | Rent Price: $" << c.rentPrice << "/day\n";
                        cout << "1. Buy Cash\n2. Rent\n3. Installment\n4. Reserve ($1000 Deposit)\nChoice: ";
                        int ch; cin >> ch;

                        // --- RESERVE ---
                        if (ch == 4) {
                            if (c.status == 1) { cout << "Already reserved.\n"; return; }

                            cout << "Confirm Reservation? Cost is $1000. \nWARNING: Canceling returns only $500 ($500 Penalty). (1.Yes 0.No): ";
                            int rc; cin >> rc;
                            if (rc != 1) return;

                            c.status = 1; c.reservationStartTime = time(0); c.reservedByCustomerID = custID;
                            for (auto& u : db.customers) if (u.id == custID) u.carHistory.push_back(HistoryRecordCars(sId, cId, getDate(), "RESERVE", 1000.0, 0));
                            db.saveAll();
                            cout << "Reserved for 60 seconds!\n"; return;
                        }

                        double finalAmt = 0; string type = ""; int months = 0;

                        if (ch == 1) { finalAmt = c.price; type = "BUY"; }

                        // --- RENT ---
                        else if (ch == 2) {
                            int d; cout << "Enter Number of Days: "; cin >> d;
                            finalAmt = c.rentPrice * d; type = "RENT";

                            // CONFIRMATION
                            cout << "Total Rent Price: $" << finalAmt << " for " << d << " days.\n";
                            cout << "Confirm Renting? (1. Yes / 0. No): ";
                            int rentConf; cin >> rentConf;
                            if (rentConf != 1) { cout << "Cancelled.\n"; return; }
                        }
                        else if (ch == 3) { cout << "Months (3/6/12): "; cin >> months; finalAmt = c.price; type = "INSTALLMENT"; }
                        else return;

                        c.status = 2; c.reservedByCustomerID = -1;
                        for (auto& u : db.customers) {
                            if (u.id == custID) {
                                u.carHistory.push_back(HistoryRecordCars(sId, cId, getDate(), type, finalAmt, months));
                                db.saveAll();
                                cout << "Transaction Complete.\n"; return;
                            }
                        }
                    }
                }
            }
        }
        cout << "Not Found.\n";
    }

    static void bookService(Database& db, int custID) {
        viewServicesOnly(db);
        int gId, sId; cout << "\nEnter Garage ID: "; cin >> gId; cout << "Enter Service ID: "; cin >> sId;

        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (auto& s : g.services) {
                    if (s.id == sId) {
                        cout << "Selected: " << s.name << " ($" << s.price << ")\n";
                        string days[] = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
                        cout << "Choose Day:\n";
                        for (int i = 0; i < 7; i++) cout << i + 1 << ". " << days[i] << endl;
                        int d; cin >> d;
                        if (d < 1 || d>7) return;

                        for (auto& u : db.customers) {
                            if (u.id == custID) {
                                u.serviceHistory.push_back(HistoryRecordServices(gId, sId, getDate(), days[d - 1], s.price));
                                db.saveAll();
                                cout << "Booked for " << days[d - 1] << "!\n"; return;
                            }
                        }
                    }
                }
            }
        }
        cout << "Not Found.\n";
    }

    static void viewMyReservations(Database& db, int custID) {
        checkTimers(db);
        cout << "\n======= MY RESERVATIONS =======\n";
        bool found = false;
        for (auto& r : db.showrooms) {
            for (auto& c : r.cars) {
                if (c.status == 1 && c.reservedByCustomerID == custID) {
                    double timeLeft = 60.0 - difftime(time(0), c.reservationStartTime);
                    if (timeLeft < 0) timeLeft = 0;
                    cout << "SR: " << r.id << " | Car: " << c.year << " " << c.make << " " << c.model
                        << " | Expires: " << (int)timeLeft << "s\n";
                    found = true;
                }
            }
        }
        if (!found) cout << "No active reservations.\n";
    }

    static void cancelReservation(Database& db, int custID) {
        viewMyReservations(db, custID);
        int sId, cId; cout << "Enter SR ID to Cancel: "; cin >> sId; cout << "Enter Car ID: "; cin >> cId;
        for (auto& r : db.showrooms) {
            if (r.id == sId) {
                for (auto& c : r.cars) {
                    if (c.id == cId && c.status == 1 && c.reservedByCustomerID == custID) {
                        c.status = 0; c.reservedByCustomerID = -1; c.reservationStartTime = 0;
                        db.saveAll();
                        cout << "Cancelled. You have been refunded $500 ($500 Penalty deducted).\n";
                        return;
                    }
                }
            }
        }
        cout << "Invalid selection.\n";
    }

    static void viewHistory(Database& db, int custID) {
        cout << "\n========= FULL HISTORY =========\n";
        for (const auto& c : db.customers) {
            if (c.id == custID) {
                cout << "--- CARS (Bought/Rented/Reserved) ---\n";
                if (c.carHistory.empty()) cout << "None.\n";
                for (const auto& h : c.carHistory) {
                    string cInfo = "Unknown";
                    for (auto& r : db.showrooms) if (r.id == h.showroomID) for (auto& car : r.cars) if (car.id == h.carID)
                        cInfo = to_string(car.year) + " " + car.make + " " + car.model;
                    cout << "[" << h.type << "] " << cInfo << " | Amount: $" << h.totalAmount << " | Date: " << h.date << endl;
                }
                cout << "\n--- SERVICES APPOINTED ---\n";
                if (c.serviceHistory.empty()) cout << "None.\n";
                for (const auto& h : c.serviceHistory) {
                    string sName = "Unknown";
                    for (auto& g : db.garages) if (g.id == h.garageID) for (auto& s : g.services) if (s.id == h.serviceID) sName = s.name;
                    cout << "[SERVICE] " << sName << " | Day: " << h.dayBooked << " | Cost: $" << h.cost << endl;
                }
            }
        }
        cout << "================================\n";
    }
};