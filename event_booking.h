#pragma once
#ifndef EVENT_BOOKING_H
#define EVENT_BOOKING_H

#include "date.h"
#include "venue.h"
#include "event_registration.h"
#include "timeslot.h"
#include "logistics.h"

using namespace std;

// Event Booking (links registration with date and venue)
struct EventBooking {
    string bookingID;
    EventRegistration eventReg;
    Date eventDate;
    string eventTime;
    Venue venue;
    string bookingStatus; // "Pending", "Confirmed", "Completed", "Cancelled"
    double finalCost;
    vector<string> logisticsItems;
    double logisticsCost;
};

// Function prototypes - Event Booking System
class SystemData;
void eventBookingMenu(SystemData& data);
void createEventBooking(SystemData& data);
void viewEventBookings(const SystemData& data);
void updateEventBooking(SystemData& data);
void cancelEventBooking(SystemData& data);

#endif