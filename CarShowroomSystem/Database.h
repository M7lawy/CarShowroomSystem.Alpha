#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "DataClasses.h"

using namespace std;

class Database {
public:
    // --- THE DATA ---
    vector<Showroom> showrooms;
    vector<Garage> garages;
    vector<Admin> admins;
    vector<Seller> sellers;
    vector<Customer> customers;

    // --- HELPER ---
    vector<string> split(string s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) tokens.push_back(token);
        return tokens;
    }

    // --- CONSTRUCTOR (LOADS DATA) ---
    Database() {
        loadAll();
        // Default Admin if empty
        if (admins.empty()) {
            Admin root; root.id = 1; root.username = "admin"; root.password = "123";
            admins.push_back(root);
        }
    }

    // --- SAVE FUNCTION ---
    void saveAll() {
        ofstream aFile("admins.txt");
        for (const auto& a : admins) aFile << a.id << "," << a.username << "," << a.password << endl;
        aFile.close();

        ofstream slFile("sellers.txt");
        for (const auto& s : sellers) slFile << s.id << "," << s.username << "," << s.password << "," << s.myShowroomID << "," << s.myGarageID << endl;
        slFile.close();

        ofstream cFile("customers.txt");
        ofstream hCarFile("history_cars.txt");
        ofstream hSrvFile("history_services.txt");
        for (const auto& c : customers) {
            cFile << c.id << "," << c.username << "," << c.password << endl;
            for (const auto& h : c.carHistory) hCarFile << c.id << "," << h.showroomID << "," << h.carID << "," << h.date << "," << h.type << "," << h.totalAmount << "," << h.installmentMonths << endl;
            for (const auto& h : c.serviceHistory) hSrvFile << c.id << "," << h.garageID << "," << h.serviceID << "," << h.date << "," << h.dayBooked << "," << h.cost << endl;
        }
        cFile.close(); hCarFile.close(); hSrvFile.close();

        ofstream shFile("showrooms.txt");
        ofstream carFile("cars.txt");
        for (const auto& r : showrooms) {
            shFile << r.id << "," << r.name << "," << r.location << "," << r.phone << endl;
            for (const auto& c : r.cars) carFile << r.id << "," << c.id << "," << c.make << "," << c.model << "," << c.year << "," << c.price << "," << c.status << "," << c.rentPrice << "," << c.reservationStartTime << endl;
        }
        shFile.close(); carFile.close();

        ofstream gFile("Garage.txt");
        ofstream srvFile("Services.txt");
        for (const auto& g : garages) {
            gFile << g.id << "," << g.name << "," << g.location << "," << g.phone << endl;
            for (const auto& s : g.services) srvFile << g.id << "," << s.id << "," << s.name << "," << s.price << endl;
        }
        gFile.close(); srvFile.close();
        cout << "Data Saved.\n";
    }

    // --- LOAD FUNCTION ---
    void loadAll() {
        string line;
        ifstream aFile("admins.txt");
        while (getline(aFile, line)) {
            vector<string> d = split(line, ',');
            if (d.size() >= 3) { Admin a; a.id = stoi(d[0]); a.username = d[1]; a.password = d[2]; admins.push_back(a); }
        }

        ifstream slFile("sellers.txt");
        while (getline(slFile, line)) {
            vector<string> d = split(line, ',');
            if (d.size() >= 5) { Seller s; s.id = stoi(d[0]); s.username = d[1]; s.password = d[2]; s.myShowroomID = stoi(d[3]); s.myGarageID = stoi(d[4]); sellers.push_back(s); }
        }

        ifstream cFile("customers.txt");
        while (getline(cFile, line)) {
            vector<string> d = split(line, ',');
            if (d.size() >= 3) { Customer c; c.id = stoi(d[0]); c.username = d[1]; c.password = d[2]; customers.push_back(c); }
        }

        ifstream shFile("showrooms.txt");
        while (getline(shFile, line)) {
            vector<string> d = split(line, ',');
            if (d.size() >= 4) { Showroom s; s.id = stoi(d[0]); s.name = d[1]; s.location = d[2]; s.phone = d[3]; showrooms.push_back(s); }
        }

        ifstream gFile("Garage.txt");
        while (getline(gFile, line)) {
            vector<string> d = split(line, ',');
            if (d.size() >= 4) { Garage g; g.id = stoi(d[0]); g.name = d[1]; g.location = d[2]; g.phone = d[3]; garages.push_back(g); }
        }

        // Link Cars
        ifstream carFile("cars.txt");
        if (carFile.is_open()) {
            while (getline(carFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 9) {
                    int sId = stoi(d[0]);
                    Car c(stoi(d[1]), d[2], d[3], stoi(d[4]), stod(d[5]), stod(d[7]), stoi(d[6]), (time_t)stoll(d[8]));
                    for (auto& r : showrooms) if (r.id == sId) r.cars.push_back(c);
                }
            }
        }

        // Link Services
        ifstream srvFile("Services.txt");
        if (srvFile.is_open()) {
            while (getline(srvFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 4) {
                    int gId = stoi(d[0]);
                    Service s(stoi(d[1]), d[2], stod(d[3]));
                    for (auto& g : garages) if (g.id == gId) g.services.push_back(s);
                }
            }
        }

        // Link History
        ifstream hcFile("history_cars.txt");
        if (hcFile.is_open()) {
            while (getline(hcFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 7) {
                    int cId = stoi(d[0]);
                    for (auto& cust : customers) if (cust.id == cId) cust.carHistory.push_back(HistoryRecordCars(stoi(d[1]), stoi(d[2]), d[3], d[4], stod(d[5]), stoi(d[6])));
                }
            }
        }
        ifstream hsFile("history_services.txt");
        if (hsFile.is_open()) {
            while (getline(hsFile, line)) {
                vector<string> d = split(line, ',');
                if (d.size() >= 6) {
                    int cId = stoi(d[0]);
                    for (auto& cust : customers) if (cust.id == cId) cust.serviceHistory.push_back(HistoryRecordServices(stoi(d[1]), stoi(d[2]), d[3], d[4], stod(d[5])));
                }
            }
        }
    }
};