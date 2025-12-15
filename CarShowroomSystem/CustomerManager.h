#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Database.h"
#include <ctime>
#include <iomanip>
#include <string>

class CustomerManager {
private:
    static string getDate() {
        time_t now = time(0); string dt = ctime(&now); if (!dt.empty()) dt.pop_back(); return dt;
    }

    static void checkTimers(Database& db) {
        time_t now = time(0);
        for (auto& r : db.showrooms) {
            for (auto& c : r.cars) {
                if (c.status == 1 && difftime(now, c.reservationStartTime) > 60.0) {
                    c.status = 0; c.reservationStartTime = 0;
                }
            }
        }
    }

public:
    static int login(Database& db, string user, string pass) {
        for (auto& c : db.customers) if (c.username == user && c.password == pass) return c.id;
        return -1;
    }

    static void registerCustomer(Database& db) {
        Customer c; c.id = db.customers.size() + 1;
        cout << "User: "; cin >> c.username; cout << "Pass: "; cin >> c.password;
        db.customers.push_back(c);
        cout << "Registered!\n";
    }

    static void viewMarket(Database& db) {
        checkTimers(db);
        cout << "\n================ MARKETPLACE ================\n";
        cout << "--- CAR SHOWROOMS ---\n";
        for (const auto& r : db.showrooms) {
            cout << "SHOWROOM: " << r.name << " (ID: " << r.id << ") - " << r.location << endl;
            if (r.cars.empty()) cout << "   (No cars available)\n";
            else {
                for (const auto& c : r.cars) {
                    string st = "AVAILABLE";
                    if (c.status == 1) st = "RESERVED";
                    if (c.status == 2) st = "SOLD";

                    cout << "   > ID:" << c.id << " | " << c.make << " " << c.model
                        << " | Buy: $" << c.price << " | Rent: $" << c.rentPrice << "/day | [" << st << "]\n";
                }
            }
            cout << "---------------------------------------------\n";
        }

        cout << "\n--- SERVICE GARAGES ---\n";
        for (const auto& g : db.garages) {
            cout << "GARAGE: " << g.name << " (ID: " << g.id << ") - " << g.location << endl;
            if (g.services.empty()) cout << "   (No services available)\n";
            else {
                for (const auto& s : g.services) {
                    cout << "   > ID:" << s.id << " | " << s.name << " | Price: $" << s.price << endl;
                }
            }
            cout << "---------------------------------------------\n";
        }
    }

    static void searchCars(Database& db) {
        checkTimers(db);
        string q; cout << "Enter Make or Model: "; cin >> q;
        bool found = false;
        cout << "\n--- Search Results ---\n";
        for (auto& r : db.showrooms) {
            for (auto& c : r.cars) {
                if ((c.make == q || c.model == q) && c.status == 0) {
                    cout << "Found in " << r.name << " (ID " << r.id << "): "
                        << c.make << " " << c.model << " [$" << c.price << "]\n";
                    found = true;
                }
            }
        }
        if (!found) cout << "No available cars found matching '" << q << "'.\n";
    }

    static void searchServices(Database& db) {
        string q; cout << "Enter Service Name: "; cin.ignore(); getline(cin, q);
        bool found = false;
        cout << "\n--- Search Results ---\n";
        for (auto& g : db.garages) {
            for (auto& s : g.services) {
                if (s.name.find(q) != string::npos) {
                    cout << "Found in " << g.name << " (ID " << g.id << "): "
                        << s.name << " (SrvID " << s.id << ") [$" << s.price << "]\n";
                    found = true;
                }
            }
        }
        if (!found) cout << "No services found.\n";
    }

    static void buyCarOrRent(Database& db, int custID) {
        checkTimers(db);
        int sId, cId;
        cout << "Enter Showroom ID: "; cin >> sId;
        cout << "Enter Car ID: "; cin >> cId;

        for (auto& r : db.showrooms) {
            if (r.id == sId) {
                for (auto& c : r.cars) {
                    if (c.id == cId) {
                        if (c.status == 2) { cout << "Error: This car is SOLD.\n"; return; }
                        if (c.status == 1) { cout << "Error: This car is RESERVED.\n"; return; }

                        cout << "\n--- CONFIRM SELECTION ---\n";
                        cout << c.make << " " << c.model << " (" << c.year << ")\n";
                        cout << "Cash Price: $" << c.price << "\n";
                        cout << "Rent Price: $" << c.rentPrice << "/day\n";
                        cout << "Reservation Deposit: $1000 (Refundable)\n";
                        cout << "-----------------------\n";
                        cout << "Is this correct? (1. Yes / 0. No): ";
                        int conf; cin >> conf;
                        if (conf != 1) { cout << "Transaction Cancelled.\n"; return; }

                        cout << "\nChoose Action:\n1. Cash Buy\n2. Rent\n3. Installment Plan\n4. Reserve (Pay $1000 Deposit)\nChoice: ";
                        int ch; cin >> ch;

                        // --- RESERVATION WITH DEPOSIT ---
                        if (ch == 4) {
                            c.status = 1; c.reservationStartTime = time(0);

                            // Save Deposit History
                            for (auto& u : db.customers) {
                                if (u.id == custID) {
                                    u.carHistory.push_back(HistoryRecordCars(sId, cId, getDate(), "RESERVE", 1000.0, 0));
                                    break;
                                }
                            }
                            cout << "Car Reserved! $1000 Deposit Paid.\n";
                            cout << "Note: Reservation expires in 60 seconds if not finalized.\n";
                            return;
                        }

                        double finalAmt = 0;
                        string type = "";
                        int months = 0;

                        if (ch == 1) {
                            finalAmt = c.price; type = "BUY";
                        }
                        else if (ch == 2) {
                            int d; cout << "Enter Days: "; cin >> d;
                            finalAmt = d * c.rentPrice; type = "RENT";
                            cout << "Total Rent Cost: $" << finalAmt << endl;
                            cout << "Confirm Renting? (1. Yes / 0. No): ";
                            int rconf; cin >> rconf; if (rconf != 1) return;
                        }
                        else if (ch == 3) {
                            int plans[] = { 3, 6, 12, 15, 18, 24, 30, 36 };
                            cout << "Select Plan:\n";
                            for (int i = 0; i < 8; i++) cout << i + 1 << ". " << plans[i] << " Months\n";
                            int p; cin >> p;
                            if (p < 1 || p > 8) { cout << "Invalid plan.\n"; return; }
                            months = plans[p - 1];
                            double monthly = c.price / months;
                            cout << "Plan: " << months << " months @ $" << monthly << "/mo\n";
                            cout << "Confirm? (1. Yes / 0. No): ";
                            int iconf; cin >> iconf; if (iconf != 1) return;
                            finalAmt = c.price; type = "INSTALLMENT";
                        }
                        else return;

                        c.status = 2; // Sold
                        for (auto& u : db.customers) {
                            if (u.id == custID) {
                                u.carHistory.push_back(HistoryRecordCars(sId, cId, getDate(), type, finalAmt, months));
                                cout << "Transaction Successful!\n";
                                return;
                            }
                        }
                    }
                }
            }
        }
        cout << "Invalid Showroom or Car ID.\n";
    }

    static void bookService(Database& db, int custID) {
        int gId, sId;
        cout << "Enter Garage ID: "; cin >> gId;
        cout << "Enter Service ID: "; cin >> sId;

        for (auto& g : db.garages) {
            if (g.id == gId) {
                for (const auto& s : g.services) {
                    if (s.id == sId) {
                        cout << "\nYou Selected: " << s.name << " at " << g.name << endl;
                        cout << "Price: $" << s.price << endl;
                        cout << "Confirm? (1. Yes / 0. No): ";
                        int c; cin >> c; if (c != 1) return;

                        string days[] = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
                        cout << "\nChoose Day:\n";
                        for (int i = 0; i < 7; i++) cout << i + 1 << ". " << days[i] << endl;
                        int d; cin >> d;
                        if (d < 1 || d > 7) return;
                        string dayStr = days[d - 1];

                        for (auto& cust : db.customers) {
                            if (cust.id == custID) {
                                cust.serviceHistory.push_back(HistoryRecordServices(gId, sId, getDate(), dayStr, s.price));
                                cout << "Booked successfully for " << dayStr << "!\n";
                                return;
                            }
                        }
                    }
                }
            }
        }
        cout << "Invalid Garage or Service ID.\n";
    }

    // --- UPDATED VIEW HISTORY (With Names) ---
    static void viewHistory(Database& db, int custID) {
        for (const auto& c : db.customers) {
            if (c.id == custID) {

                cout << "\n========= PURCHASE / RENT / RESERVE HISTORY =========\n";
                if (c.carHistory.empty()) cout << "No history.\n";
                else {
                    for (const auto& h : c.carHistory) {
                        // Lookup Names
                        string sName = "Unknown Showroom";
                        string cName = "Unknown Car";

                        for (const auto& r : db.showrooms) {
                            if (r.id == h.showroomID) {
                                sName = r.name;
                                for (const auto& car : r.cars) {
                                    if (car.id == h.carID) {
                                        cName = car.make + " " + car.model;
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        cout << " > Type: " << h.type
                            << " | Car: " << cName
                            << " | From: " << sName << endl
                            << "   Amount: $" << h.totalAmount
                            << " | Date: " << h.date;
                        if (h.installmentMonths > 0) cout << " [Plan: " << h.installmentMonths << " Months]";
                        cout << "\n-------------------------------------------------\n";
                    }
                }

                cout << "\n========= SERVICE APPOINTMENTS =========\n";
                if (c.serviceHistory.empty()) cout << "No appointments.\n";
                else {
                    for (const auto& h : c.serviceHistory) {
                        // Lookup Names
                        string gName = "Unknown Garage";
                        string sName = "Unknown Service";

                        for (const auto& g : db.garages) {
                            if (g.id == h.garageID) {
                                gName = g.name;
                                for (const auto& s : g.services) {
                                    if (s.id == h.serviceID) {
                                        sName = s.name;
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        cout << " > Service: " << sName
                            << " | At: " << gName << endl
                            << "   Booked For: " << h.dayBooked
                            << " | Cost: $" << h.cost
                            << " | Date Booked: " << h.date
                            << "\n-------------------------------------------------\n";
                    }
                }
                return;
            }
        }
    }
};