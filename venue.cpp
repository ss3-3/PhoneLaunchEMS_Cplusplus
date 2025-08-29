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

// Venue management functions
bool isVenueAvailable(const Venue& venue, const Date& date, const string& time) {
    for (const auto& slot : venue.bookingSchedule) {
        if (slot.date.toString() == date.toString() && slot.time == time && slot.isBooked) {
            return false;
        }
    }
    return true;
}

void displayAvailableVenues(const vector<Venue>& venues, const Date& date, const string& time) {
    cout << "\n=== AVAILABLE VENUES FOR " << date.toString() << " AT " << time << " ===" << endl;
    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;
    cout << left
        << setw(4) << "No."
        << setw(15) << "Venue Name"
        << setw(30) << "Address"
        << setw(10) << "Capacity"
        << setw(15) << "Cost (RM)"
        << setw(20) << "Contact Person" << endl;
    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;

    int displayIndex = 1;
    bool anyAvailable = false;

    for (const auto& venue : venues) {
        if (isVenueAvailable(venue, date, time)) {
            anyAvailable = true;
            cout << left
                << setw(4) << displayIndex
                << setw(15) << venue.venueName
                << setw(30) << venue.address.substr(0, 29)
                << setw(10) << venue.capacity
                << "RM " << setw(12) << fixed << setprecision(2) << venue.rentalCost
                << setw(20) << venue.contactPerson << endl;
            displayIndex++;
        }
    }

    if (!anyAvailable) {
        cout << "No venues available for the specified date and time." << endl;
    }

    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;
}

int selectAvailableVenue(const vector<Venue>& venues, const Date& date, const string& time) {
    vector<int> availableIndices;

    // Build list of available venue indices using for loop
    for (size_t i = 0; i < venues.size(); i++) {
        if (isVenueAvailable(venues[i], date, time)) {
            availableIndices.push_back(i);
        }
    }

    if (availableIndices.empty()) {
        return -1; // No venues available
    }

    int choice = getValidIntegerInput("Select venue number: ", 1, availableIndices.size());
    return availableIndices[choice - 1];
}

EventRegistration* findRegistrationByID(vector<EventRegistration>& registrations, const string& eventID) {
    for (size_t i = 0; i < registrations.size(); i++) {
        if (registrations[i].eventID == eventID) {
            return &registrations[i];
        }
    }
    return nullptr;
}