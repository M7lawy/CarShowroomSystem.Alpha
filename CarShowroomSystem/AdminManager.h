#pragma once
#include "Database.h"
#include <iostream>
#include <string>

using namespace std;

class AdminManager {
public:
    static int login(Database& db, string user, string pass) {
        LinkedList<Admin>::Node* curr = db.admins.head;
        while (curr) { if (curr->data.username == user && curr->data.password == pass) return curr->data.id; curr = curr->next; }
        return -1;
    }

    static void addAdmin(Database& db) {
        int maxId = 0; LinkedList<Admin>::Node* curr = db.admins.head;
        while (curr) { if (curr->data.id > maxId) maxId = curr->data.id; curr = curr->next; }
        Admin a; a.id = maxId + 1;
        cout << "New User: "; cin >> a.username; cout << "New Pass: "; cin >> a.password;
        db.admins.append(a); db.saveAll();
        cout << "Admin Added.\n";
    }

    static void changePassword(Database& db, int id) {
        LinkedList<Admin>::Node* a = db.admins.head;
        while (a) {
            if (a->data.id == id) {
                string n; cout << "New Password: "; cin >> n;
                a->data.password = n; db.saveAll(); cout << "Changed.\n"; return;
            }
            a = a->next;
        }
    }

    static void listShowrooms(Database& db) {
        cout << "\n--- CURRENT SHOWROOMS ---\n";
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            cout << "ID: " << r->data.id << " | Name: " << r->data.name << " | Loc: " << r->data.location << endl;
            LinkedList<Car>::Node* c = r->data.cars.head;
            while (c) {
                string st = (c->data.status == 2) ? "SOLD" : (c->data.status == 1 ? "RESERVED" : "AVAILABLE");
                cout << "   -> Car ID: " << c->data.id << " | " << c->data.year << " " << c->data.make << " " << c->data.model << " | $" << c->data.price << " | Rent: $" << c->data.rentPrice << " | " << st << endl;
                c = c->next;
            }
            r = r->next;
        }
        cout << "-------------------------\n";
    }

    static void listGarages(Database& db) {
        cout << "\n--- CURRENT GARAGES ---\n";
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            cout << "ID: " << g->data.id << " | Name: " << g->data.name << " | Loc: " << g->data.location << endl;
            LinkedList<Service>::Node* s = g->data.services.head;
            while (s) {
                cout << "   -> Srv ID: " << s->data.id << " | " << s->data.name << " ($" << s->data.price << ")" << endl;
                s = s->next;
            }
            g = g->next;
        }
        cout << "-----------------------\n";
    }

    static void addShowroom(Database& db) {
        int maxId = 0; LinkedList<Showroom>::Node* curr = db.showrooms.head;
        while (curr) { if (curr->data.id > maxId) maxId = curr->data.id; curr = curr->next; }
        Showroom s; s.id = maxId + 1;
        cout << "Name: "; cin.ignore(); getline(cin, s.name);
        cout << "Location: "; getline(cin, s.location); cout << "Phone: "; cin >> s.phone;
        db.showrooms.append(s); db.saveAll(); cout << "Showroom Added.\n";
    }

    static void deleteShowroom(Database& db) {
        listShowrooms(db);
        int id; cout << "Enter Showroom ID to DELETE: "; cin >> id;
        if (db.showrooms.removeById(id)) {
            LinkedList<Seller>::Node* s = db.sellers.head;
            while (s) { if (s->data.myShowroomID == id) s->data.myShowroomID = -1; s = s->next; }
            db.saveAll(); cout << "Deleted.\n";
        }
        else cout << "Not Found.\n";
    }

    static void addCarGlobal(Database& db) {
        listShowrooms(db);
        int sId; cout << "Enter Showroom ID: "; cin >> sId;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == sId) {
                int maxId = 0; LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) { if (c->data.id > maxId) maxId = c->data.id; c = c->next; }
                int cid = maxId + 1;
                string mk, md; double p, rp; int y;
                cout << "Make: "; cin >> mk; cout << "Model: "; cin >> md;
                cout << "Year: "; cin >> y; cout << "Price: "; cin >> p; cout << "Rent Price: "; cin >> rp;
                r->data.cars.append(Car(cid, mk, md, y, p, rp)); db.saveAll(); cout << "Car Added.\n"; return;
            }
            r = r->next;
        }
        cout << "Showroom not found.\n";
    }

    static void deleteCarGlobal(Database& db) {
        listShowrooms(db);
        int sId, cId; cout << "Showroom ID: "; cin >> sId; cout << "Car ID: "; cin >> cId;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == sId) {
                if (r->data.cars.removeById(cId)) { db.saveAll(); cout << "Deleted.\n"; return; }
            }
            r = r->next;
        }
        cout << "Not Found.\n";
    }

    static void editCarPriceGlobal(Database& db) {
        listShowrooms(db);
        int sId, cId; cout << "Showroom ID: "; cin >> sId; cout << "Car ID: "; cin >> cId;
        LinkedList<Showroom>::Node* r = db.showrooms.head;
        while (r) {
            if (r->data.id == sId) {
                LinkedList<Car>::Node* c = r->data.cars.head;
                while (c) {
                    if (c->data.id == cId) {
                        cout << "Editing: " << c->data.year << " " << c->data.make << " " << c->data.model << endl;
                        cout << "Current Price: $" << c->data.price << ". New Price: "; cin >> c->data.price;
                        cout << "Current Rent: $" << c->data.rentPrice << ". New Rent: "; cin >> c->data.rentPrice;
                        db.saveAll(); cout << "Updated.\n"; return;
                    }
                    c = c->next;
                }
            }
            r = r->next;
        }
        cout << "Not Found.\n";
    }

    static void addGarage(Database& db) {
        int maxId = 0; LinkedList<Garage>::Node* curr = db.garages.head;
        while (curr) { if (curr->data.id > maxId) maxId = curr->data.id; curr = curr->next; }
        Garage g; g.id = maxId + 1;
        cout << "Name: "; cin.ignore(); getline(cin, g.name);
        cout << "Location: "; getline(cin, g.location); cout << "Phone: "; cin >> g.phone;
        db.garages.append(g); db.saveAll(); cout << "Garage Added.\n";
    }

    static void deleteGarage(Database& db) {
        listGarages(db);
        int id; cout << "Enter Garage ID to DELETE: "; cin >> id;
        if (db.garages.removeById(id)) {
            LinkedList<Seller>::Node* s = db.sellers.head;
            while (s) { if (s->data.myGarageID == id) s->data.myGarageID = -1; s = s->next; }
            db.saveAll(); cout << "Deleted.\n";
        }
        else cout << "Not Found.\n";
    }

    static void addServiceGlobal(Database& db) {
        listGarages(db);
        int gId; cout << "Enter Garage ID: "; cin >> gId;
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                string n; double p; cout << "Name: "; cin.ignore(); getline(cin, n); cout << "Price: "; cin >> p;
                int maxId = 0; LinkedList<Service>::Node* s = g->data.services.head;
                while (s) { if (s->data.id > maxId) maxId = s->data.id; s = s->next; }
                g->data.services.append(Service(maxId + 1, n, p)); db.saveAll(); cout << "Added.\n"; return;
            }
            g = g->next;
        }
        cout << "Garage not found.\n";
    }

    static void deleteServiceGlobal(Database& db) {
        listGarages(db);
        int gId, sId; cout << "Garage ID: "; cin >> gId; cout << "Service ID: "; cin >> sId;
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                if (g->data.services.removeById(sId)) { db.saveAll(); cout << "Deleted.\n"; return; }
            }
            g = g->next;
        }
        cout << "Not Found.\n";
    }

    static void editServicePriceGlobal(Database& db) {
        listGarages(db);
        int gId, sId; cout << "Garage ID: "; cin >> gId; cout << "Service ID: "; cin >> sId;
        LinkedList<Garage>::Node* g = db.garages.head;
        while (g) {
            if (g->data.id == gId) {
                LinkedList<Service>::Node* s = g->data.services.head;
                while (s) {
                    if (s->data.id == sId) {
                        cout << "Editing: " << s->data.name << endl;
                        cout << "Current Price: $" << s->data.price << ". New Price: "; cin >> s->data.price;
                        db.saveAll(); cout << "Updated.\n"; return;
                    }
                    s = s->next;
                }
            }
            g = g->next;
        }
        cout << "Not Found.\n";
    }

    static void viewAll(Database& db) { listShowrooms(db); listGarages(db); }
};