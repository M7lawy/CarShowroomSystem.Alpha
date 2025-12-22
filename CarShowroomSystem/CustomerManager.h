#pragma once
#include "Database.h"
#include <ctime>
#include <iomanip>

class CustomerManager {
private:
    static string getDate() { time_t now = time(0); string dt = ctime(&now); if (!dt.empty()) dt.pop_back(); return dt; }

    static void checkTimers(Database& db) {
        time_t now = time(0); bool changed = false;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            LinkedList<Car>::Node* c = r->data.cars.head;
            while (c) {
                if (c->data.status == 1 && difftime(now, c->data.reservationStartTime) > 60.0) {
                    c->data.status = 0; c->data.reservationStartTime = 0; c->data.reservedByCustomerID = -1; changed = true;
                }
                c = c->next;
            }
            r = r->next;
        }
        if (changed) db.saveAll();
    }

public:
    static int login(Database& db, string user, string pass) {
        LinkedList<Customer>::Node* c = db.customers.head;
        while (c) { if (c->data.username == user && c->data.password == pass) return c->data.id; c = c->next; }
        return -1;
    }

    static void registerCustomer(Database& db) {
        int maxId = 0; LinkedList<Customer>::Node* curr = db.customers.head;
        while (curr) { if (curr->data.id > maxId) maxId = curr->data.id; curr = curr->next; }
        Customer c; c.id = maxId + 1;
        cout << "User: "; cin >> c.username; cout << "Pass: "; cin >> c.password;
        db.customers.append(c); db.saveAll();
        cout << "Registered!\n";
    }

    static void changePassword(Database& db, int id) {
        LinkedList<Customer>::Node* c = db.customers.head;
        while (c) {
            if (c->data.id == id) {
                string n; cout << "New Password: "; cin >> n;
                c->data.password = n; db.saveAll(); cout << "Changed.\n"; return;
            }
            c = c->next;
        }
    }

    static void viewMarket(Database& db) {
        checkTimers(db);
        cout << "\n======= MARKETPLACE =======\n";
        cout << "--- CARS ---\n";
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            cout << "SHOWROOM: " << r->data.name << " (ID " << r->data.id << ")\n";
            LinkedList<Car>::Node* c = r->data.cars.head;
            while (c) {
                string st = (c->data.status == 0) ? "AVAIL" : ((c->data.status == 1) ? "RESERVED" : "SOLD");
                cout << "   ID:" << c->data.id << " | " << c->data.year << " " << c->data.make << " " << c->data.model
                    << " | Buy: $" << c->data.price << " | Rent: $" << c->data.rentPrice << "/day | " << st << endl;
                c = c->next;
            }
            cout << "---------------------------\n";
            r = r->next;
        }
        cout << "--- SERVICES ---\n";
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            cout << "GARAGE: " << g->data.name << " (ID " << g->data.id << ")\n";
            LinkedList<Service>::Node* s = g->data.services.head;
            while (s) {
                cout << "   ID:" << s->data.id << " | " << s->data.name << " | $" << s->data.price << endl;
                s = s->next;
            }
            cout << "---------------------------\n";
            g = g->next;
        }
    }

    static void viewShowroomsOnly(Database& db) {
        checkTimers(db);
        cout << "\n======= AVAILABLE CARS =======\n";
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            cout << "SHOWROOM: " << r->data.name << " (ID " << r->data.id << ")\n";
            LinkedList<Car>::Node* c = r->data.cars.head;
            while (c) {
                string st = (c->data.status == 0) ? "AVAIL" : ((c->data.status == 1) ? "RESERVED" : "SOLD");
                cout << "   ID:" << c->data.id << " | " << c->data.year << " " << c->data.make << " " << c->data.model
                    << " | Buy: $" << c->data.price << " | Rent: $" << c->data.rentPrice << "/day | " << st << endl;
                c = c->next;
            }
            cout << "------------------------------\n";
            r = r->next;
        }
    }

    static void viewServicesOnly(Database& db) {
        cout << "\n======= AVAILABLE SERVICES =======\n";
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            cout << "GARAGE: " << g->data.name << " (ID " << g->data.id << ") - " << g->data.location << endl;
            LinkedList<Service>::Node* s = g->data.services.head;
            while (s) {
                cout << "   > ID:" << s->data.id << " | " << s->data.name << " | $" << s->data.price << endl;
                s = s->next;
            }
            cout << "----------------------------------\n";
            g = g->next;
        }
    }

    static void buyCarOrRent(Database& db, int custID) {
        viewShowroomsOnly(db);
        int sId, cId; cout << "\nEnter Showroom ID: "; cin >> sId; cout << "Enter Car ID: "; cin >> cId;

        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == sId) {
                LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) {
                    if (c->data.id == cId) {
                        if (c->data.status == 2) { cout << "SOLD.\n"; return; }
                        if (c->data.status == 1 && c->data.reservedByCustomerID != custID) { cout << "ERROR: Reserved by another.\n"; return; }

                        cout << "\nSELECTED: " << c->data.year << " " << c->data.make << " " << c->data.model << endl;
                        cout << "Buy: $" << c->data.price << " | Rent: $" << c->data.rentPrice << "/day\n";
                        cout << "1. Buy Cash\n2. Rent\n3. Installment\n4. Reserve ($1000)\nChoice: ";
                        int ch; cin >> ch;

                        if (ch == 4) {
                            if (c->data.status == 1) { cout << "Already reserved.\n"; return; }
                            cout << "Confirm Reserve? Cost $1000. Cancel Penalty $500. (1.Yes 0.No): "; int rc; cin >> rc; if (rc != 1) return;
                            c->data.status = 1; c->data.reservationStartTime = time(0); c->data.reservedByCustomerID = custID;
                            LinkedList<Customer>::Node* u = db.customers.head;
                            while (u) { if (u->data.id == custID) u->data.carHistory.append(HistoryRecordCars(sId, cId, getDate(), "RESERVE", 1000.0, 0)); u = u->next; }
                            db.saveAll(); cout << "Reserved!\n"; return;
                        }

                        double finalAmt = 0; string type = ""; int months = 0;
                        if (ch == 1) { finalAmt = c->data.price; type = "BUY"; }
                        else if (ch == 2) {
                            int d; cout << "Days: "; cin >> d; finalAmt = c->data.rentPrice * d; type = "RENT";
                            cout << "Total: $" << finalAmt << ". Confirm? (1.Yes 0.No): "; int xc; cin >> xc; if (xc != 1) return;
                        }
                        else if (ch == 3) { cout << "Months (3/6/12): "; cin >> months; finalAmt = c->data.price; type = "INSTALLMENT"; }
                        else return;

                        c->data.status = 2; c->data.reservedByCustomerID = -1;
                        LinkedList<Customer>::Node* u = db.customers.head;
                        while (u) {
                            if (u->data.id == custID) {
                                u->data.carHistory.append(HistoryRecordCars(sId, cId, getDate(), type, finalAmt, months));
                                db.saveAll(); cout << "Success.\n"; return;
                            }
                            u = u->next;
                        }
                    }
                    c = c->next;
                }
            }
            r = r->next;
        }
        cout << "Not Found.\n";
    }

    static void bookService(Database& db, int custID) {
        viewServicesOnly(db);
        int gId, sId; cout << "\nEnter Garage ID: "; cin >> gId; cout << "Enter Service ID: "; cin >> sId;

        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                LinkedList<Service>::Node* s = g->data.services.head;
                while (s) {
                    if (s->data.id == sId) {
                        cout << "Selected: " << s->data.name << " ($" << s->data.price << ")\n";
                        string days[] = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
                        cout << "Choose Day:\n"; for (int i = 0; i < 7; i++) cout << i + 1 << ". " << days[i] << endl;
                        int d; cin >> d; if (d < 1 || d>7) return;

                        LinkedList<Customer>::Node* u = db.customers.head;
                        while (u) {
                            if (u->data.id == custID) {
                                u->data.serviceHistory.append(HistoryRecordServices(gId, sId, getDate(), days[d - 1], s->data.price));
                                db.saveAll(); cout << "Booked!\n"; return;
                            }
                            u = u->next;
                        }
                    }
                    s = s->next;
                }
            }
            g = g->next;
        }
        cout << "Not Found.\n";
    }

    static void viewMyReservations(Database& db, int custID) {
        checkTimers(db);
        cout << "\n======= MY RESERVATIONS =======\n";
        bool found = false;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            LinkedList<Car>::Node* c = r->data.cars.head;
            while (c) {
                if (c->data.status == 1 && c->data.reservedByCustomerID == custID) {
                    double timeLeft = 60.0 - difftime(time(0), c->data.reservationStartTime); if (timeLeft < 0) timeLeft = 0;
                    cout << "SR: " << r->data.id << " | Car: " << c->data.year << " " << c->data.make << " " << c->data.model << " | Expires: " << (int)timeLeft << "s\n";
                    found = true;
                }
                c = c->next;
            }
            r = r->next;
        }
        if (!found) cout << "No active reservations.\n";
    }

    static void cancelReservation(Database& db, int custID) {
        viewMyReservations(db, custID);
        int sId, cId; cout << "Enter SR ID: "; cin >> sId; cout << "Enter Car ID: "; cin >> cId;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == sId) {
                LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) {
                    if (c->data.id == cId && c->data.status == 1 && c->data.reservedByCustomerID == custID) {
                        c->data.status = 0; c->data.reservedByCustomerID = -1; c->data.reservationStartTime = 0;
                        db.saveAll(); cout << "Cancelled ($500 Refunded).\n"; return;
                    }
                    c = c->next;
                }
            }
            r = r->next;
        }
        cout << "Invalid.\n";
    }

    static void viewHistory(Database& db, int custID) {
        cout << "\n========= HISTORY =========\n";
        LinkedList<Customer>::Node* c = db.customers.head;
        while (c) {
            if (c->data.id == custID) {
                cout << "--- CARS ---\n";
                if (c->data.carHistory.isEmpty()) cout << "None.\n";
                LinkedList<HistoryRecordCars>::Node* h = c->data.carHistory.head;
                while (h) {
                    cout << "[" << h->data.type << "] Amt: $" << h->data.totalAmount << " | Date: " << h->data.date << endl;
                    h = h->next;
                }
                cout << "\n--- SERVICES ---\n";
                if (c->data.serviceHistory.isEmpty()) cout << "None.\n";
                LinkedList<HistoryRecordServices>::Node* hs = c->data.serviceHistory.head;
                while (hs) {
                    cout << "[SERVICE] Cost: $" << hs->data.cost << " | Day: " << hs->data.dayBooked << endl;
                    hs = hs->next;
                }
            }
            c = c->next;
        }
        cout << "===========================\n";
    }
};