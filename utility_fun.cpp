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

// Display system logo
void displayLogo() {
    clearScreen();
    cout << endl;
    cout << "	.----------------.  .----------------.  .----------------. " << endl;
    cout << "	| .--------------. || .--------------. || .--------------. | " << endl;
    cout << "	| |     ____     | || |  _________   | || | ____    ____ | | " << endl;
    cout << "	| |   .'    `.   | || | |_   ___  |  | || ||_   \\  /   _|| | " << endl;
    cout << "	| |  /  .--.  \\  | || |   | |_  \\_|  | || |  |   \\/   |  | | " << endl;
    cout << "	| |  | |    | |  | || |   |  _ | _   | || |  | |\\  /| |  | | " << endl;
    cout << "	| |  \\  `- - '/  | || |  _| |___/ |  | || | _| |_\\/_| |_ | | " << endl;
    cout << "	| |   `.____.'   | || | |_________|  | || ||_____||_____|| | " << endl;
    cout << "	| |              | || |              | || |              | | " << endl;
    cout << "	| '--------------' || '--------------' || '--------------' | " << endl;
    cout << "	'----------------'  '----------------'  '----------------' " << endl;
    cout << endl;

    cout << "	______ _                         _____                _ " << endl;
    cout << "	| ___ \\ |                       |  ___|              | | " << endl;
    cout << "	| |_/ / |__   ___  _ __   ___   | |____   _____ _ __ | |_ " << endl;
    cout << "	| __/ | '_ \\ / _ \\| '_ \\ / _ \\  |  __\\ \\ / / _ \\ '_ \\| __|                                                " << endl;
    cout << "	| |   | | | | (_) | | | |  __/  | |___\\ V /  __/ | | | |_ " << endl;
    cout << "	\\_|   |_| |_|\\___/|_| |_|\\___|  \\____/ \\_/ \\___|_| |_|\\__| " << endl;


    cout << "	___  ___                                                  _     _____           _ " << endl;
    cout << "	|  \\/  |                                                 | |   /  ___|         | | " << endl;
    cout << "	| .  . | __ _ _ __   __ _  __ _  ___ _ __ ___   ___ _ __ | |_  \\ `-- ._   _ ___| |_ ___ _ __ ___ " << endl;
    cout << "	| |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __|  `__. \\ | | / __| __/ _ \\ '_ ` _ \\ " << endl;
    cout << "	| |  | | (_| | | | | (_| | (_| |  __/ | | | | |  __/ | | | |_  /\\__/ / |_| \\__ \\ | | __/ | | | | | " << endl;
    cout << "	\\_|  |_/\\__,_|_| |_|\\__,_|\\__, |\\___|_| |_| |_|\\___|_| |_|\\__| \\____/\\__, |___ /\\__\\___|_| |_| |_| " << endl;
    cout << "                                   __/ |                                      __/ | " << endl;
    cout << "				  |___/                                      |___/ " << endl;
    cout << endl;
}

// Clear screen function
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Date getCurrentDate() {
    // For demonstration purposes - in real application, use system date
    Date current;
    current.year = 2025;
    current.month = 9;
    current.day = 20;
    return current;
}

string generateEventID(const vector<EventRegistration>& registrations) {
    int nextNumber = registrations.size() + 1001;
    return "EVT" + to_string(nextNumber);
}

string generateBookingID(const vector<EventBooking>& bookings) {
    int nextNumber = bookings.size() + 2001;
    return "BKG" + to_string(nextNumber);
}

string generateVenueID(const vector<Venue>& venues) {
    int nextNumber = venues.size() + 1;
    string id = "V" + string(3 - to_string(nextNumber).length(), '0') + to_string(nextNumber);
    return id;
}

string formatDouble(double value, int precision) {
    ostringstream out;
    out << fixed << setprecision(precision) << value;
    return out.str();
}

// Pause screen for user interaction
void pauseScreen() {
    cout << "\nPress any key to continue...";
    system("pause");
}