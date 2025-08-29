#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <vector>
#include "venue.h"
#include "event_registration.h"
#include "event_booking.h"
#include "timeslot.h"
using namespace std;
struct SystemData {
    vector<Venue> venues;
    vector<EventRegistration> registrations;
    vector<EventBooking> bookings;
    TimeSlotConfig timeConfig;

    SystemData();                 
    void initializeSampleData(); 
};

#endif
