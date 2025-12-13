#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//   Car Class
class Car {
public:
	int id;
	string make;
	string model;
	int year;
	double price;

	bool isRented;

	Car(int id, string mk, string md, int yr, double pr) {

		this->id = id;
		this->make = mk;
		this->model = md;
		this->year = yr;
		this->price = pr;
		this->isRented = false;


	}
};

//  SHOWROOM Class
class Showroom {
public:
    int id;
    string name;
    string location;
    string phone;

    // A list of cars inside this showroom
    vector<Car> cars;
};

//   SERVICE Class (For Garages)
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

//  GARAGE Class
class Garage {
public:
    int id;
    string name;
    string location;
    string phone;

    // A list of services this garage offers
    vector<Service> services;
};

//  USER Classes (Admin & Customer)
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
};
