#pragma once
#ifndef EVENT_REGISTRATION_H
#define EVENT_REGISTRATION_H

#include <vector>
#include <string>
#include "product.h"
using namespace std;

struct EventRegistration {
    string eventID;
    string manufacturer;
    string eventTitle;
    int productQuantity;
    vector<Product> phoneInfo; //one or many phone model 
    string description; // simply descibe the event activity
    int expectedGuests; //how many people attend the product launches event
    double estimatedBudget;
    string eventStatus; // "Registered", "Approved", "Rejected"
    string organizerName; //Steven Lee
    string organizerContact; //0123456789
    string organizerEmail; //stevenlee123@gmail.com
    string position; // "staff", "event department leader", "boss"
};

// Function prototypes - Event Registration System
class SystemData;
void eventRegistrationMenu(SystemData& data);
void createEventRegistration(SystemData& data);
void viewEventRegistrations(const SystemData& data);
void editPhoneInfo(EventRegistration* regPtr);
void updateEventRegistration(SystemData& data);
void deleteEventRegistration(SystemData& data);

#endif