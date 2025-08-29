#include "file_operation.h"
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

// File operations implementation
void saveDataToFiles(const SystemData& data) {
    saveVenuesToFile(data.venues);
    saveRegistrationsToFile(data.registrations);
    saveBookingsToFile(data.bookings);
    cout << "Data saved successfully to files." << endl;
}

void loadDataFromFiles(SystemData& data) {
    loadVenuesFromFile(data.venues);
    loadRegistrationsFromFile(data.registrations);
    loadBookingsFromFile(data.bookings);
}

void saveVenuesToFile(const vector<Venue>& venues) {
    ofstream file("venues.txt");
    if (file.is_open()) {
        for (const auto& venue : venues) {
            file << venue.venueID << "|"
                << venue.venueName << "|"
                << venue.address << "|"
                << venue.capacity << "|"
                << venue.rentalCost << "|"
                << venue.contactPerson << "|"
                << venue.phoneNumber << endl;

            // Save booking schedule
            for (const auto& slot : venue.bookingSchedule) {
                file << "SLOT|" << slot.date.toString() << "|"
                    << slot.time << "|" << slot.eventID << "|"
                    << (slot.isBooked ? "1" : "0") << endl;
            }
            file << "END_VENUE" << endl;
        }
        file.close();
    }
}

void saveRegistrationsToFile(const vector<EventRegistration>& registrations) {
    ofstream file("registrations.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open file for saving! Kindly try again!" << endl;
        return;
    }
    else
    {
        string phoneData = "";
        for (const auto& reg : registrations) {
            for (size_t i = 0; i < reg.productQuantity; ++i)
            {
                phoneData += reg.phoneInfo[i].productName + "," +
                    reg.phoneInfo[i].productModel + "," +
                    to_string(reg.phoneInfo[i].productPrice); //double must change to string to avoid error
                if (i != reg.phoneInfo.size() - 1)
                {
                    phoneData += ";"; //add ; to  divide the different product info
                }
            }
            file << reg.eventID << "|"
                << reg.eventTitle << "|"
                << reg.productQuantity << "|"
                << phoneData.c_str() << "|" // retreive the phone data use const char * 
                << reg.manufacturer << "|"
                << reg.description << "|"
                << reg.expectedGuests << "|"
                << reg.estimatedBudget << "|"
                << reg.eventStatus << "|"
                << reg.organizerName << "|"
                << reg.organizerContact << "|"
                << reg.organizerEmail << "|"
                << reg.position
                << endl;
        }
        file.close(); //send the file close request to system
    }
}

void saveBookingsToFile(const vector<EventBooking>& bookings) {
    ofstream file("bookings.txt");
    if (file.is_open()) {
        for (const auto& booking : bookings) {
            file << booking.bookingID << "|"
                << booking.eventReg.eventID << "|"
                << booking.eventReg.eventTitle << "|"
                << booking.eventReg.manufacturer << "|"
                << booking.eventReg.description << "|"
                << booking.eventReg.expectedGuests << "|"
                << booking.eventReg.estimatedBudget << "|"
                << booking.eventReg.eventStatus << "|"
                << booking.eventReg.organizerName << "|"
                << booking.eventReg.organizerContact << "|"
                << booking.eventDate.toString() << "|"
                << booking.eventTime << "|"
                << booking.venue.venueID << "|"
                << booking.venue.venueName << "|"
                << booking.venue.address << "|"
                << booking.venue.capacity << "|"
                << booking.venue.rentalCost << "|"
                << booking.venue.contactPerson << "|"
                << booking.venue.phoneNumber << "|"
                << booking.bookingStatus << "|"
                << booking.finalCost << endl;
        }
        file.close();
    }
}

void loadVenuesFromFile(vector<Venue>& venues) {
    ifstream file("venues.txt");
    string line;

    if (file.is_open()) {
        venues.clear(); // Clear existing data

        while (getline(file, line)) {
            if (line == "END_VENUE") continue;

            if (line.substr(0, 5) == "SLOT|") {
                // Handle booking slots for the last venue
                if (!venues.empty()) {
                    stringstream ss(line.substr(5));
                    string token;
                    TimeSlot slot;

                    getline(ss, token, '|'); // date
                    // Parse date
                    stringstream dateStream(token);
                    string datePart;
                    getline(dateStream, datePart, '-');
                    slot.date.year = stoi(datePart);
                    getline(dateStream, datePart, '-');
                    slot.date.month = stoi(datePart);
                    getline(dateStream, datePart);
                    slot.date.day = stoi(datePart);

                    getline(ss, slot.time, '|');
                    getline(ss, slot.eventID, '|');
                    getline(ss, token, '|');
                    slot.isBooked = (token == "1");

                    venues.back().bookingSchedule.push_back(slot);
                }
            }
            else {
                // Handle venue data
                stringstream ss(line);
                string token;
                Venue venue;

                getline(ss, venue.venueID, '|');
                getline(ss, venue.venueName, '|');
                getline(ss, venue.address, '|');
                getline(ss, token, '|');
                venue.capacity = stoi(token);
                getline(ss, token, '|');
                venue.rentalCost = stod(token);
                getline(ss, venue.contactPerson, '|');
                getline(ss, venue.phoneNumber, '|');

                venues.push_back(venue);
            }
        }
        file.close();
    }
}

void loadRegistrationsFromFile(vector<EventRegistration>& registrations) {
    ifstream file("registrations.txt");
    string line;

    if (!file.is_open()) {
        cout << "No existing registration file found. Kindly try again! " << endl;
        return;
    }
    else
    {
        registrations.clear(); // Clear existing data

        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            EventRegistration reg;

            getline(ss, reg.eventID, '|');
            getline(ss, reg.manufacturer, '|');
            getline(ss, reg.eventTitle, '|');
            //integer data
            getline(ss, field, '|');
            reg.productQuantity = stoi(field); //change the string to integer
            //phone info
            getline(ss, field, '|');
            reg.phoneInfo.clear(); //clear before add, avoid duplicate
            stringstream phones(field); //set the stringstream
            string phoneEntry;
            while (getline(phones, phoneEntry, ';')) {
                stringstream phoneStream(phoneEntry);
                Product p;
                string priceStr;
                getline(phoneStream, p.productName, '^');
                getline(phoneStream, p.productModel, '^');
                getline(phoneStream, priceStr, '^');
                p.productPrice = stod(priceStr); // change string to double
                reg.phoneInfo.push_back(p);
            }
            getline(ss, reg.description, '|');
            getline(ss, field, '|');
            reg.expectedGuests = stoi(field);
            getline(ss, field, '|');
            reg.estimatedBudget = stod(field);
            getline(ss, reg.eventStatus, '|');
            getline(ss, reg.organizerName, '|');
            getline(ss, reg.organizerContact, '|');
            getline(ss, reg.organizerEmail, '|');
            getline(ss, reg.position, '|');

            registrations.push_back(reg);
        }
        file.close();
    }
}

void loadBookingsFromFile(vector<EventBooking>& bookings) {
    ifstream file("bookings.txt");
    string line;

    if (file.is_open()) {
        bookings.clear(); // Clear existing data

        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            EventBooking booking;

            // Load booking data and embedded structures
            getline(ss, booking.bookingID, '|');
            getline(ss, booking.eventReg.eventID, '|');
            getline(ss, booking.eventReg.eventTitle, '|');
            getline(ss, booking.eventReg.manufacturer, '|');
            getline(ss, booking.eventReg.description, '|');
            getline(ss, token, '|');
            booking.eventReg.expectedGuests = stoi(token);
            getline(ss, token, '|');
            booking.eventReg.estimatedBudget = stod(token);
            getline(ss, booking.eventReg.eventStatus, '|');
            getline(ss, booking.eventReg.organizerName, '|');
            getline(ss, booking.eventReg.organizerContact, '|');

            // Parse date
            getline(ss, token, '|');
            stringstream dateStream(token);
            string datePart;
            getline(dateStream, datePart, '-');
            booking.eventDate.year = stoi(datePart);
            getline(dateStream, datePart, '-');
            booking.eventDate.month = stoi(datePart);
            getline(dateStream, datePart);
            booking.eventDate.day = stoi(datePart);

            getline(ss, booking.eventTime, '|');
            getline(ss, booking.venue.venueID, '|');
            getline(ss, booking.venue.venueName, '|');
            getline(ss, booking.venue.address, '|');
            getline(ss, token, '|');
            booking.venue.capacity = stoi(token);
            getline(ss, token, '|');
            booking.venue.rentalCost = stod(token);
            getline(ss, booking.venue.contactPerson, '|');
            getline(ss, booking.venue.phoneNumber, '|');
            getline(ss, booking.bookingStatus, '|');
            getline(ss, token, '|');
            booking.finalCost = stod(token);

            bookings.push_back(booking);
        }
        file.close();
    }
}
