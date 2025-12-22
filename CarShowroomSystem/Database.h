#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "DataClasses.h"

using namespace std;

class Database {
public:
    LinkedList<Showroom> showrooms;
    LinkedList<Garage> garages;
    LinkedList<Admin> admins;
    LinkedList<Seller> sellers;
    LinkedList<Customer> customers;

    vector<string> split(string s, char delimiter) {
        vector<string> tokens; string token; istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) tokens.push_back(token);
        return tokens;
    }

    Database() { loadAll(); }

    void saveAll() {
        { ofstream f("admins.txt"); LinkedList<Admin>::Node* curr = admins.head; while (curr) { f << curr->data.id << "," << curr->data.username << "," << curr->data.password << endl; curr = curr->next; } }
        { ofstream f("sellers.txt"); LinkedList<Seller>::Node* curr = sellers.head; while (curr) { f << curr->data.id << "," << curr->data.username << "," << curr->data.password << "," << curr->data.myShowroomID << "," << curr->data.myGarageID << endl; curr = curr->next; } }
        { ofstream f("customers.txt"); LinkedList<Customer>::Node* curr = customers.head; while (curr) { f << curr->data.id << "," << curr->data.username << "," << curr->data.password << endl; curr = curr->next; } }
        { ofstream f("showrooms.txt"); LinkedList<Showroom>::Node* curr = showrooms.head; while (curr) { f << curr->data.id << "," << curr->data.name << "," << curr->data.location << "," << curr->data.phone << endl; curr = curr->next; } }
        { ofstream f("garages.txt"); LinkedList<Garage>::Node* curr = garages.head; while (curr) { f << curr->data.id << "," << curr->data.name << "," << curr->data.location << "," << curr->data.phone << endl; curr = curr->next; } }

        {
            ofstream f("cars.txt");
            LinkedList<Showroom>::Node* rNode = showrooms.head;
            while (rNode) {
                LinkedList<Car>::Node* cNode = rNode->data.cars.head;
                while (cNode) {
                    f << rNode->data.id << "," << cNode->data.id << "," << cNode->data.make << "," << cNode->data.model << "," << cNode->data.year << ","
                        << cNode->data.price << "," << cNode->data.status << "," << cNode->data.rentPrice << "," << cNode->data.reservationStartTime << "," << cNode->data.reservedByCustomerID << endl;
                    cNode = cNode->next;
                }
                rNode = rNode->next;
            }
        }

        {
            ofstream f("services.txt");
            LinkedList<Garage>::Node* gNode = garages.head;
            while (gNode) {
                LinkedList<Service>::Node* sNode = gNode->data.services.head;
                while (sNode) {
                    f << gNode->data.id << "," << sNode->data.id << "," << sNode->data.name << "," << sNode->data.price << endl;
                    sNode = sNode->next;
                }
                gNode = gNode->next;
            }
        }

        {
            ofstream f("history_cars.txt");
            LinkedList<Customer>::Node* cNode = customers.head;
            while (cNode) {
                LinkedList<HistoryRecordCars>::Node* hNode = cNode->data.carHistory.head;
                while (hNode) {
                    f << cNode->data.id << "," << hNode->data.showroomID << "," << hNode->data.carID << "," << hNode->data.date << "," << hNode->data.type << "," << hNode->data.totalAmount << "," << hNode->data.installmentMonths << endl;
                    hNode = hNode->next;
                }
                cNode = cNode->next;
            }
        }
        {
            ofstream f("history_services.txt");
            LinkedList<Customer>::Node* cNode = customers.head;
            while (cNode) {
                LinkedList<HistoryRecordServices>::Node* hNode = cNode->data.serviceHistory.head;
                while (hNode) {
                    f << cNode->data.id << "," << hNode->data.garageID << "," << hNode->data.serviceID << "," << hNode->data.date << "," << hNode->data.dayBooked << "," << hNode->data.cost << endl;
                    hNode = hNode->next;
                }
                cNode = cNode->next;
            }
        }
    }

    void generateDummyData() {
        if (!admins.isEmpty()) return;

        Admin a; a.id = 1; a.username = "admin"; a.password = "123"; admins.append(a);

        string makes[] = { "BMW", "Mercedes", "Audi", "Toyota", "Honda", "Ford", "Tesla", "Nissan", "Ferrari", "Porsche" };
        string models[] = { "X5", "C-Class", "A4", "Camry", "Civic", "Mustang", "Model S", "Altima", "Spider", "911" };
        string locations[] = { "Cairo", "Alexandria", "Giza", "Luxor", "Aswan", "Mansoura", "Tanta", "Suez", "Port Said", "Fayoum" };
        string srvNames[] = { "Oil Change", "Tire Rotation", "Brake Check", "Engine Tuneup", "Car Wash", "Battery Replace" };

        int sellerIdCounter = 1; int custIdCounter = 1;

        for (int i = 1; i <= 10; i++) {
            Seller s; s.id = sellerIdCounter++; s.username = "seller" + to_string(i); s.password = "123";

            int srId = 1; LinkedList<Showroom>::Node* t = showrooms.head;
            while (t) { if (t->data.id >= srId) srId = t->data.id + 1; t = t->next; }

            Showroom sr; sr.id = srId; sr.name = makes[i - 1] + " Center"; sr.location = locations[i - 1]; sr.phone = "0100000" + to_string(i);

            for (int j = 0; j < 4; j++) {
                int cId = 1; LinkedList<Car>::Node* ct = sr.cars.head;
                while (ct) { if (ct->data.id >= cId) cId = ct->data.id + 1; ct = ct->next; }

                Car c(cId, makes[i - 1], models[i - 1], 2020 + j, 20000 + (j * 5000), 200 + (j * 50));
                sr.cars.append(c);
            }
            showrooms.append(sr); s.myShowroomID = sr.id;

            int grId = 1; LinkedList<Garage>::Node* gt = garages.head;
            while (gt) { if (gt->data.id >= grId) grId = gt->data.id + 1; gt = gt->next; }

            Garage g; g.id = grId; g.name = "Garage " + locations[i - 1]; g.location = locations[i - 1]; g.phone = "0120000" + to_string(i);
            for (int k = 0; k < 4; k++) {
                int srvId = 1; LinkedList<Service>::Node* st = g.services.head;
                while (st) { if (st->data.id >= srvId) srvId = st->data.id + 1; st = st->next; }
                g.services.append(Service(srvId, srvNames[k % 6], 50 + (k * 20)));
            }
            garages.append(g); s.myGarageID = g.id;
            sellers.append(s);
        }

        for (int i = 1; i <= 5; i++) {
            Customer c; c.id = custIdCounter++; c.username = "cust" + to_string(i); c.password = "123"; customers.append(c);
        }
        saveAll();
    }

    void loadAll() {
        string line;

        ifstream aFile("admins.txt");
        if (aFile.is_open()) {
            while (getline(aFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 3) { Admin a; a.id = stoi(d[0]); a.username = d[1]; a.password = d[2]; admins.append(a); }
            }
        }
        if (admins.isEmpty()) { generateDummyData(); return; }

        ifstream slFile("sellers.txt"); while (getline(slFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 5) { Seller s; s.id = stoi(d[0]); s.username = d[1]; s.password = d[2]; s.myShowroomID = stoi(d[3]); s.myGarageID = stoi(d[4]); sellers.append(s); } }
        ifstream cFile("customers.txt"); while (getline(cFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 3) { Customer c; c.id = stoi(d[0]); c.username = d[1]; c.password = d[2]; customers.append(c); } }

        ifstream shFile("showrooms.txt"); if (!shFile.is_open()) shFile.open("Showrooms.txt");
        while (getline(shFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 4) { Showroom s; s.id = stoi(d[0]); s.name = d[1]; s.location = d[2]; s.phone = d[3]; showrooms.append(s); } }

        ifstream gFile("garages.txt"); if (!gFile.is_open()) gFile.open("Garage.txt");
        while (getline(gFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 4) { Garage g; g.id = stoi(d[0]); g.name = d[1]; g.location = d[2]; g.phone = d[3]; garages.append(g); } }

        ifstream carFile("cars.txt"); if (!carFile.is_open()) carFile.open("Cars.txt");
        if (carFile.is_open()) {
            while (getline(carFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 9) {
                    int sId = stoi(d[0]);
                    int resCust = (d.size() >= 10) ? stoi(d[9]) : -1;
                    Car c(stoi(d[1]), d[2], d[3], stoi(d[4]), stod(d[5]), stod(d[7]), stoi(d[6]), (time_t)stoll(d[8]), resCust);
                    LinkedList<Showroom>::Node* curr = showrooms.head;
                    while (curr) { if (curr->data.id == sId) { curr->data.cars.append(c); break; } curr = curr->next; }
                }
            }
        }

        ifstream srvFile("services.txt"); if (!srvFile.is_open()) srvFile.open("Services.txt");
        if (srvFile.is_open()) { while (getline(srvFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 4) { int gId = stoi(d[0]); Service s(stoi(d[1]), d[2], stod(d[3])); LinkedList<Garage>::Node* curr = garages.head; while (curr) { if (curr->data.id == gId) { curr->data.services.append(s); break; } curr = curr->next; } } } }

        ifstream hcFile("history_cars.txt"); if (hcFile.is_open()) { while (getline(hcFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 7) { int cId = stoi(d[0]); LinkedList<Customer>::Node* curr = customers.head; while (curr) { if (curr->data.id == cId) { curr->data.carHistory.append(HistoryRecordCars(stoi(d[1]), stoi(d[2]), d[3], d[4], stod(d[5]), stoi(d[6]))); break; } curr = curr->next; } } } }
        ifstream hsFile("history_services.txt"); if (hsFile.is_open()) { while (getline(hsFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 6) { int cId = stoi(d[0]); LinkedList<Customer>::Node* curr = customers.head; while (curr) { if (curr->data.id == cId) { curr->data.serviceHistory.append(HistoryRecordServices(stoi(d[1]), stoi(d[2]), d[3], d[4], stod(d[5]))); break; } curr = curr->next; } } } }
    }
};