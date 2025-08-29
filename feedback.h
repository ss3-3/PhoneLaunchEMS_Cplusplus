#pragma once
#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include "date.h"
#include "event_booking.h"
#include "utility_fun.h"
#include "input_validation.h"

// Feedback structure
struct EventFeedback {
    string feedbackID;
    string bookingID;
    string eventTitle;
    string organizerName;
    Date eventDate;
    string venueName;

    // Ratings (1-5 scale)
    int venueRating;
    int organizationRating;
    int logisticsRating;
    int overallRating;

    // Comments
    string venueComments;
    string organizationComments;
    string logisticsComments;
    string generalComments;
    string suggestions;

    // Metadata
    string submittedBy;
    Date submissionDate;
    bool wouldRecommend;
};

//feedback menu
void feedbackMenu(SystemData& data);

// Submit new event feedback
void submitEventFeedback(SystemData& data);

// View feedback
void viewAllFeedback(const SystemData& data);

// View feedback statistics
void viewFeedbackStatistics(const SystemData& data);

// Validate feedback rating input
int getValidRating(const std::string& prompt);

// Delete feedback (admin function)
void deleteFeedback(SystemData& data);

#endif