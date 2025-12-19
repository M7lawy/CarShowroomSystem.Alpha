#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "DataClasses.h"

using namespace std;

class Database {
public:
    vector<Showroom> showrooms;
    vector<Garage> garages;
    vector<Admin> admins;
    vector<Seller> sellers;
    vector<Customer> customers;

    vector<string> split(string s, char delimiter) {
        vector<string> tokens; string token; istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) tokens.push_back(token);
        return tokens;
    }

    Database() { loadAll(); }

    void saveAll() {
        { ofstream f("admins.txt"); for (const auto& a : admins) f << a.id << "," << a.username << "," << a.password << endl; }
        { ofstream f("sellers.txt"); for (const auto& s : sellers) f << s.id << "," << s.username << "," << s.password << "," << s.myShowroomID << "," << s.myGarageID << endl; }
        { ofstream f("customers.txt"); for (const auto& c : customers) f << c.id << "," << c.username << "," << c.password << endl; }
        { ofstream f("showrooms.txt"); for (const auto& r : showrooms) f << r.id << "," << r.name << "," << r.location << "," << r.phone << endl; }
        { ofstream f("garages.txt"); for (const auto& g : garages) f << g.id << "," << g.name << "," << g.location << "," << g.phone << endl; }
        {
            ofstream f("cars.txt");
            for (const auto& r : showrooms) {
                for (const auto& c : r.cars) {
                    f << r.id << "," << c.id << "," << c.make << "," << c.model << "," << c.year << ","
                        << c.price << "," << c.status << "," << c.rentPrice << "," << c.reservationStartTime << "," << c.reservedByCustomerID << endl;
                }
            }
        }
        {
            ofstream f("Services.txt");
            for (const auto& g : garages) {
                for (const auto& s : g.services) f << g.id << "," << s.id << "," << s.name << "," << s.price << endl;
            }
        }
        {
            ofstream f("history_cars.txt");
            for (const auto& c : customers) {
                for (const auto& h : c.carHistory) f << c.id << "," << h.showroomID << "," << h.carID << "," << h.date << "," << h.type << "," << h.totalAmount << "," << h.installmentMonths << endl;
            }
        }
        {
            ofstream f("history_services.txt");
            for (const auto& c : customers) {
                for (const auto& h : c.serviceHistory) f << c.id << "," << h.garageID << "," << h.serviceID << "," << h.date << "," << h.dayBooked << "," << h.cost << endl;
            }
        }
    }

    void generateDummyData() {
        if (!admins.empty()) return;

        Admin a; a.id = 1; a.username = "admin"; a.password = "123"; admins.push_back(a);

        string makes[] = { "BMW", "Mercedes", "Audi", "Toyota", "Honda", "Ford", "Tesla", "Nissan", "Ferrari", "Porsche" };
        string models[] = { "X5", "C-Class", "A4", "Camry", "Civic", "Mustang", "Model S", "Altima", "Spider", "911" };
        string locations[] = { "Cairo", "Alexandria", "Giza", "Luxor", "Aswan", "Mansoura", "Tanta", "Suez", "Port Said", "Fayoum" };
        string srvNames[] = { "Oil Change", "Tire Rotation", "Brake Check", "Engine Tuneup", "Car Wash", "Battery Replace" };

        // Safe ID Counters
        int sellerIdCounter = 1;
        int custIdCounter = 1;

        for (int i = 1; i <= 10; i++) {
            Seller s; s.id = sellerIdCounter++; s.username = "seller" + to_string(i); s.password = "123";

            // Showroom ID
            int srId = 1; for (auto& x : showrooms) if (x.id >= srId) srId = x.id + 1;
            Showroom sr; sr.id = srId; sr.name = makes[i - 1] + " Center"; sr.location = locations[i - 1]; sr.phone = "0100000" + to_string(i);

            // Car IDs (Local to showroom logic, but ensuring unique sequence)
            for (int j = 0; j < 4; j++) {
                int cId = 1; for (auto& car : sr.cars) if (car.id >= cId) cId = car.id + 1;
                Car c(cId, makes[i - 1], models[i - 1], 2020 + j, 20000 + (j * 5000), 200 + (j * 50));
                sr.cars.push_back(c);
            }
            showrooms.push_back(sr); s.myShowroomID = sr.id;

            // Garage ID
            int grId = 1; for (auto& x : garages) if (x.id >= grId) grId = x.id + 1;
            Garage g; g.id = grId; g.name = "Garage " + locations[i - 1]; g.location = locations[i - 1]; g.phone = "0120000" + to_string(i);

            for (int k = 0; k < 4; k++) {
                int srvId = 1; for (auto& sv : g.services) if (sv.id >= srvId) srvId = sv.id + 1;
                g.services.push_back(Service(srvId, srvNames[k % 6], 50 + (k * 20)));
            }
            garages.push_back(g); s.myGarageID = g.id;
            sellers.push_back(s);
        }

        for (int i = 1; i <= 5; i++) {
            Customer c; c.id = custIdCounter++; c.username = "cust" + to_string(i); c.password = "123"; customers.push_back(c);
        }
        saveAll();
    }

    void loadAll() {
        string line;
        ifstream aFile("admins.txt");
        if (aFile.is_open()) {
            while (getline(aFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 3) { Admin a; a.id = stoi(d[0]); a.username = d[1]; a.password = d[2]; admins.push_back(a); }
            }
        }
        if (admins.empty()) { generateDummyData(); return; }

        ifstream slFile("sellers.txt"); while (getline(slFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 5) { Seller s; s.id = stoi(d[0]); s.username = d[1]; s.password = d[2]; s.myShowroomID = stoi(d[3]); s.myGarageID = stoi(d[4]); sellers.push_back(s); } }
        ifstream cFile("customers.txt"); while (getline(cFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 3) { Customer c; c.id = stoi(d[0]); c.username = d[1]; c.password = d[2]; customers.push_back(c); } }
        ifstream shFile("showrooms.txt"); while (getline(shFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 4) { Showroom s; s.id = stoi(d[0]); s.name = d[1]; s.location = d[2]; s.phone = d[3]; showrooms.push_back(s); } }
        ifstream gFile("Garage.txt"); while (getline(gFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 4) { Garage g; g.id = stoi(d[0]); g.name = d[1]; g.location = d[2]; g.phone = d[3]; garages.push_back(g); } }

        ifstream carFile("cars.txt");
        if (carFile.is_open()) {
            while (getline(carFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 9) {
                    int sId = stoi(d[0]);
                    int resCust = (d.size() >= 10) ? stoi(d[9]) : -1;
                    Car c(stoi(d[1]), d[2], d[3], stoi(d[4]), stod(d[5]), stod(d[7]), stoi(d[6]), (time_t)stoll(d[8]), resCust);
                    for (auto& r : showrooms) if (r.id == sId) r.cars.push_back(c);
                }
            }
        }

        ifstream srvFile("Services.txt"); if (srvFile.is_open()) { while (getline(srvFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 4) { int gId = stoi(d[0]); Service s(stoi(d[1]), d[2], stod(d[3])); for (auto& g : garages) if (g.id == gId) g.services.push_back(s); } } }
        ifstream hcFile("history_cars.txt"); if (hcFile.is_open()) { while (getline(hcFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 7) { int cId = stoi(d[0]); for (auto& cust : customers) if (cust.id == cId) cust.carHistory.push_back(HistoryRecordCars(stoi(d[1]), stoi(d[2]), d[3], d[4], stod(d[5]), stoi(d[6]))); } } }
        ifstream hsFile("history_services.txt"); if (hsFile.is_open()) { while (getline(hsFile, line)) { vector<string> d = split(line, ','); if (d.size() >= 6) { int cId = stoi(d[0]); for (auto& cust : customers) if (cust.id == cId) cust.serviceHistory.push_back(HistoryRecordServices(stoi(d[1]), stoi(d[2]), d[3], d[4], stod(d[5]))); } } }
    }
};