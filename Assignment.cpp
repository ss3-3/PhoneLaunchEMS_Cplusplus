#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <regex>
#include "date.h"
#include "venue.h"
#include "event_registration.h"
#include "event_booking.h"
#include "system_data.h"
#include "timeslot.h"
#include "product.h"
#include "utility_fun.h"
#include "input_validation.h"
#include "file_operation.h"
using namespace std;

// Main function
int main() {
    SystemData data;
    loadDataFromFiles(data);

    bool exitProgram = false;
    while (!exitProgram) {
        displayLogo();
        cout << "=== PHONE EVENT MANAGEMENT SYSTEM ===" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;
        cout << "1. Event Registration" << endl;
        cout << "2. Event Booking on date, venue and logistics" << endl;
        cout << "3. Event Monotoring" << endl;
        cout << "4. Event Payment" << endl;
        cout << "5. Save Data & Exit" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

        int choice = getValidIntegerInput("Enter your choice [1-5]: ", 1, 5);

        switch (choice) {
        case 1:
            eventRegistrationMenu(data);
            break;
        case 2:
            eventBookingMenu(data);
            break;
        case 3:
            //eventMonitoring;
            break;
        case 4:
            //eventPayment;
            break;
        case 5:
            saveDataToFiles(data);
            exitProgram = true;
            cout << "Thank you for using the Phone Event Management System!" << endl;
            cout << "All data has been saved successfully." << endl;
            break;
        }
    }

    return 0;
}
