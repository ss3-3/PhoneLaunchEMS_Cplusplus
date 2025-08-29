#include "event_registration.h"
#include "system_data.h"
#include "event_booking.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <regex>
#include "date.h"
#include "venue.h"
#include "timeslot.h"
#include "utility_fun.h"
#include "input_validation.h"

using namespace std;

// Event Registration System Functions
void eventRegistrationMenu(SystemData& data) {
    bool exitMenu = false;

    while (!exitMenu) {
        clearScreen();
        displayLogo();
        cout << "=== EVENT REGISTRATION SYSTEM ===" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;
        cout << "1. Create New Event Registration" << endl;
        cout << "2. View All Event Registrations" << endl;
        cout << "3. Update Event Registration" << endl;
        cout << "4. Delete Event Registration" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

        int choice = getValidIntegerInput("Enter your choice [1-5]: ", 1, 5);

        switch (choice) {
        case 1:
            createEventRegistration(data);
            break;
        case 2:
            viewEventRegistrations(data);
            break;
        case 3:
            updateEventRegistration(data);
            break;
        case 4:
            deleteEventRegistration(data);
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

void createEventRegistration(SystemData& data) {
    clearScreen();
    cout << "=== CREATE NEW EVENT REGISTRATION ===" << endl;

    EventRegistration newReg;

    //declear variable to store the data in struct and vector
    string productName, productModel;
    double productPrice;

    // Manufacturer input with decorative prompt
    cout << "+ ====================================================================== +" << endl;
    cout << "||                               EVENT DETAILS                           ||" << endl;
    cout << "+ ====================================================================== +" << endl << endl;

    // Generate unique event ID
    newReg.eventID = generateEventID(data.registrations);
    cout << "Generated Event ID: " << newReg.eventID << endl << endl;
        
    newReg.manufacturer = getValidStringInputWithExit("Enter Manufacturer: ");
    if (newReg.manufacturer == "")
    {
        confirmExit(); //show the exit message
        return; //back to previous
    }
    newReg.eventTitle = getValidStringInputWithExit("Enter Event Title: ");
    if (newReg.eventTitle.empty())
    {
        confirmExit(); //show the exit message
        return; //back to previous
    }
    // product detail
    cout << "+ ====================================================================== +" << endl;
    cout << "||                               PRODUCT DETAILS                        ||" << endl;
    cout << "+ ====================================================================== +" << endl << endl;

    newReg.productQuantity = getValidIntegerInputWithExit("Enter Product Quantity to launch: ", 1, 20);
    if (newReg.productQuantity == -1)
    {
        confirmExit();
        return;
    }
    newReg.phoneInfo.resize(newReg.productQuantity); //resize = set the vector size, limit input
    for (int i = 0; i < newReg.productQuantity; i++)
    {
        cout << "Product " << i + 1 << ": " << endl;
        productName = getValidStringInputWithExit("Enter product name: ");
        if (productName.empty())
        {
            confirmExit();
            return;
        }
        productModel = getValidStringInputWithExit("Enter product model: ");
        if (productModel.empty())
        {
            confirmExit();
            return;
        }
        productPrice = getValidDoubleInputWithExit("Enter product price (RM): ", 5);
        if (productPrice == -1.0)
        {
            confirmExit();
            return;
        }
        newReg.phoneInfo[i] = { productName, productModel, productPrice }; //store in vector
    }
    // additional detail
    cout << "+ ====================================================================== +" << endl;
    cout << "||                            ADDITIONAL DETAILS                       ||" << endl;
    cout << "+ ====================================================================== +" << endl << endl;

    newReg.description = getValidStringInputWithExit("Enter Event Description: ");
    if (newReg.description.empty())
    {
        confirmExit();
        return;
    }
    newReg.organizerName = getValidStringInputWithExit("Enter Organizer Name: ");
    if (newReg.organizerName.empty())
    {
        confirmExit();
        return;
    }
    newReg.organizerContact = getValidPhoneNumber("Enter Organizer Contact: ");
    if (newReg.organizerContact.empty())
    {
        confirmExit();
        return;
    }
    newReg.organizerEmail = getValidEmailAddress("Enter Organizer Email Address  (must small capital letter with @XXX.com): ");
    if (newReg.organizerEmail.empty())
    {
        confirmExit();
        return;
    }
    newReg.position = getValidStringInput("Enter your position: "); //staff or CEO
    if (newReg.position.empty())
    {
        confirmExit();
        return;
    }
    newReg.expectedGuests = getValidIntegerInputWithExit("Enter Expected Number of Guests (100-10000): ", 100, 10000);
    if (newReg.expectedGuests == -1)
    {
        confirmExit();
        return;
    }
    newReg.estimatedBudget = getValidDoubleInputWithExit("Enter Estimated Budget (RM): ", 2500);
    if (newReg.estimatedBudget == -1.0)
    {
        confirmExit();
        return;
    }
    newReg.eventStatus = "Registered";

    data.registrations.push_back(newReg);

    cout << "\n=== EVENT REGISTRATION CREATED SUCCESSFULLY ===" << endl;
    cout << "Event ID: " << newReg.eventID << endl;
    cout << "Manufacturer: " << newReg.manufacturer << endl;
    cout << "Event Title: " << newReg.eventTitle << endl;
    cout << "Product launched quantity: " << newReg.productQuantity << endl;
    for (int i = 0; i < newReg.productQuantity; i++)
    {
        cout << "Product " << i + 1 << endl;
        cout << "Product Name: " << newReg.phoneInfo[i].productName << endl;
        cout << "Product Model: " << newReg.phoneInfo[i].productModel << endl;
        cout << "Product Price (RM): " << newReg.phoneInfo[i].productPrice << endl;
    }
    cout << "Organizer: " << newReg.organizerName << endl;
    cout << "Position: " << newReg.position << endl;
    cout << "Contact No: " << newReg.organizerContact << endl;
    cout << "Email Address: " << newReg.organizerEmail << endl;
    cout << "Expected Guests: " << newReg.expectedGuests << endl;
    cout << "Estimated Budget: RM " << fixed << setprecision(2) << newReg.estimatedBudget << endl;
    cout << "Status: " << newReg.eventStatus << endl;
}

void viewEventRegistrations(const SystemData& data) {
    clearScreen();
    cout << "=== ALL EVENT REGISTRATIONS ===" << endl;

    if (data.registrations.empty()) {
        cout << left << '+' << setfill('-') << setw(50) << '+' << endl;
        cout << "No event registrations found." << endl;
        return;
    }

    for (const auto& reg : data.registrations) {
        //auto: let compiler determine the type of data
        //&: direct retrieve data instead of copy a new one (waste memory)
        //const: read only, cannot modify the value (more secure)

        cout << left << '+' << setfill('=') << setw(100) << '+' << endl;
        cout << "Event ID: " << reg.eventID << endl;
        cout << "Event Title: " << reg.eventTitle << endl;
        cout << "Product launched quantity: " << reg.productQuantity << endl;
        for (int i = 0; i < reg.productQuantity; i++)
        {
            cout << "Product " << i + 1 << ": " << endl;
            cout << "Product Name: " << reg.phoneInfo[i].productName << endl;
            cout << "Product Model: " << reg.phoneInfo[i].productModel << endl;
            cout << "Product Price (RM): " << fixed << setprecision(2) << reg.phoneInfo[i].productPrice << endl;
        }
        cout << "Event Description: " << reg.description << endl;
        cout << "Organizer Name: " << reg.organizerName << '(' << reg.position << ')' << endl;
        cout << "Organizer Contact No: " << reg.organizerContact << endl;
        cout << "Organizer Email Address: " << reg.organizerEmail << endl;
        cout << "Maxinum Participants: " << reg.expectedGuests << endl;
        cout << "Budget: RM" << reg.estimatedBudget << endl;
    }
    cout << left << '+' << setfill('=') << setw(100) << '+' << endl;
    cout << "Total Registrations: " << data.registrations.size() << endl;
}

void editPhoneInfo(EventRegistration* regPtr) {
    string checkPhoneModel, newName, newModel, newPrice;
    double newPriceDouble;

    checkPhoneModel = getValidStringInputWithExit("Enter the phone model you want to update");
    if (checkPhoneModel.empty()) return;

    bool found = false; //initial false
    for (auto& phone : regPtr->phoneInfo)
    {
        if (phone.productModel == checkPhoneModel)
        {
            found = true;

            cout << "Current Name: " << phone.productName << endl;
            cout << "Current Model: " << phone.productModel << endl;
            cout << "Current Price: RM" << phone.productPrice << endl;

            //Phone name
            newName = getValidStringInputOrKeepCurrent("\nEnter new name");
            if (newName == "EXIT_REQUESTED") return;
            if (newName != "KEEP_CURRENT") phone.productName = newName;
            //Phone Model
            newModel = getValidStringInputOrKeepCurrent("\nEnter new model ");
            if (newModel == "EXIT_REQUESTED") return;
            if (newModel != "KEEP_CURRENT") phone.productModel = newModel;
            //Phone price
            cout << endl << "Enter new price ";
            getline(cin, newPrice);

            if (newPrice == "0")
            {
                if (getExit()) return;
            }
            else if (!newPrice.empty())
            {
                try
                {
                    newPriceDouble = stod(newPrice); //change double value
                    if (newPriceDouble > 0)
                    {
                        phone.productPrice = newPriceDouble;
                    }
                    else
                    {
                        cout << "Price must be greater than 0. Keeping current price. " << endl;
                    }
                }
                catch (const exception& e)
                {
                    //If user enter characters or special symbol
                    cout << "Invalid price format. keeping current price. " << endl;
                }
            }

            cout << endl << "Phone updated successfully!" << endl;
            break;
        }
    }
    if (!found) {
        cout << "No phone found with model " << checkPhoneModel << endl;
    }
}

void updateEventRegistration(SystemData& data) {
    clearScreen();
    cout << "=== UPDATE EVENT REGISTRATION ===" << endl;

    if (data.registrations.empty()) {
        cout << left << '+' << setfill('=') << setw(100) << '+' << endl;
        cout << "No event registrations to update." << endl;
        return;
    }

    viewEventRegistrations(data);

    string eventID = getValidStringInput("\nEnter Event ID to update: ");
    if (eventID.empty()) return; //if user select exit

    eventID = toUpperCase(eventID);
    // Find registration using pointer function
    EventRegistration* regPtr = findRegistrationByID(data.registrations, eventID);

    if (regPtr == nullptr) {
        cout << "Event registration not found!" << endl;
        return;
    }

    cout << "\n=== Current Registration Details ===" << endl;
    cout << "Event Title Registered: " << regPtr->eventTitle << endl;
    cout << "Phone launches quantity: " << regPtr->productQuantity << endl;
    int i = 1;
    cout << "Phone(s) registered" << endl;
    for (const auto& phone : regPtr->phoneInfo)
    {
        cout << "-------------------" << endl;
        cout << "Phone " << i << ": " << endl;
        cout << "Phone Name: " << phone.productName << endl;
        cout << "Phone Model: " << phone.productModel << endl;
        cout << "Phone Price: " << fixed << setprecision(2) << phone.productPrice << endl;
        i++;
    }
    cout << "Manufacturer: " << regPtr->manufacturer << endl;
    cout << "Description: " << regPtr->description << endl;
    cout << "Organizer: " << regPtr->organizerName << endl;
    cout << "Contact: " << regPtr->organizerContact << endl;
    cout << "Maximum Participants: " << regPtr->expectedGuests << endl;
    cout << "Budget: RM " << fixed << setprecision(2) << regPtr->estimatedBudget << endl;
    cout << "Status: " << regPtr->eventStatus << endl;

    cout << "\nWhat would you like to update?" << endl;
    cout << "1. Event Title" << endl;
    cout << "2. Product Launch Quantity" << endl;
    cout << "3. Phone Model" << endl;
    cout << "4. Manufacturer" << endl;
    cout << "5. Description" << endl;
    cout << "6. Organizer Name" << endl;
    cout << "7. Position" << endl;
    cout << "8. Organizer Contact" << endl;
    cout << "9. Organizer Email Address" << endl;
    cout << "10. Maximum Participants" << endl;
    cout << "11. Estimated Budget" << endl;
    cout << "12. Status" << endl;

    int choice = getValidIntegerInput("Enter choice [1-12]: ", 1, 13);

    switch (choice) {
    case 1:
    {
        string newEventTitle = getValidStringInputWithExit("Enter new event title");
        if (newEventTitle.empty()) return;
        regPtr->eventTitle = newEventTitle;
        break;
    }
    case 2:
    {
        regPtr->productQuantity = getValidIntegerInput("Enter new quantity: ", 1, 20);
        break;
    }
    case 3:
    {
        editPhoneInfo(regPtr); //change the phone name, model and price
        break;
    }
    case 4:
    {
        string newManufacturer = getValidStringInputWithExit("Enter new Manufacturer");
        if (newManufacturer.empty()) return;
        regPtr->manufacturer = newManufacturer;
        break;
    }
    case 5:
    {
        string newDescription = getValidStringInputWithExit("Enter new Description");
        if (newDescription.empty()) return;
        regPtr->description = newDescription;
        break;
    }
    case 6:
    {
        string newOrganizerName = getValidStringInputWithExit("Enter new Organizer Name");
        if (newOrganizerName.empty()) return;
        regPtr->organizerName = newOrganizerName;
        break;
    }
    case 7:
    {
        string newPosition = getValidStringInput("Enter new Organizer Position: ");
        if (newPosition.empty()) return;
        regPtr->position = newPosition;
        break;
    }
    case 8:
        regPtr->organizerContact = getValidPhoneNumber("Enter new Organizer Contact: ");
        break;
    case 9:
        regPtr->organizerEmail = getValidEmailAddress("Enter new Organizer Email Address: ");
        break;
    case 10:
        regPtr->expectedGuests = getValidIntegerInput("Enter new Expected Guests (1-10000): ", 1, 10000);
        break;
    case 11:
        regPtr->estimatedBudget = getValidDoubleInput("Enter new Estimated Budget (RM): ");
        break;
    case 12:
        cout << "Status options: Registered, Approved, Rejected" << endl;
        regPtr->eventStatus = getValidStringInput("Enter new Status: ");
        break;
    }

    cout << "Event registration updated successfully!" << endl;
}

void deleteEventRegistration(SystemData& data) {
    clearScreen();
    cout << "=== DELETE EVENT REGISTRATION ===" << endl;

    if (data.registrations.empty()) { //if no event created
        cout << "No event registrations to delete." << endl;
        return;
    }

    viewEventRegistrations(data);

    string eventID = getValidStringInput("\nEnter Event ID to delete: ");

    // Find registration using loop
    int regIndex = -1;
    for (size_t i = 0; i < data.registrations.size(); i++) {
        if (data.registrations[i].eventID == eventID) {
            regIndex = i;
            break;
        }
    }

    if (regIndex == -1) {
        cout << "Event registration not found!" << endl;
        return;
    }

    cout << "\n=== Registration Details to Delete ===" << endl;
    cout << "Event ID: " << data.registrations[regIndex].eventID << endl;
    cout << "Event Title: " << data.registrations[regIndex].eventTitle << endl;
    cout << "Manufacturer: " << data.registrations[regIndex].manufacturer << endl;
    cout << "Organizer: " << data.registrations[regIndex].organizerName << endl;

    vector<char> validChars = { 'Y', 'N' };
    char confirm = getValidCharInput("\nAre you sure you want to delete this registration? (Y/N): ", validChars);

    if (confirm == 'Y' || confirm == 'y') {
        // Check if registration is used in bookings
        bool hasBookings = false;
        for (const auto& booking : data.bookings) {
            if (booking.eventReg.eventID == eventID) {
                hasBookings = true;
                break;
            }
        }

        if (hasBookings) {
            cout << "Cannot delete registration - it has associated bookings!" << endl;
            return;
        }

        data.registrations.erase(data.registrations.begin() + regIndex);
        cout << "Event registration deleted successfully!" << endl;
    }
    else {
        cout << "Deletion cancelled." << endl;
    }
}
