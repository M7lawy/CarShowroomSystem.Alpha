#pragma once
#include "Database.h"
#include <iostream>
#include <string>

using namespace std;

class SellerManager {
public:
    static int login(Database& db, string user, string pass) {
        LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.username == user && s->data.password == pass) return s->data.id; s = s->next; }
        return -1;
    }

    static void registerSeller(Database& db) {
        int maxId = 0; LinkedList<Seller>::Node* curr = db.sellers.head;
        while (curr) { if (curr->data.id > maxId) maxId = curr->data.id; curr = curr->next; }
        Seller s; s.id = maxId + 1;
        cout << "User: "; cin >> s.username; cout << "Pass: "; cin >> s.password;
        db.sellers.append(s); db.saveAll();
        cout << "Registered! ID: " << s.id << endl;
    }

    static void changePassword(Database& db, int id) {
        LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) {
            if (s->data.id == id) {
                string n; cout << "New Password: "; cin >> n;
                s->data.password = n; db.saveAll(); cout << "Changed.\n"; return;
            }
            s = s->next;
        }
    }

    static void showMyCars(Database& db, int sellerID) {
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }

        if (rId == -1) { cout << "No Showroom.\n"; return; }
        cout << "\n--- MY CARS ---\n";

        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == rId) {
                if (r->data.cars.isEmpty()) cout << "No cars.\n";
                LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) {
                    string st = (c->data.status == 2) ? "SOLD" : (c->data.status == 1 ? "RESERVED" : "AVAILABLE");
                    cout << "ID: " << c->data.id << " | " << c->data.year << " " << c->data.make << " " << c->data.model
                        << " | $" << c->data.price << " | Rent: $" << c->data.rentPrice << " | " << st << endl;
                    c = c->next;
                }
            }
            r = r->next;
        }
        cout << "---------------\n";
    }

    static void showMyServices(Database& db, int sellerID) {
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }

        if (gId == -1) { cout << "No Garage.\n"; return; }
        cout << "\n--- MY SERVICES ---\n";

        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                if (g->data.services.isEmpty()) cout << "No services.\n";
                LinkedList<Service>::Node* sv = g->data.services.head;
                while (sv) {
                    cout << "ID: " << sv->data.id << " | " << sv->data.name << " | $" << sv->data.price << endl;
                    sv = sv->next;
                }
            }
            g = g->next;
        }
        cout << "-------------------\n";
    }

    static void createShowroom(Database& db, int sellerID) {
        LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) {
            if (s->data.id == sellerID) {
                if (s->data.myShowroomID != -1) { cout << "Already have a showroom!\n"; return; }
                int maxId = 0; LinkedList<Showroom>::Node* rTmp = db.showrooms.head;
                while (rTmp) { if (rTmp->data.id > maxId) maxId = rTmp->data.id; rTmp = rTmp->next; }

                Showroom r; r.id = maxId + 1;
                cout << "Name: "; cin.ignore(); getline(cin, r.name);
                cout << "Location: "; getline(cin, r.location); cout << "Phone: "; cin >> r.phone;
                db.showrooms.append(r); s->data.myShowroomID = r.id; db.saveAll();
                cout << "Created.\n"; return;
            }
            s = s->next;
        }
    }

    static void createGarage(Database& db, int sellerID) {
        LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) {
            if (s->data.id == sellerID) {
                if (s->data.myGarageID != -1) { cout << "Already have a garage.\n"; return; }
                int maxId = 0; LinkedList<Garage>::Node* gTmp = db.garages.head;
                while (gTmp) { if (gTmp->data.id > maxId) maxId = gTmp->data.id; gTmp = gTmp->next; }

                Garage g; g.id = maxId + 1;
                cout << "Name: "; cin.ignore(); getline(cin, g.name);
                cout << "Location: "; getline(cin, g.location); cout << "Phone: "; cin >> g.phone;
                db.garages.append(g); s->data.myGarageID = g.id; db.saveAll();
                cout << "Garage Created.\n"; return;
            }
            s = s->next;
        }
    }

    static void addCar(Database& db, int sellerID) {
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }
        if (rId == -1) { cout << "Create showroom first.\n"; return; }

        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == rId) {
                int maxId = 0; LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) { if (c->data.id > maxId) maxId = c->data.id; c = c->next; }
                int cid = maxId + 1;
                string mk, md; double p, rp; int y;
                cout << "Make: "; cin >> mk; cout << "Model: "; cin >> md;
                cout << "Year: "; cin >> y; cout << "Price: "; cin >> p; cout << "Rent Price: "; cin >> rp;
                r->data.cars.append(Car(cid, mk, md, y, p, rp)); db.saveAll();
                cout << "Car Added.\n"; return;
            }
            r = r->next;
        }
    }

    static void deleteCar(Database& db, int sellerID) {
        showMyCars(db, sellerID);
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }
        if (rId == -1) return;

        int cId; cout << "Car ID to delete: "; cin >> cId;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == rId) {
                if (r->data.cars.removeById(cId)) { db.saveAll(); cout << "Deleted.\n"; return; }
            }
            r = r->next;
        }
        cout << "Car not found.\n";
    }

    static void deleteAllCars(Database& db, int sellerID) {
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }
        if (rId == -1) return;

        cout << "Delete ALL cars? (1.Yes 0.No): "; int c; cin >> c; if (c != 1) return;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) { if (r->data.id == rId) { r->data.cars.clear(); db.saveAll(); cout << "All Cars Deleted.\n"; return; } r = r->next; }
    }

    static void deleteMyShowroom(Database& db, int sellerID) {
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }
        if (rId == -1) return;

        cout << "Delete ENTIRE Showroom? (1.Yes 0.No): "; int c; cin >> c; if (c != 1) return;
        if (db.showrooms.removeById(rId)) {
            LinkedList<Seller>::Node* tempS = db.sellers.head;
            while (tempS) { if (tempS->data.id == sellerID) tempS->data.myShowroomID = -1; tempS = tempS->next; }
            db.saveAll(); cout << "Showroom Deleted.\n";
        }
    }

    static void editCarPrice(Database& db, int sellerID) {
        showMyCars(db, sellerID);
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }
        if (rId == -1) return;

        int cId; cout << "Enter Car ID: "; cin >> cId;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == rId) {
                LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) {
                    if (c->data.id == cId) {
                        cout << "Current Price: " << c->data.price << ". New: "; cin >> c->data.price;
                        cout << "Current Rent: " << c->data.rentPrice << ". New: "; cin >> c->data.rentPrice;
                        db.saveAll(); cout << "Updated.\n"; return;
                    }
                    c = c->next;
                }
            }
            r = r->next;
        }
        cout << "Not Found.\n";
    }

    static void addService(Database& db, int sellerID) {
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }
        if (gId == -1) { cout << "Create garage first.\n"; return; }

        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                string n; double p; cout << "Name: "; cin.ignore(); getline(cin, n); cout << "Price: "; cin >> p;
                int maxId = 0; LinkedList<Service>::Node* sv = g->data.services.head;
                while (sv) { if (sv->data.id > maxId) maxId = sv->data.id; sv = sv->next; }
                int sId = maxId + 1;
                g->data.services.append(Service(sId, n, p)); db.saveAll();
                cout << "Service Added!\n"; return;
            }
            g = g->next;
        }
    }

    static void deleteService(Database& db, int sellerID) {
        showMyServices(db, sellerID);
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }
        if (gId == -1) return;

        int sId; cout << "Service ID: "; cin >> sId;
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                if (g->data.services.removeById(sId)) { db.saveAll(); cout << "Deleted.\n"; return; }
            }
            g = g->next;
        }
        cout << "Not Found.\n";
    }

    static void deleteAllServices(Database& db, int sellerID) {
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }
        if (gId == -1) return;
        cout << "Delete ALL Services? (1.Yes 0.No): "; int c; cin >> c; if (c != 1) return;
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) { if (g->data.id == gId) { g->data.services.clear(); db.saveAll(); cout << "All Deleted.\n"; return; } g = g->next; }
    }

    static void deleteMyGarage(Database& db, int sellerID) {
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }
        if (gId == -1) return;
        cout << "Delete ENTIRE Garage? (1.Yes 0.No): "; int c; cin >> c; if (c != 1) return;
        if (db.garages.removeById(gId)) {
            LinkedList<Seller>::Node* tempS = db.sellers.head;
            while (tempS) { if (tempS->data.id == sellerID) tempS->data.myGarageID = -1; tempS = tempS->next; }
            db.saveAll(); cout << "Garage Deleted.\n";
        }
    }

    static void editServicePrice(Database& db, int sellerID) {
        showMyServices(db, sellerID);
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }
        if (gId == -1) return;

        int sId; cout << "Enter Service ID: "; cin >> sId;
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                LinkedList<Service>::Node* sv = g->data.services.head;
                while (sv) {
                    if (sv->data.id == sId) {
                        cout << "Current: " << sv->data.price << ". New: "; cin >> sv->data.price;
                        db.saveAll(); cout << "Updated.\n"; return;
                    }
                    sv = sv->next;
                }
            }
            g = g->next;
        }
        cout << "Not Found.\n";
    }

    static void viewMyBusiness(Database& db, int sellerID) { showMyCars(db, sellerID); showMyServices(db, sellerID); }

    static void viewSoldCars(Database& db, int sellerID) {
        int rId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) rId = s->data.myShowroomID; s = s->next; }
        if (rId == -1) { cout << "No Showroom.\n"; return; }

        cout << "\n--- SOLD CARS ---\n";
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        bool found = false;
        while (r) {
            if (r->data.id == rId) {
                LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) {
                    if (c->data.status == 2) { cout << "ID: " << c->data.id << " | " << c->data.make << " ($" << c->data.price << ")\n"; found = true; }
                    c = c->next;
                }
            }
            r = r->next;
        }
        if (!found) cout << "None.\n";
    }

    static void viewServicesProvided(Database& db, int sellerID) {
        int gId = -1; LinkedList<Seller>::Node* s = db.sellers.head;
        while (s) { if (s->data.id == sellerID) gId = s->data.myGarageID; s = s->next; }
        if (gId == -1) { cout << "No Garage.\n"; return; }

        cout << "\n--- SERVICES PROVIDED ---\n";
        LinkedList<Customer>::Node* c = db.customers.head;
        while (c) {
            LinkedList<HistoryRecordServices>::Node* h = c->data.serviceHistory.head;
            while (h) {
                if (h->data.garageID == gId) {
                    cout << "Cust: " << c->data.username << " | Date: " << h->data.date << " | $" << h->data.cost << endl;
                }
                h = h->next;
            }
            c = c->next;
        }
    }
};