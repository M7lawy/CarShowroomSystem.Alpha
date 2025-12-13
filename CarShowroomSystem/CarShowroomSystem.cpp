#include <iostream>
#include "SystemManager.h" 

using namespace std;

int main() {
    SystemManager sys;
    int choice;

    while (true) {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as Customer\n";
        cout << "3. Register New Customer\n";
        cout << "4. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        // --- ADMIN SECTION ---
        if (choice == 1) {
            string u, p;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> p;

            if (sys.loginAdmin(u, p)) {
                while (true) {
                    int adminChoice;
                    cout << "\n--- ADMIN MENU ---\n";
                    cout << "1. Add Showroom\n2. Add Car\n3. View All Data\n4. Logout\nChoice: ";
                    cin >> adminChoice;

                    if (adminChoice == 1) sys.addShowroom();
                    else if (adminChoice == 2) sys.addCarToShowroom();
                    else if (adminChoice == 3) sys.viewAllData();
                    else if (adminChoice == 4) break;
                }
            }
            else {
                cout << "Invalid Admin Credentials.\n";
            }
        }
        // --- CUSTOMER SECTION ---
        else if (choice == 2) {
            string u, p;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> p;

            int currentCustomerID = sys.loginCustomer(u, p);

            if (currentCustomerID != -1) {
                while (true) {
                    int custChoice;
                    cout << "\n--- CUSTOMER MENU ---\n";
                    cout << "1. View All Showrooms\n2. Search Cars\n3. Buy/Rent Car\n4. Logout\nChoice: ";
                    cin >> custChoice;

                    if (custChoice == 1) sys.viewAllData();
                    else if (custChoice == 2) sys.searchCars();
                    else if (custChoice == 3) sys.buyOrRentCar(currentCustomerID);
                    else if (custChoice == 4) break;
                }
            }
            else {
                cout << "Invalid Username or Password. Did you register?\n";
            }
        }
        // --- REGISTRATION ---
        else if (choice == 3) {
            sys.registerCustomer();
        }
        // --- EXIT ---
        else if (choice == 4) {
            sys.saveData(); // <--- ADD THIS LINE
            break;
        }
    }
    return 0;
}