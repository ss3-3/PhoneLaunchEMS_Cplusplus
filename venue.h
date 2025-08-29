#pragma once
#ifndef VENUE_H
#define VENUE_H

#include <string>
#include <vector>
#include "event_registration.h"
#include "timeslot.h"

using namespace std;

struct Venue {
    string venueID;
    string venueName;
    string address;
    int capacity = 0;
    double rentalCost = 0.0;
    string contactPerson;
    string phoneNumber;
    vector<TimeSlot> bookingSchedule;
};

// Venue management functions
bool isVenueAvailable(const Venue& venue, const Date& date, const string& time);
void displayAvailableVenues(const vector<Venue>& venues, const Date& date, const string& time);
int selectAvailableVenue(const vector<Venue>& venues, const Date& date, const string& time);
EventRegistration* findRegistrationByID(vector<EventRegistration>& registrations, const string& eventID);

#endif