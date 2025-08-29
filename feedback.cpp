#include "feedback.h"
#include <map>
#include "utility_fun.h"
#include "system_data.h"
#include "input_validation.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
using namespace std;
using namespace std;

void feedbackMenu(SystemData& data) {
    bool exitMenu = false;
    while (!exitMenu) {
        clearScreen();
        displayLogo();
        cout << "=== EVENT FEEDBACK SYSTEM ===" << endl;
        cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;
        cout << "1. Submit Event Feedback" << endl;
        cout << "2. View All Feedback" << endl;
        cout << "3. View Feedback Statistics" << endl;
        cout << "4. Delete Feedback" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;

        int choice = getValidIntegerInput("Enter your choice [1-5]: ", 1, 5);

        switch (choice) {
        case 1:  submitEventFeedback(data); break;
        case 2:  viewAllFeedback(data); break;
        case 3:  viewFeedbackStatistics(data); break;
        case 4:  deleteFeedback(data); break;
        case 5:  exitMenu = true; break;
        }

        if (choice != 5) {
            pauseScreen();
        }
    }
}

// Generate unique feedback ID
string generateFeedbackID(const SystemData& data) {
    return "FB" + to_string(1000 + data.feedbacks.size());
}

// Submit new event feedback
void submitEventFeedback(SystemData& data) {
    clearScreen();
    displayLogo();
    cout << "=== SUBMIT EVENT FEEDBACK ===" << endl;
    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

    // Check if there are any completed bookings
    vector<EventBooking*> completedBookings;
    for (auto& booking : data.bookings) {
        if (booking.bookingStatus == "Confirmed" || booking.bookingStatus == "Completed") {
            completedBookings.push_back(&booking);
        }
    }

    if (completedBookings.empty()) {
        cout << "No completed events found. Cannot submit feedback." << endl;
        return;
    }

    // Display completed bookings
    cout << "Available completed events for feedback:" << endl;
    cout << setfill('-') << setw(80) << "-" << setfill(' ') << endl;
    for (size_t i = 0; i < completedBookings.size(); i++) {
        cout << (i + 1) << ". Event: " << completedBookings[i]->eventReg.eventTitle << endl;
        cout << "   Booking ID: " << completedBookings[i]->bookingID << endl;
        cout << "   Organizer: " << completedBookings[i]->eventReg.organizerName << endl;
        cout << "   Date: " << completedBookings[i]->eventDate.toString() << endl;
        cout << "   Venue: " << completedBookings[i]->venue.venueName << endl;
        cout << endl;
    }

    // Select event
    int eventChoice = getValidIntegerInput("Select event to provide feedback for [1-" +
        to_string(completedBookings.size()) + "]: ",
        1, completedBookings.size());
    EventBooking* selectedBooking = completedBookings[eventChoice - 1];

    // Create new feedback
    EventFeedback newFeedback;
    newFeedback.feedbackID = generateFeedbackID(data);
    newFeedback.bookingID = selectedBooking->bookingID;
    newFeedback.eventTitle = selectedBooking->eventReg.eventTitle;
    newFeedback.organizerName = selectedBooking->eventReg.organizerName;
    newFeedback.eventDate = selectedBooking->eventDate;
    newFeedback.venueName = selectedBooking->venue.venueName;

    cout << "\nSelected Event: " << newFeedback.eventTitle << endl;
    cout << setfill('-') << setw(50) << "-" << setfill(' ') << endl;

    // Get ratings (1-5 scale)
    cout << "\nPlease rate the following aspects (1-5 scale, where 5 is excellent):" << endl;
    newFeedback.venueRating = getValidIntegerInput("Venue Rating [1-5]: ", 1, 5);
    newFeedback.organizationRating = getValidIntegerInput("Event Organization Rating [1-5]: ", 1, 5);
    newFeedback.logisticsRating = getValidIntegerInput("Logistics Rating [1-5]: ", 1, 5);
    newFeedback.overallRating = getValidIntegerInput("Overall Rating [1-5]: ", 1, 5);

    // Get comments
    cout << "\nPlease provide your comments:" << endl;
    cout << "Venue Comments: ";
    cin.ignore();
    getline(cin, newFeedback.venueComments);

    cout << "Organization Comments: ";
    getline(cin, newFeedback.organizationComments);

    cout << "Logistics Comments: ";
    getline(cin, newFeedback.logisticsComments);

    cout << "General Comments: ";
    getline(cin, newFeedback.generalComments);

    cout << "Suggestions for improvement: ";
    getline(cin, newFeedback.suggestions);

    // Get submitter information
    cout << "Your name: ";
    getline(cin, newFeedback.submittedBy);

    // Would recommend
    char recommend;
    cout << "Would you recommend this venue/service to others? (y/n): ";
    cin >> recommend;
    newFeedback.wouldRecommend = (recommend == 'y' || recommend == 'Y');

    // Set submission date (current date)
    newFeedback.submissionDate = getCurrentDate();

    // Add to system
    data.feedbacks.push_back(newFeedback);

    cout << "\nFeedback submitted successfully!" << endl;
    cout << "Feedback ID: " << newFeedback.feedbackID << endl;
}

// View all feedback
void viewAllFeedback(const SystemData& data) {
    clearScreen();
    displayLogo();
    cout << "=== ALL EVENT FEEDBACK ===" << endl;
    cout << setfill('=') << setw(80) << "=" << setfill(' ') << endl;

    if (data.feedbacks.empty()) {
        cout << "No feedback records found." << endl;
        return;
    }

    for (const auto& feedback : data.feedbacks) {
        cout << "Feedback ID: " << feedback.feedbackID << endl;
        cout << "Event: " << feedback.eventTitle << endl;
        cout << "Organizer: " << feedback.organizerName << endl;
        cout << "Event Date: " << feedback.eventDate.toString() << endl;
        cout << "Venue: " << feedback.venueName << endl;
        cout << "Submitted by: " << feedback.submittedBy << endl;
        cout << "Submission Date: " << feedback.submissionDate.toString() << endl;

        cout << "\nRatings:" << endl;
        cout << "  Venue: " << feedback.venueRating << "/5" << endl;
        cout << "  Organization: " << feedback.organizationRating << "/5" << endl;
        cout << "  Logistics: " << feedback.logisticsRating << "/5" << endl;
        cout << "  Overall: " << feedback.overallRating << "/5" << endl;

        cout << "Would Recommend: " << (feedback.wouldRecommend ? "Yes" : "No") << endl;

        if (!feedback.generalComments.empty()) {
            cout << "General Comments: " << feedback.generalComments << endl;
        }
        if (!feedback.suggestions.empty()) {
            cout << "Suggestions: " << feedback.suggestions << endl;
        }

        cout << setfill('-') << setw(80) << "-" << setfill(' ') << endl;
    }
}

// View feedback statistics
void viewFeedbackStatistics(const SystemData& data) {
    clearScreen();
    displayLogo();
    cout << "=== FEEDBACK STATISTICS ===" << endl;
    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;

    if (data.feedbacks.empty()) {
        cout << "No feedback data available for statistics." << endl;
        return;
    }

    size_t totalFeedback = data.feedbacks.size();

    // Calculate overall statistics
    double totalVenue = 0, totalOrg = 0, totalLog = 0, totalOverall = 0;
    int recommendCount = 0;

    for (const auto& feedback : data.feedbacks) {
        totalVenue += feedback.venueRating;
        totalOrg += feedback.organizationRating;
        totalLog += feedback.logisticsRating;
        totalOverall += feedback.overallRating;
        if (feedback.wouldRecommend) recommendCount++;
    }

    cout << "OVERALL STATISTICS" << endl;
    cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;
    cout << "Total Feedback Submissions: " << totalFeedback << endl;
    cout << "Average Venue Rating: " << fixed << setprecision(2) << (totalVenue / totalFeedback) << "/5" << endl;
    cout << "Average Organization Rating: " << (totalOrg / totalFeedback) << "/5" << endl;
    cout << "Average Logistics Rating: " << (totalLog / totalFeedback) << "/5" << endl;
    cout << "Average Overall Rating: " << (totalOverall / totalFeedback) << "/5" << endl;
    cout << "Recommendation Rate: " << recommendCount << "/" << totalFeedback <<
        " (" << (recommendCount * 100.0 / totalFeedback) << "%)" << endl;

    // Rating distribution
    cout << "\nRATING DISTRIBUTION" << endl;
    cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;

    vector<int> overallCounts(6, 0); // Index 0 unused, 1-5 for ratings
    for (const auto& feedback : data.feedbacks) {
        overallCounts[feedback.overallRating]++;
    }

    for (int i = 5; i >= 1; i--) {
        cout << i << " stars: " << overallCounts[i] << " (" <<
            (overallCounts[i] * 100.0 / totalFeedback) << "%)" << endl;
    }

    // Top rated venues (if multiple venues exist)
    cout << "\nVENUE PERFORMANCE" << endl;
    cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;

    map<string, vector<int>> venueRatings;
    for (const auto& feedback : data.feedbacks) {
        venueRatings[feedback.venueName].push_back(feedback.venueRating);
    }

    vector<pair<string, double>> venueAvgs;
    for (const auto& venue : venueRatings) {
        double avg = accumulate(venue.second.begin(), venue.second.end(), 0.0) / venue.second.size();
        venueAvgs.push_back({ venue.first, avg });
    }

    // Sort by average rating
    sort(venueAvgs.begin(), venueAvgs.end(),
        [](const pair<string, double>& a, const pair<string, double>& b) {
            return a.second > b.second;
        });

    for (const auto& venue : venueAvgs) {
        cout << venue.first << ": " << fixed << setprecision(2) << venue.second << "/5 "
            << "(" << venueRatings[venue.first].size() << " reviews)" << endl;
    }

    // Recent feedback trends
    cout << "\nRECENT FEEDBACK TRENDS" << endl;
    cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;

    // Get feedback from last 30 days (simplified - you might want to implement proper date comparison)
    vector<EventFeedback> recentFeedback;
    for (const auto& feedback : data.feedbacks) {
        // Simple check - in real implementation, compare dates properly
        recentFeedback.push_back(feedback);
    }

    if (recentFeedback.size() >= 3) {
        // Show trend for last few feedback
        auto recent = recentFeedback.end() - min(size_t(5), recentFeedback.size());
        double recentAvg = 0;
        for (auto it = recent; it != recentFeedback.end(); ++it) {
            recentAvg += it->overallRating;
        }
        recentAvg /= min(size_t(5), recentFeedback.size());
        cout << "Recent Average Rating (last " << min(size_t(5), recentFeedback.size())
            << " feedback): " << fixed << setprecision(2) << recentAvg << "/5" << endl;
    }

    // Common suggestions/issues
    cout << "\nCOMMON FEEDBACK THEMES" << endl;
    cout << setfill('-') << setw(40) << "-" << setfill(' ') << endl;

    int excellentCount = 0, goodCount = 0, averageCount = 0, poorCount = 0;
    for (const auto& feedback : data.feedbacks) {
        if (feedback.overallRating >= 5) excellentCount++;
        else if (feedback.overallRating >= 4) goodCount++;
        else if (feedback.overallRating >= 3) averageCount++;
        else poorCount++;
    }

    cout << "Excellent Events (5 stars): " << excellentCount << endl;
    cout << "Good Events (4+ stars): " << goodCount << endl;
    cout << "Average Events (3+ stars): " << averageCount << endl;
    cout << "Needs Improvement (<3 stars): " << poorCount << endl;
}

// Delete feedback (admin function)
void deleteFeedback(SystemData& data) {
    clearScreen();
    displayLogo();
    cout << "=== DELETE FEEDBACK ===" << endl;
    cout << setfill('=') << setw(50) << "=" << setfill(' ') << endl;

    if (data.feedbacks.empty()) {
        cout << "No feedback records to delete." << endl;
        return;
    }

    // Display all feedback for selection
    cout << "Current Feedback Records:" << endl;
    cout << setfill('-') << setw(70) << "-" << setfill(' ') << endl;

    for (size_t i = 0; i < data.feedbacks.size(); i++) {
        const auto& feedback = data.feedbacks[i];
        cout << (i + 1) << ". ID: " << feedback.feedbackID
            << " | Event: " << feedback.eventTitle
            << " | Rating: " << feedback.overallRating << "/5" << endl;
    }

    int choice = getValidIntegerInput("Select feedback to delete [1-" +
        to_string(data.feedbacks.size()) + "] (0 to cancel): ",
        0, data.feedbacks.size());

    if (choice == 0) {
        cout << "Delete operation cancelled." << endl;
        return;
    }

    // Confirm deletion
    const auto& toDelete = data.feedbacks[choice - 1];
    cout << "\nConfirm deletion of feedback:" << endl;
    cout << "Feedback ID: " << toDelete.feedbackID << endl;
    cout << "Event: " << toDelete.eventTitle << endl;
    cout << "Submitted by: " << toDelete.submittedBy << endl;

    char confirm;
    cout << "Are you sure you want to delete this feedback? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        data.feedbacks.erase(data.feedbacks.begin() + (choice - 1));
        cout << "Feedback deleted successfully." << endl;
    }
    else {
        cout << "Delete operation cancelled." << endl;
    }
}
