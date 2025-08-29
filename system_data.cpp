#include "system_data.h"
#include <string>  // for stoi, stod
using namespace std;

// Constructor calls initializeSampleData automatically
SystemData::SystemData() {
    initializeSampleData();
}

// Initialize sample venues
void SystemData::initializeSampleData() {
    if (venues.empty()) {
        string venueData[4][7] = {
            {"V001", "Hall A", "Level 1, Convention Center", "300", "2500.00", "Ahmad Rahman", "03-1234-5678"},
            {"V002", "Hall B", "Level 2, Convention Center", "500", "4000.00", "Siti Nurhaliza", "03-2345-6789"},
            {"V003", "Hall C", "Level 3, Convention Center", "800", "6500.00", "Lee Wei Ming", "03-3456-7890"},
            {"V004", "Auditorium", "Main Building, Tech Plaza", "1200", "10000.00", "Maria Santos", "03-4567-8901"}
        };

        for (int i = 0; i < 4; i++) {
            Venue venue;
            venue.venueID = venueData[i][0];
            venue.venueName = venueData[i][1];
            venue.address = venueData[i][2];
            venue.capacity = stoi(venueData[i][3]);
            venue.rentalCost = stod(venueData[i][4]);
            venue.contactPerson = venueData[i][5];
            venue.phoneNumber = venueData[i][6];
            venues.push_back(venue);
        }
    }
}
