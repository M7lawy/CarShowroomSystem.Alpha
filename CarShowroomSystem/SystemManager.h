#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream> // Required for file handling
#include <sstream> // Required for splitting strings
#include "DataClasses.h" // Include our data structures

using namespace std;

class SystemManager {
private:
    // These vectors are our "Database" in memory
    vector<Showroom> showrooms;
    vector<Garage> garages;
    vector<Admin> admins;
    vector<Customer> customers;

    // Helper to split a string by a delimiter (like comma)
    vector<string> split(string s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    // Constructor: Let's add a dummy admin so you can log in immediately
    SystemManager() {

        // Load existing data first
        loadData();

        // Ensure Admin exists (simple check)
        Admin root;
        root.id = 1;
        root.username = "admin";
        root.password = "123";
        admins.push_back(root);
    }

    // --- ADMIN FUNCTIONS ---

    bool loginAdmin(string user, string pass) {
        for (Admin a : admins) {
            if (a.username == user && a.password == pass) {
                return true;
            }
        }
        return false;
    }

    void addShowroom() {
        Showroom s;
        // Auto-generate ID based on list size
        s.id = showrooms.size() + 1;

        cout << "--- New Showroom ---\n";
        cout << "Enter Name: ";
        cin.ignore(); // Clear buffer to handle spaces
        getline(cin, s.name);
        cout << "Enter Location: ";
        getline(cin, s.location);
        cout << "Enter Phone: ";
        cin >> s.phone;

        showrooms.push_back(s);
        cout << "Showroom Added Successfully! ID: " << s.id << endl;
    }

    void addCarToShowroom() {
        if (showrooms.empty()) {
            cout << "No showrooms available! Create one first.\n";
            return;
        }

        int sId;
        cout << "Enter Showroom ID to add car to: ";
        cin >> sId;

        bool found = false;
        // Loop through showrooms by REFERENCE (&) so we modify the original, not a copy
        for (auto& room : showrooms) {
            if (room.id == sId) {
                int cId = room.cars.size() + 1;
                string mk, md;
                int yr;
                double pr;

                cout << "Enter Make (e.g. BMW): "; cin >> mk;
                cout << "Enter Model (e.g. X5): "; cin >> md;
                cout << "Enter Year: "; cin >> yr;
                cout << "Enter Price: "; cin >> pr;

                Car c(cId, mk, md, yr, pr);
                room.cars.push_back(c);

                cout << "Car added to " << room.name << " successfully!\n";
                found = true;
                break;
            }
        }

        if (!found) cout << "Showroom ID not found.\n";
    }

    void viewAllData() {
        cout << "\n=== CURRENT SYSTEM DATA ===\n";
        for (const auto& room : showrooms) {
            cout << "Showroom: " << room.name << " (" << room.location << ")\n";
            if (room.cars.empty()) {
                cout << "   -> No cars yet.\n";
            }
            else {
                for (const auto& c : room.cars) {
                    cout << "   -> Car: " << c.make << " " << c.model << " [$" << c.price << "]\n";
                }
            }
        }
        cout << "===========================\n";
    }

    // --- CUSTOMER FUNCTIONS ---

    void registerCustomer() {
        Customer c;
        c.id = customers.size() + 1;
        cout << "\n--- Customer Registration ---\n";
        cout << "Enter Username: "; cin >> c.username;
        cout << "Enter Password: "; cin >> c.password;

        customers.push_back(c);
        cout << "Registration successful! You can now login.\n";
    }

    // Returns the Customer ID if successful, or -1 if failed
    int loginCustomer(string user, string pass) {
        for (const auto& c : customers) {
            if (c.username == user && c.password == pass) {
                return c.id;
            }
        }
        return -1;
    }

    void searchCars() {
        string query;
        cout << "Enter Car Make or Model to search (e.g. BMW): ";
        cin >> query;

        cout << "\n--- Search Results ---\n";
        bool foundAny = false;

        for (const auto& room : showrooms) {
            for (const auto& c : room.cars) {
                // Check if make/model matches AND if car is not already rented
                if ((c.make == query || c.model == query) && !c.isRented) {
                    cout << "Found in " << room.name << ": "
                        << c.make << " " << c.model << " | Year: " << c.year
                        << " | Price: $" << c.price << " | Car ID: " << c.id << endl;
                    foundAny = true;
                }
            }
        }
        if (!foundAny) cout << "No available cars found matching '" << query << "'.\n";
    }

    void buyOrRentCar(int customerID) {
        int sId, cId;
        cout << "Enter Showroom ID: "; cin >> sId;
        cout << "Enter Car ID: "; cin >> cId;

        bool found = false;
        // Note: Using '&' reference to update the actual car status
        for (auto& room : showrooms) {
            if (room.id == sId) {
                for (auto& c : room.cars) {
                    if (c.id == cId) {
                        if (c.isRented) {
                            cout << "Sorry, this car is already taken.\n";
                        }
                        else {
                            c.isRented = true; // Mark as taken
                            cout << "Congratulations! You have successfully processed Car #" << cId << endl;
                            // (Optional) You could save this transaction to a 'Process' vector here
                        }
                        found = true;
                        break;
                    }
                }
            }
            if (found) break;
        }
        if (!found) cout << "Invalid Showroom or Car ID.\n";
    }

        void saveData() {
            // 1. Save Showrooms
            ofstream sFile("showrooms.txt");
            for (const auto& room : showrooms) {
                sFile << room.id << "," << room.name << "," << room.location << "," << room.phone << endl;
            }
            sFile.close();

            // 2. Save Cars (We need to link them to their Showroom ID)
            ofstream cFile("cars.txt");
            for (const auto& room : showrooms) {
                for (const auto& c : room.cars) {
                    // Format: ShowroomID, CarID, Make, Model, Year, Price, IsRented
                    cFile << room.id << "," << c.id << "," << c.make << ","
                        << c.model << "," << c.year << "," << c.price << "," << c.isRented << endl;
                }
            }
            cFile.close();

            // 3. Save Customers
            ofstream uFile("customers.txt");
            for (const auto& cust : customers) {
                uFile << cust.id << "," << cust.username << "," << cust.password << endl;
            }
            uFile.close();

            cout << "Data saved successfully to text files.\n";
        }

        void loadData() {
            string line;

            // 1. Load Showrooms
            ifstream sFile("showrooms.txt");
            if (sFile.is_open()) {
                while (getline(sFile, line)) {
                    vector<string> data = split(line, ',');
                    if (data.size() >= 4) {
                        Showroom s;
                        s.id = stoi(data[0]);
                        s.name = data[1];
                        s.location = data[2];
                        s.phone = data[3];
                        showrooms.push_back(s);
                    }
                }
                sFile.close();
            }

            // 2. Load Cars
            ifstream cFile("cars.txt");
            if (cFile.is_open()) {
                while (getline(cFile, line)) {
                    vector<string> data = split(line, ',');
                    if (data.size() >= 7) {
                        int sId = stoi(data[0]); // The showroom this car belongs to

                        Car c(stoi(data[1]), data[2], data[3], stoi(data[4]), stod(data[5]));
                        c.isRented = stoi(data[6]); // 0 or 1

                        // Find the correct showroom and add the car
                        for (auto& room : showrooms) {
                            if (room.id == sId) {
                                room.cars.push_back(c);
                                break;
                            }
                        }
                    }
                }
                cFile.close();
            }

            // 3. Load Customers
            ifstream uFile("customers.txt");
            if (uFile.is_open()) {
                while (getline(uFile, line)) {
                    vector<string> data = split(line, ',');
                    if (data.size() >= 3) {
                        Customer cust;
                        cust.id = stoi(data[0]);
                        cust.username = data[1];
                        cust.password = data[2];
                        customers.push_back(cust);
                    }
                }
                uFile.close();
            }
        }
};