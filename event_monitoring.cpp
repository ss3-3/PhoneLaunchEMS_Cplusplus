#include "event_monitoring.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <regex>
#include <map> 
#include <utility>
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


// Event Monitoring Module Implementation
void eventMonitoringMenu(SystemData& data) {
    bool exitMenu = false;

    while (!exitMenu) {
        clearScreen();
        displayLogo();
        cout << "=== EVENT MONITORING & REPORTING ===" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;
        cout << "1. Event Summary Dashboard" << endl;
        cout << "2. Venue Utilization Report" << endl;
        cout << "3. Financial Report" << endl;
        cout << "4. Upcoming Events" << endl;
        cout << "5. Registration Statistics" << endl;
        cout << "6. Search Events" << endl;
        cout << "7. Generate Detailed Report" << endl;
        cout << "8. Back to Main Menu" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

        int choice = getValidIntegerInput("Enter your choice [1-8]: ", 1, 8);

        switch (choice) {
        case 1:
            displayEventSummary(data);
            break;
        case 2:
            displayVenueUtilization(data);
            break;
        case 3:
            displayFinancialReport(data);
            break;
        case 4:
            displayUpcomingEvents(data);
            break;
        case 5:
            displayRegistrationStatistics(data);
            break;
        case 6:
            searchEvents(data);
            break;
        case 7:
            generateEventReport(data);
            break;
        case 8:
            exitMenu = true;
            break;
        }

        if (choice != 8) {
            pauseScreen();
        }
    }
}

void displayEventSummary(const SystemData& data) {
    clearScreen();
    cout << "=== EVENT SUMMARY DASHBOARD ===" << endl;
    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;

    int totalRegistrations = data.registrations.size();
    int totalBookings = data.bookings.size();
    int totalVenues = data.venues.size();

    // Count different statuses
    int registeredCount = 0, approvedCount = 0, rejectedCount = 0;
    int pendingCount = 0, confirmedCount = 0, completedCount = 0, cancelledCount = 0;

    for (const auto& reg : data.registrations) {
        if (reg.eventStatus == "REGISTERED") registeredCount++;
        else if (reg.eventStatus == "APPROVED") approvedCount++;
        else if (reg.eventStatus == "REJECTED") rejectedCount++;
    }

    for (const auto& booking : data.bookings) {
        if (booking.bookingStatus == "Pending") pendingCount++;
        else if (booking.bookingStatus == "Confirmed") confirmedCount++;
        else if (booking.bookingStatus == "Completed") completedCount++;
        else if (booking.bookingStatus == "Cancelled") cancelledCount++;
    }

    cout << left << setw(30) << "Total Registrations:" << totalRegistrations << endl;
    cout << left << setw(30) << "Total Bookings:" << totalBookings << endl;
    cout << left << setw(30) << "Available Venues:" << totalVenues << endl;
    cout << endl;

    cout << "=== REGISTRATION STATUS ===" << endl;
    cout << left << setw(20) << "Registered:" << registeredCount << endl;
    cout << left << setw(20) << "Approved:" << approvedCount << endl;
    cout << left << setw(20) << "Rejected:" << rejectedCount << endl;
    cout << endl;

    cout << "=== BOOKING STATUS ===" << endl;
    cout << left << setw(20) << "Pending:" << pendingCount << endl;
    cout << left << setw(20) << "Confirmed:" << confirmedCount << endl;
    cout << left << setw(20) << "Completed:" << completedCount << endl;
    cout << left << setw(20) << "Cancelled:" << cancelledCount << endl;
    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;
}

void displayVenueUtilization(const SystemData& data) {
    clearScreen();
    cout << "=== VENUE UTILIZATION REPORT ===" << endl;
    cout << setfill('=') << setw(80) << "=" << setfill(' ') << endl;

    cout << left
        << setw(15) << "Venue ID"
        << setw(20) << "Venue Name"
        << setw(10) << "Capacity"
        << setw(15) << "Bookings"
        << setw(20) << "Utilization %" << endl;
    cout << setfill('-') << setw(80) << "-" << setfill(' ') << endl;

    for (const auto& venue : data.venues) {
        int bookingCount = venue.bookingSchedule.size();
        double utilization = (static_cast<double>(bookingCount) / 30.0) * 100.0; // Assuming 30 days month

        cout << left
            << setw(15) << venue.venueID
            << setw(20) << venue.venueName
            << setw(10) << venue.capacity
            << setw(15) << bookingCount
            << setw(20) << fixed << setprecision(1) << utilization << "%" << endl;
    }
    cout << setfill('=') << setw(80) << "=" << setfill(' ') << endl;
}

void displayFinancialReport(const SystemData& data) {
    clearScreen();
    cout << "=== FINANCIAL REPORT ===" << endl;
    cout << setfill('=') << setw(70) << "=" << setfill(' ') << endl;

    double totalRevenue = 0.0;
    double totalBudget = 0.0;
    double pendingRevenue = 0.0;
    double confirmedRevenue = 0.0;
    double completedRevenue = 0.0;

    for (const auto& booking : data.bookings) {
        if (booking.bookingStatus == "Completed") {
            completedRevenue += booking.finalCost;
            totalRevenue += booking.finalCost;
        }
        else if (booking.bookingStatus == "Confirmed") {
            confirmedRevenue += booking.finalCost;
            totalRevenue += booking.finalCost;
        }
        else if (booking.bookingStatus == "Pending") {
            pendingRevenue += booking.finalCost;
        }
    }

    for (const auto& reg : data.registrations) {
        totalBudget += reg.estimatedBudget;
    }

    cout << left << setw(30) << "Total Estimated Budget (All Events):"
        << "RM " << fixed << setprecision(2) << totalBudget << endl;
    cout << left << setw(30) << "Total Revenue (Bookings):"
        << "RM " << fixed << setprecision(2) << totalRevenue << endl;
    cout << left << setw(30) << "Pending Revenue:"
        << "RM " << fixed << setprecision(2) << pendingRevenue << endl;
    cout << left << setw(30) << "Confirmed Revenue:"
        << "RM " << fixed << setprecision(2) << confirmedRevenue << endl;
    cout << left << setw(30) << "Completed Revenue:"
        << "RM " << fixed << setprecision(2) << completedRevenue << endl;
    cout << endl;

    // Top 5 most expensive events
    cout << "=== TOP 5 MOST EXPENSIVE EVENTS ===" << endl;
    vector<EventBooking> sortedBookings = data.bookings;
    sort(sortedBookings.begin(), sortedBookings.end(),
        [](const EventBooking& a, const EventBooking& b) {
            return a.finalCost > b.finalCost;
        });

    int count = 0;
    for (const auto& booking : sortedBookings) {
        if (count++ >= 5) break;
        cout << left << setw(25) << booking.eventReg.eventTitle
            << "RM " << fixed << setprecision(2) << booking.finalCost << endl;
    }
    cout << setfill('=') << setw(70) << "=" << setfill(' ') << endl;
}

void displayUpcomingEvents(const SystemData& data) {
    clearScreen();
    cout << "=== UPCOMING EVENTS (Next 30 days) ===" << endl;
    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;

    Date currentDate = getCurrentDate();
    int upcomingCount = 0;

    cout << left
        << setw(12) << "Date"
        << setw(15) << "Time"
        << setw(20) << "Event Title"
        << setw(15) << "Venue"
        << setw(10) << "Guests"
        << setw(15) << "Status" << endl;
    cout << setfill('-') << setw(100) << "-" << setfill(' ') << endl;

    for (const auto& booking : data.bookings) {
        // Simple date comparison (for demonstration)
        if (booking.eventDate.year >= currentDate.year &&
            booking.eventDate.month >= currentDate.month &&
            booking.eventDate.day >= currentDate.day &&
            booking.bookingStatus != "Cancelled") {

            cout << left
                << setw(12) << booking.eventDate.toString()
                << setw(15) << booking.eventTime
                << setw(20) << booking.eventReg.eventTitle.substr(0, 19)
                << setw(15) << booking.venue.venueName.substr(0, 14)
                << setw(10) << booking.eventReg.expectedGuests
                << setw(15) << booking.bookingStatus << endl;

            upcomingCount++;
        }
    }

    if (upcomingCount == 0) {
        cout << "No upcoming events in the next 30 days." << endl;
    }

    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;
    cout << "Total Upcoming Events: " << upcomingCount << endl;
}

void displayRegistrationStatistics(const SystemData& data) {
    clearScreen();
    cout << "=== REGISTRATION STATISTICS ===" << endl;
    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;

    map<string, int> manufacturerStats;
    map<string, int> organizerStats;
    int totalGuests = 0;
    int totalProducts = 0;

    for (const auto& reg : data.registrations) {
        manufacturerStats[reg.manufacturer]++;
        organizerStats[reg.organizerName]++;
        totalGuests += reg.expectedGuests;
        totalProducts += reg.productQuantity;
    }

    cout << left << setw(25) << "Total Expected Guests:" << totalGuests << endl;
    cout << left << setw(25) << "Total Products to Launch:" << totalProducts << endl;
    cout << endl;

    cout << "=== MANUFACTURER DISTRIBUTION ===" << endl;
    for (const auto& stat : manufacturerStats) {
        cout << left << setw(20) << stat.first << stat.second << " events" << endl;
    }
    cout << endl;

    cout << "=== TOP ORGANIZERS ===" << endl;
    vector<pair<string, int>> sortedOrganizers(organizerStats.begin(), organizerStats.end());
    sort(sortedOrganizers.begin(), sortedOrganizers.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });

    int count = 0;
    for (const auto& organizer : sortedOrganizers) {
        if (count++ >= 5) break;
        cout << left << setw(20) << organizer.first << organizer.second << " events" << endl;
    }
    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;
}

void searchEvents(const SystemData& data) {
    clearScreen();
    cout << "=== SEARCH EVENTS ===" << endl;
    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

    string searchTerm = getValidStringInput("Enter search term (event title, manufacturer, or organizer): ");
    string upperSearchTerm = toUpperCase(searchTerm);

    vector<EventRegistration> foundRegistrations;
    vector<EventBooking> foundBookings;

    // Search in registrations
    for (const auto& reg : data.registrations) {
        if (toUpperCase(reg.eventTitle).find(upperSearchTerm) != string::npos ||
            toUpperCase(reg.manufacturer).find(upperSearchTerm) != string::npos ||
            toUpperCase(reg.organizerName).find(upperSearchTerm) != string::npos) {
            foundRegistrations.push_back(reg);
        }
    }

    // Search in bookings
    for (const auto& booking : data.bookings) {
        if (toUpperCase(booking.eventReg.eventTitle).find(upperSearchTerm) != string::npos ||
            toUpperCase(booking.eventReg.manufacturer).find(upperSearchTerm) != string::npos ||
            toUpperCase(booking.eventReg.organizerName).find(upperSearchTerm) != string::npos) {
            foundBookings.push_back(booking);
        }
    }

    cout << "\n=== SEARCH RESULTS ===" << endl;
    cout << "Found " << foundRegistrations.size() << " registrations and "
        << foundBookings.size() << " bookings matching '" << searchTerm << "'" << endl;

    if (!foundRegistrations.empty()) {
        cout << "\n=== MATCHING REGISTRATIONS ===" << endl;
        for (const auto& reg : foundRegistrations) {
            cout << "ID: " << reg.eventID << " | " << reg.eventTitle
                << " by " << reg.manufacturer << " | Status: " << reg.eventStatus << endl;
        }
    }

    if (!foundBookings.empty()) {
        cout << "\n=== MATCHING BOOKINGS ===" << endl;
        for (const auto& booking : foundBookings) {
            cout << "Booking ID: " << booking.bookingID << " | " << booking.eventReg.eventTitle
                << " on " << booking.eventDate.toString() << " at " << booking.venue.venueName
                << " | Status: " << booking.bookingStatus << endl;
        }
    }

    if (foundRegistrations.empty() && foundBookings.empty()) {
        cout << "No events found matching your search term." << endl;
    }

    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;
}

void generateEventReport(const SystemData& data) {
    clearScreen();
    cout << "=== GENERATE DETAILED EVENT REPORT ===" << endl;
    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

    string filename = "event_report_" + to_string(getCurrentDate().year) +
        to_string(getCurrentDate().month) + to_string(getCurrentDate().day) + ".txt";

    ofstream reportFile(filename);

    if (!reportFile.is_open()) {
        cout << "Error: Could not create report file!" << endl;
        return;
    }

    // Generate comprehensive report
    reportFile << "EVENT MANAGEMENT SYSTEM REPORT" << endl;
    reportFile << "Generated on: " << getCurrentDate().toString() << endl;
    reportFile << setfill('=') << setw(50) << "=" << setfill(' ') << endl << endl;

    // Summary section
    reportFile << "SUMMARY" << endl;
    reportFile << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
    reportFile << "Total Registrations: " << data.registrations.size() << endl;
    reportFile << "Total Bookings: " << data.bookings.size() << endl;
    reportFile << "Available Venues: " << data.venues.size() << endl << endl;

    // Detailed registrations
    reportFile << "DETAILED REGISTRATIONS" << endl;
    reportFile << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
    for (const auto& reg : data.registrations) {
        reportFile << "Event ID: " << reg.eventID << endl;
        reportFile << "Title: " << reg.eventTitle << endl;
        reportFile << "Manufacturer: " << reg.manufacturer << endl;
        reportFile << "Organizer: " << reg.organizerName << endl;
        reportFile << "Status: " << reg.eventStatus << endl;
        reportFile << "Expected Guests: " << reg.expectedGuests << endl;
        reportFile << "Budget: RM " << fixed << setprecision(2) << reg.estimatedBudget << endl;
        reportFile << "Products: " << reg.productQuantity << endl;
        for (const auto& product : reg.phoneInfo) {
            reportFile << "  - " << product.productName << " (" << product.productModel
                << ") RM " << fixed << setprecision(2) << product.productPrice << endl;
        }
        reportFile << endl;
    }

    // Detailed bookings
    reportFile << "DETAILED BOOKINGS" << endl;
    reportFile << setfill('-') << setw(30) << "-" << setfill(' ') << endl;
    for (const auto& booking : data.bookings) {
        reportFile << "Booking ID: " << booking.bookingID << endl;
        reportFile << "Event: " << booking.eventReg.eventTitle << endl;
        reportFile << "Date: " << booking.eventDate.toString() << endl;
        reportFile << "Time: " << booking.eventTime << endl;
        reportFile << "Venue: " << booking.venue.venueName << endl;
        reportFile << "Status: " << booking.bookingStatus << endl;
        reportFile << "Cost: RM " << fixed << setprecision(2) << booking.finalCost << endl;
        reportFile << endl;
    }

    reportFile.close();
    cout << "Report generated successfully: " << filename << endl;
    cout << "Total events reported: " << data.registrations.size() << " registrations, "
        << data.bookings.size() << " bookings" << endl;
    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;
}