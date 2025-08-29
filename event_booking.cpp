#include "event_booking.h"
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
#include "system_data.h"
#include "timeslot.h"
#include "product.h"
#include "utility_fun.h"
#include "input_validation.h"
#include "file_operation.h"

// Event Booking System Functions
void eventBookingMenu(SystemData& data) {
    bool exitMenu = false;

    while (!exitMenu) {
        clearScreen();
        displayLogo();
        cout << "=== EVENT BOOKING SYSTEM ===" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;
        cout << "1. Create New Event Booking" << endl;
        cout << "2. View All Event Bookings" << endl;
        cout << "3. Update Event Booking" << endl;
        cout << "4. Cancel Event Booking" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

        int choice = getValidIntegerInput("Enter your choice [1-5]: ", 1, 5);

        switch (choice) {
        case 1:
            createEventBooking(data);
            break;
        case 2:
            viewEventBookings(data);
            break;
        case 3:
            updateEventBooking(data);
            break;
        case 4:
            cancelEventBooking(data);
            break;
        case 5:
            exitMenu = true;
            break;
        }

        if (choice != 5) {
            pauseScreen();
        }
    }
}

void createEventBooking(SystemData& data) {
    clearScreen();
    cout << "=== CREATE NEW EVENT BOOKING ===" << endl;
    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

    if (data.registrations.empty()) {
        cout << "No event registrations available for booking!" << endl;
        cout << "Please create an event registration first." << endl;
        return;
    }

    // Display approved registrations
    vector<EventRegistration> approvedRegs;
    for (const auto& reg : data.registrations) {
        if (reg.eventStatus == "Approved" || reg.eventStatus == "Registered") {
            approvedRegs.push_back(reg);
        }
    }

    if (approvedRegs.empty()) {
        cout << "No approved event registrations available for booking!" << endl;
        return;
    }

    cout << "=== AVAILABLE REGISTRATIONS FOR BOOKING ===" << endl;
    cout << setfill('=') << setw(80) << "=" << setfill(' ') << endl;
    cout << left
        << setw(4) << "No."
        << setw(8) << "Event ID"
        << setw(15) << "Event Title" //from phone model change to title
        << setw(12) << "Manufacturer"
        << setw(20) << "Organizer"
        << setw(12) << "Status" << endl;
    cout << setfill('=') << setw(80) << "=" << setfill(' ') << endl;

    for (size_t i = 0; i < approvedRegs.size(); i++) {
        cout << left
            << setw(4) << (i + 1)
            << setw(8) << approvedRegs[i].eventID
            << setw(15) << approvedRegs[i].eventTitle.substr(0, 14) //from phone model change to title
            << setw(12) << approvedRegs[i].manufacturer.substr(0, 11)
            << setw(20) << approvedRegs[i].organizerName.substr(0, 19)
            << setw(12) << approvedRegs[i].eventStatus << endl;
    }
    cout << setfill('=') << setw(80) << "=" << setfill(' ') << endl;

    int regChoice = getValidIntegerInput("Select registration number: ", 1, approvedRegs.size());
    EventRegistration selectedReg = approvedRegs[regChoice - 1];

    EventBooking newBooking;
    newBooking.bookingID = generateBookingID(data.bookings);
    newBooking.eventReg = selectedReg;

    cout << "\nSelected Event: " << selectedReg.eventTitle << " by " << selectedReg.manufacturer << endl;
    cout << "Generated Booking ID: " << newBooking.bookingID << endl;

    // Get event date
    cout << "\nEnter Event Date:" << endl;
    newBooking.eventDate = getValidDateInput();

    // Select time slot
    cout << "\nAvailable Time Slots:" << endl;
    for (size_t i = 0; i < data.timeConfig.timeSlotNames.size(); i++) {
        cout << (i + 1) << ". " << data.timeConfig.timeSlotNames[i] << endl;
    }

    int timeChoice = getValidIntegerInput("Select time slot [1-" +
        to_string(data.timeConfig.timeSlotNames.size()) + "]: ", 1,
        data.timeConfig.timeSlotNames.size());

    newBooking.eventTime = data.timeConfig.timeSlots[timeChoice - 1];

    // Display and select venue
    displayAvailableVenues(data.venues, newBooking.eventDate, newBooking.eventTime);

    int venueIndex = selectAvailableVenue(data.venues, newBooking.eventDate, newBooking.eventTime);

    if (venueIndex == -1) {
        cout << "No venues available for the selected date and time." << endl;
        return;
    }

    newBooking.venue = data.venues[venueIndex];
    newBooking.finalCost = newBooking.venue.rentalCost;
    newBooking.bookingStatus = "Pending";

    // Check capacity
    if (newBooking.eventReg.expectedGuests > newBooking.venue.capacity) {
        cout << "\nWARNING: Expected guests (" << newBooking.eventReg.expectedGuests
            << ") exceeds venue capacity (" << newBooking.venue.capacity << ")!" << endl;

        vector<char> validChars = { 'Y', 'N' };
        char choice = getValidCharInput("Continue anyway? (Y/N): ", validChars);

        if (choice != 'Y' && choice != 'y') {
            cout << "Booking cancelled." << endl;
            return;
        }
    }

    // LOGISTICS SELECTION
    vector<string> selectedLogistics;
    double logisticsCost = selectLogistics(selectedLogistics);
    newBooking.finalCost += logisticsCost;

    // Add booking to the venue's schedule
    TimeSlot newSlot;
    newSlot.date = newBooking.eventDate;
    newSlot.time = newBooking.eventTime;
    newSlot.eventID = newBooking.bookingID;
    newSlot.isBooked = true;
    data.venues[venueIndex].bookingSchedule.push_back(newSlot);

    data.bookings.push_back(newBooking);

    cout << "\n=== BOOKING CREATED SUCCESSFULLY ===" << endl;
    cout << "Booking ID: " << newBooking.bookingID << endl;
    cout << "Event: " << newBooking.eventReg.eventTitle << " by " << newBooking.eventReg.manufacturer << endl;
    cout << "Date & Time: " << newBooking.eventDate.toString() << " (" << newBooking.eventTime << ")" << endl;
    cout << "Venue: " << newBooking.venue.venueName << endl;
    cout << "Expected Guests: " << newBooking.eventReg.expectedGuests << endl;
   
    cout << "\n=== COST BREAKDOWN ===" << endl;
    cout << "Venue Rental: RM " << fixed << setprecision(2) << newBooking.venue.rentalCost << endl;
    if (logisticsCost > 0) {
        cout << "Logistics & Services: RM " << fixed << setprecision(2) << logisticsCost << endl;
        cout << "\nSelected Logistics:" << endl;
        for (size_t i = 0; i < selectedLogistics.size(); i++) {
            cout << "  " << (i + 1) << ". " << selectedLogistics[i] << endl;
        }
    }
    cout << "------------------------" << endl;
    cout << "TOTAL COST: RM " << fixed << setprecision(2) << newBooking.finalCost << endl;
    cout << "Status: " << newBooking.bookingStatus << endl;
}

void viewEventBookings(const SystemData& data) {
    clearScreen();
    cout << "=== ALL EVENT BOOKINGS ===" << endl;

    if (data.bookings.empty()) {
        cout << "No event bookings found." << endl;
        return;
    }

    cout << setfill('=') << setw(140) << "=" << setfill(' ') << endl;
    cout << left << setw(8) << "Book ID"
        << setw(9) << "Event ID"
        << setw(15) << "Event Title" // from phone model change to title
        << setw(12) << "Manufacturer"
        << setw(12) << "Date"
        << setw(15) << "Time"
        << setw(8) << "Guests"
        << setw(15) << "Venue"
        << setw(15) << "Cost (RM)"
        << setw(10) << "Status" << endl;
    cout << setfill('=') << setw(140) << "=" << setfill(' ') << endl;

    for (const auto& booking : data.bookings) {
        cout << left << setw(8) << booking.bookingID
            << setw(9) << booking.eventReg.eventID
            << setw(15) << booking.eventReg.eventTitle.substr(0, 14)
            << setw(12) << booking.eventReg.manufacturer.substr(0, 11)
            << setw(12) << booking.eventDate.toString()
            << setw(15) << booking.eventTime
            << setw(8) << booking.eventReg.expectedGuests
            << setw(15) << booking.venue.venueName.substr(0, 14)
            << "RM " << setw(12) << fixed << setprecision(2) << booking.finalCost
            << setw(10) << booking.bookingStatus << endl;
    }
    cout << setfill('=') << setw(140) << "=" << setfill(' ') << endl;
    cout << "Total Bookings: " << data.bookings.size() << endl;
}

void updateEventBooking(SystemData& data) {
    clearScreen();
    cout << "=== UPDATE EVENT BOOKING ===" << endl;

    if (data.bookings.empty()) {
        cout << "No event bookings to update." << endl;
        return;
    }

    viewEventBookings(data);

    string bookingID = getValidStringInput("\nEnter Booking ID to update: ");
    bookingID = toUpperCase(bookingID);
    // Find booking using loop
    int bookingIndex = -1;
    for (size_t i = 0; i < data.bookings.size(); i++) {
        if (data.bookings[i].bookingID == bookingID) {
            bookingIndex = i;
            break;
        }
    }

    if (bookingIndex == -1) {
        cout << "Event booking not found!" << endl;
        return;
    }

    cout << "\n=== Current Booking Details ===" << endl;
    cout << "Booking ID: " << data.bookings[bookingIndex].bookingID << endl;
    cout << "Event: " << data.bookings[bookingIndex].eventReg.eventTitle
        << " by " << data.bookings[bookingIndex].eventReg.manufacturer << endl;
    cout << "Date: " << data.bookings[bookingIndex].eventDate.toString() << endl;
    cout << "Time: " << data.bookings[bookingIndex].eventTime << endl;
    cout << "Venue: " << data.bookings[bookingIndex].venue.venueName << endl;
    cout << "Expected Guests: " << data.bookings[bookingIndex].eventReg.expectedGuests << endl;
    cout << "Final Cost: RM " << fixed << setprecision(2) << data.bookings[bookingIndex].finalCost << endl;
    cout << "Status: " << data.bookings[bookingIndex].bookingStatus << endl;

    cout << "\nWhat would you like to update?" << endl;
    cout << "1. Booking Status" << endl;
    cout << "2. Final Cost" << endl;
    cout << "3. Add/Update Logistics" << endl;

    int choice = getValidIntegerInput("Enter choice [1-3]: ", 1, 3);

    switch (choice) {
    case 1:
    {
        cout << "Status options: [1]Pending, [2]Confirmed, [3]Completed, [4]Cancelled" << endl;
        int status = getValidIntegerInput("Enter choice [1-4]: ", 1, 4);
        switch (status) {
        case 1: data.bookings[bookingIndex].bookingStatus = "Pending"; break;
        case 2: data.bookings[bookingIndex].bookingStatus = "Confirmed"; break;
        case 3: data.bookings[bookingIndex].bookingStatus = "Completed"; break;
        case 4: data.bookings[bookingIndex].bookingStatus = "Cancelled"; break;
        }
        break;
    }
    case 2:
        data.bookings[bookingIndex].finalCost = getValidDoubleInput("Enter new Final Cost (RM): ");
        break;
    case 3: {
        cout << "\n=== UPDATE LOGISTICS ===" << endl;
        vector<string> selectedLogistics;
        double additionalLogisticsCost = selectLogistics(selectedLogistics);

        if (additionalLogisticsCost > 0) {
            data.bookings[bookingIndex].finalCost += additionalLogisticsCost;
            cout << "\nAdditional logistics cost: RM " << fixed << setprecision(2)
                << additionalLogisticsCost << endl;
            cout << "Updated total cost: RM " << fixed << setprecision(2)
                << data.bookings[bookingIndex].finalCost << endl;
        }
        break;
    }
    }

    cout << "Event booking updated successfully!" << endl;
}

void cancelEventBooking(SystemData& data) {
    clearScreen();
    cout << "=== CANCEL EVENT BOOKING ===" << endl;

    if (data.bookings.empty()) {
        cout << "No event bookings to cancel." << endl;
        return;
    }

    viewEventBookings(data);

    string bookingID = getValidStringInput("\nEnter Booking ID to cancel: ");
    bookingID = toUpperCase(bookingID);

    // Find booking using loop
    int bookingIndex = -1;
    for (size_t i = 0; i < data.bookings.size(); i++) {
        if (data.bookings[i].bookingID == bookingID) {
            bookingIndex = i;
            break;
        }
    }

    if (bookingIndex == -1) {
        cout << "Event booking not found!" << endl;
        return;
    }

    cout << "\n=== Booking Details to Cancel ===" << endl;
    cout << "Booking ID: " << data.bookings[bookingIndex].bookingID << endl;
    cout << "Event: " << data.bookings[bookingIndex].eventReg.eventTitle
        << " by " << data.bookings[bookingIndex].eventReg.manufacturer << endl;
    cout << "Date: " << data.bookings[bookingIndex].eventDate.toString() << endl;
    cout << "Time: " << data.bookings[bookingIndex].eventTime << endl;
    cout << "Venue: " << data.bookings[bookingIndex].venue.venueName << endl;
    cout << "Final Cost: RM " << fixed << setprecision(2) << data.bookings[bookingIndex].finalCost << endl;

    vector<char> validChars = { 'Y', 'N' };
    char confirm = getValidCharInput("\nAre you sure you want to cancel this booking? (Y/N): ", validChars);

    if (confirm == 'Y' || confirm == 'y') {
        // Remove booking from venue schedule
        for (size_t i = 0; i < data.venues.size(); i++) {
            if (data.venues[i].venueName == data.bookings[bookingIndex].venue.venueName) {
                for (size_t j = 0; j < data.venues[i].bookingSchedule.size(); j++) {
                    if (data.venues[i].bookingSchedule[j].eventID == bookingID) {
                        data.venues[i].bookingSchedule.erase(data.venues[i].bookingSchedule.begin() + j);
                        break;
                    }
                }
                break;
            }
        }

        data.bookings.erase(data.bookings.begin() + bookingIndex);
        cout << "Event booking cancelled successfully!" << endl;
    }
    else {
        cout << "Cancellation aborted." << endl;
    }
}