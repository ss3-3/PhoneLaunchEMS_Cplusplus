#pragma once
#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "system_data.h"

#include "input_validation.h"
using namespace std;
// Function prototypes - File Operations
void saveDataToFiles(const SystemData& data);
void loadDataFromFiles(SystemData& data);
void saveVenuesToFile(const vector<Venue>& venues);
void saveRegistrationsToFile(const vector<EventRegistration>& registrations);
void saveBookingsToFile(const vector<EventBooking>& bookings);
void loadVenuesFromFile(vector<Venue>& venues);
void loadRegistrationsFromFile(vector<EventRegistration>& registrations);
void loadBookingsFromFile(vector<EventBooking>& bookings);

#endif
