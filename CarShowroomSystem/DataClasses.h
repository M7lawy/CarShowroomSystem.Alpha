#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include "LinkedList.h" 

using namespace std;

class HistoryRecordCars {
public:
    int showroomID, carID;
    string date, type;
    double totalAmount;
    int installmentMonths;

    HistoryRecordCars(int sId, int cId, string d, string t, double amt, int months = 0) {
        showroomID = sId; carID = cId; date = d; type = t; totalAmount = amt; installmentMonths = months;
    }
    HistoryRecordCars() {}
};

class HistoryRecordServices {
public:
    int garageID, serviceID;
    string date, dayBooked;
    double cost;

    HistoryRecordServices(int gId, int sId, string d, string day, double c) {
        garageID = gId; serviceID = sId; date = d; dayBooked = day; cost = c;
    }
    HistoryRecordServices() {}
};

class Car {
public:
    int id;
    string make, model;
    int year;
    double price, rentPrice;
    int status;
    time_t reservationStartTime;
    int reservedByCustomerID;

    Car() {}
    Car(int id, string mk, string md, int yr, double pr, double rPr, int st = 0, time_t resTime = 0, int resCustId = -1) {
        this->id = id; this->make = mk; this->model = md; this->year = yr;
        this->price = pr; this->rentPrice = rPr; this->status = st;
        this->reservationStartTime = resTime;
        this->reservedByCustomerID = resCustId;
    }
};

class Service {
public:
    int id;
    string name;
    double price;
    Service() {}
    Service(int id, string n, double p) { this->id = id; this->name = n; this->price = p; }
};

class Showroom {
public:
    int id;
    string name, location, phone;
    LinkedList<Car> cars;
};

class Garage {
public:
    int id;
    string name, location, phone;
    LinkedList<Service> services;
};

class Seller {
public:
    int id;
    string username, password;
    int myShowroomID, myGarageID;
    Seller() { id = 0; myShowroomID = -1; myGarageID = -1; }
};

class Admin {
public:
    int id;
    string username, password;
};

class Customer {
public:
    int id;
    string username, password;
    LinkedList<HistoryRecordCars> carHistory;
    LinkedList<HistoryRecordServices> serviceHistory;
};