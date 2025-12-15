#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime> // Required for time tracking

using namespace std;

// ==========================================
//             HISTORY RECORDS
// ==========================================

class HistoryRecordCars {
public:
    int showroomID;
    int carID;
    string date;
    string type;           // "BUY", "RENT", "INSTALLMENT"
    double totalAmount;
    int installmentMonths; // 0 if cash/rent, otherwise 3, 6, 12...

    HistoryRecordCars(int sId, int cId, string d, string t, double amt, int months = 0) {
        showroomID = sId;
        carID = cId;
        date = d;
        type = t;
        totalAmount = amt;
        installmentMonths = months;
    }
};

class HistoryRecordServices {
public:
    int garageID;
    int serviceID;
    string date;      // Transaction Timestamp
    string dayBooked; // Appointment Day (e.g. "Monday")
    double cost;

    HistoryRecordServices(int gId, int sId, string d, string day, double c) {
        garageID = gId;
        serviceID = sId;
        date = d;
        dayBooked = day;
        cost = c;
    }
};

// ==========================================
//               ENTITIES
// ==========================================

class Car {
public:
    int id;
    string make;
    string model;
    int year;
    double price;
    double rentPrice;

    // Status: 0 = Available, 1 = Reserved, 2 = Sold/Rented
    int status;

    // Time when reservation started (for the 60-second timer)
    time_t reservationStartTime;

    Car(int id, string mk, string md, int yr, double pr, double rPr, int st = 0, time_t resTime = 0) {
        this->id = id;
        this->make = mk;
        this->model = md;
        this->year = yr;
        this->price = pr;
        this->rentPrice = rPr;
        this->status = st;
        this->reservationStartTime = resTime;
    }
};

class Service {
public:
    int id;
    string name;
    double price;

    Service(int id, string n, double p) {
        this->id = id;
        this->name = n;
        this->price = p;
    }
};

class Showroom {
public:
    int id;
    string name;
    string location;
    string phone;
    vector<Car> cars;
};

class Garage {
public:
    int id;
    string name;
    string location;
    string phone;
    vector<Service> services;
};

// ==========================================
//                 USERS
// ==========================================

class Seller {
public:
    int id;
    string username;
    string password;

    // -1 means they haven't created one yet
    int myShowroomID;
    int myGarageID;

    Seller() {
        id = 0; myShowroomID = -1; myGarageID = -1;
    }
};

class Admin {
public:
    int id;
    string username;
    string password;
};

class Customer {
public:
    int id;
    string username;
    string password;

    // Transaction History
    vector<HistoryRecordCars> carHistory;
    vector<HistoryRecordServices> serviceHistory;
};