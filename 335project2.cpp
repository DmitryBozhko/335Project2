//
//  main.cpp
//  355Project2_5
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct TimeInterval {
    int start;  // in minutes from 00:00
    int end;
};

/*
 Helper function to convert time in "HH:MM" format to minutes
 This is easier for me to determine by working with 1 number rather than 2 numbers
 Example: 2 hr and 30 mins + 3 hr and 15 mins I would need to add the mins to be 45 min then 2 + 3 but if
 the mins exceed 59 then we need to add 1 to hours. To make it easy to calculate we just tranform all hours
 to mins.
 So 2hr 30min + 3hr 15mins is = 150 min + 195 mins = 345 mins
*/
 int timeToMinutes(const string& time) {
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

/*
 Helper function to convert minutes back to "HH:MM" format
 When we want to convert back into hour::minute format we divide by 60
 From previous example:
 345 mins / 60 is 5.75 but we grab 5 as the hours and 75 as the minutes but multiple it by
 60% or 0.6 to get minutes so .75 is 45 minutes.
 So we combine both to get 5 hr 45 mins
 */
 string minutesToTime(int minutes) {
    int hours = minutes / 60;
    minutes %= 60;
    return (hours < 10 ? "0" : "") + to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes);
}

/*
 Merge intervals to get non-overlapping busy intervals
 This ensures that busy times are combined to avoid duplicate entries.
 Example: [1:00 - 2:00], [2:00 - 3:00] becomes [1:00 - 3:00]
 */
vector<TimeInterval> mergeIntervals(vector<TimeInterval>& intervals) {
    if (intervals.empty()) return {}; // If there is no input return empty
    
// Sort intervals by order
    sort(intervals.begin(), intervals.end(), [](TimeInterval a, TimeInterval b) { return a.start < b.start; });

    vector<TimeInterval> merged;
    //Start in beginning
    merged.push_back(intervals[0]);

    for (int i = 1; i < intervals.size(); ++i) {
        // If there is an overlap then combine update the end time
        if (merged.back().end >= intervals[i].start) {
            merged.back().end = max(merged.back().end, intervals[i].end);
        } else {
            // If it fits in the given interval then just put it in
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}

/*
 Find available intervals for a single member by subtracting busy intervals from active period
 Example: If my working time is from 1:00 - 10:00 but I'm busy from 6:00-6:30 for a lunch break
 and again from 6:50-7:20 then I'm free from 1:00-6:00, 6:30-6:50, then 7:20-10:00
 Then we see how long the meeting will take. Lets say 30 mins. 30 mins can't fit inside
 the time slot of 6:30-6:50 so we delete that free time slot. So the time that works for us is
 1:00-6:00 and 7:20-10:00
 */
vector<TimeInterval> findAvailableSlots(TimeInterval activePeriod, vector<TimeInterval>& busyIntervals, int minDuration) {
    
    vector<TimeInterval> available;
    //Start time starts in the active/working time
    int currentStart = activePeriod.start;

    // Checks the busy intervals and if they intersect in the active time
    // If so then check to see if the meeting length can fit inside the schedule
    for (auto& busy : busyIntervals) {
        if (busy.start > currentStart) {
            // Calculate the free time
            int freeEnd = min(activePeriod.end, busy.start);
            // If the meeting time fits inside the free time put the timeslot in
            // the avaliable time slot
            if (freeEnd - currentStart >= minDuration) {
                available.push_back({currentStart, freeEnd});
            }
        }
        // Change the now start time to the end of the busy time
        currentStart = max(currentStart, busy.end);
    }
    // When we finish checking all the busy time check to see if theres any time left after the
    // avaliable time
    if (activePeriod.end - currentStart >= minDuration) {
        available.push_back({currentStart, activePeriod.end});
    }

    return available;
}

/*
 Find common intervals across all members
 If there is any intersecting time slots amongst all members then
 put the slots into a vector
 */
vector<TimeInterval> findCommonIntervals(vector<vector<TimeInterval>>& allAvailableSlots, int minDuration) {
    if (allAvailableSlots.empty()) return {};

    vector<pair<int, bool>> endpoints;
    
    // For each point in time track how many schedules are avalaible
    for (const auto& memberSlots : allAvailableSlots) {
        for (const auto& slot : memberSlots) {
            endpoints.push_back({slot.start, true});   // start point = true
            endpoints.push_back({slot.end, false});    // end point = false
        }
    }
    
    sort(endpoints.begin(), endpoints.end());
    
    vector<TimeInterval> result;
    int count = 0;          // Overlapping scheduels
    int startTime = -1;     // Start time of common interval
    
    for (size_t i = 0; i < endpoints.size(); i++) {
        int time = endpoints[i].first;
        bool isStart = endpoints[i].second;
        
        if (isStart) {
            // if a point is found increment overlap count
            count++;
            // If all the schedules overlapp start a common interval
            if (count == allAvailableSlots.size()) {
                startTime = time;
            }
        } else {
            // If an end point is found stop the overlap
            count--;
            // If we dont have complete overlap and we had a start time
            if (count < allAvailableSlots.size() && startTime != -1) {
                // If the interval between the start of overlap and now
                // meets the minimum meeting requirement make it a valid entry
                if (time - startTime >= minDuration) {
                    result.push_back({startTime, time});
                }
                startTime = -1;
            }
        }
    }
    
    return result;
}
// Function to create a sample input file if it doesn't exist
void createSampleInputFile() {
    ofstream sampleInput("input.txt");
    
    // Test Case 1: Basic two person schedule (Original case)
    sampleInput << "2\n30\n";
    sampleInput << "09:00 18:00\n";
    sampleInput << "09:00 10:30, 12:00 13:00, 15:00 16:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "10:00 11:30, 13:00 14:00, 16:00 17:00\n";
    
    // Test Case 2: Three person schedule
    sampleInput << "3\n45\n";
    sampleInput << "09:00 18:00\n";
    sampleInput << "09:00 10:30, 12:00 13:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "10:00 11:30, 13:00 14:00\n";
    sampleInput << "08:00 16:00\n";
    sampleInput << "11:00 12:00, 14:00 15:00\n";
    
    // Test Case 3: Edge Case - All members completely free
    sampleInput << "2\n30\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "\n";
    
    // Test Case 4: Edge Case - No common slots possible
    sampleInput << "2\n30\n";
    sampleInput << "09:00 12:00\n";
    sampleInput << "09:00 12:00\n";
    sampleInput << "13:00 17:00\n";
    sampleInput << "13:00 17:00\n";
    
    // Test Case 5: Exact fit for meeting duration
    sampleInput << "2\n30\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "09:00 10:00, 10:30 17:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "09:00 10:00, 10:30 17:00\n";
    
    // Test Case 6: Early/Late Hours
    sampleInput << "2\n60\n";
    sampleInput << "06:00 14:00\n";
    sampleInput << "06:00 08:00, 10:00 12:00\n";
    sampleInput << "12:00 20:00\n";
    sampleInput << "14:00 16:00, 18:00 20:00\n";
    
    // Test Case 7: Back-to-back meetings
    sampleInput << "2\n30\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "09:00 10:00, 10:00 11:00, 11:00 12:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "12:00 13:00, 13:00 14:00, 14:00 15:00\n";
    
    // Test Case 8: Single person
    sampleInput << "1\n30\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "10:00 11:00, 13:00 14:00\n";
    
    // Test Case 9: Long meeting duration
    sampleInput << "2\n120\n";
    sampleInput << "09:00 18:00\n";
    sampleInput << "09:00 10:00, 12:00 13:00\n";
    sampleInput << "09:00 18:00\n";
    sampleInput << "15:00 16:00, 17:00 18:00\n";
    
    // Test Case 10: Maximum members (5 people)
    sampleInput << "5\n30\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "12:00 13:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "11:00 12:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "10:00 11:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "13:00 14:00\n";
    sampleInput << "09:00 17:00\n";
    sampleInput << "14:00 15:00\n";
    
    sampleInput.close();
    cout << "Sample input.txt file created with 10 test cases including edge cases." << endl;
    cout << "Test cases breakdown:\n";
    cout << "1. Basic two person schedule\n";
    cout << "2. Three person schedule\n";
    cout << "3. Edge Case: All members completely free\n";
    cout << "4. Edge Case: No common slots possible\n";
    cout << "5. Exact fit for meeting duration\n";
    cout << "6. Early/Late hours schedule\n";
    cout << "7. Back-to-back meetings\n";
    cout << "8. Single person schedule\n";
    cout << "9. Long meeting duration\n";
    cout << "10. Maximum members (5 people)\n";
}

int main() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    // If the input file doesn't exist create a new file with test cases
    if (!inputFile.is_open()) {
        createSampleInputFile();
        cout << "Created sample input file. Please run the program again." << endl;
        return 1;
    }
    // Make sure output opened
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return 1;
    }

    try {
        int caseNumber = 1;
        
        /*
         Process each test case until end of file
         Each case includes a number of members, minimum meeting duration 
         and each member's active period and busy intervals
        */

        while (!inputFile.eof()) {
            int numMembers;
            // Read number of members and stop if there arent any test cases left
            if (!(inputFile >> numMembers)) {
                break;
            }

            // Write the header into output file
            outputFile << "\nCase " << caseNumber << ":\n";
            outputFile << "----------------------------------------\n";

            // Read minimum meeting duration then ignore new line
            int minDuration;
            inputFile >> minDuration;
            inputFile.ignore();

            // Write number of memebrs and meeting duration into the output file
            outputFile << "Number of members: " << numMembers << endl;
            outputFile << "Meeting duration: " << minDuration << " minutes\n";

            // Vectors to store available slots for all members and their active periods
            vector<vector<TimeInterval>> allAvailableSlots;
            vector<TimeInterval> activePeriods(numMembers);

            // Process the members schedule's
            for (int i = 0; i < numMembers; ++i) {

                // Read, convert, and process the working hours 
                string login, logout;
                inputFile >> login >> logout;
                activePeriods[i] = {timeToMinutes(login), timeToMinutes(logout)};
                inputFile.ignore();

                outputFile << "Member " << (i + 1) << " active period: " << login << " to " << logout << endl;
                
                // Read busy intervals line and parse into TimeInterval
                // If the line is empty that means that members has no busy intervals
                string busyIntervalsLine;
                getline(inputFile, busyIntervalsLine);

                vector<TimeInterval> busyIntervals;
                if (!busyIntervalsLine.empty()) {
                    stringstream ss(busyIntervalsLine);
                    string interval;
                    while (getline(ss, interval, ',')) {
                        stringstream intervalStream(interval);
                        string start, end;
                        intervalStream >> start >> end;
                        busyIntervals.push_back({timeToMinutes(start), timeToMinutes(end)});
                    }
                }

                // Merge the overlapping busy intervals then find avaliable slots that can
                // fit into the meeting duration and add it to the allAvaliableSlots 
                busyIntervals = mergeIntervals(busyIntervals);
                vector<TimeInterval> availableSlots = findAvailableSlots(activePeriods[i], busyIntervals, minDuration);
                allAvailableSlots.push_back(availableSlots);
            }

            vector<TimeInterval> commonFreeSlots = findCommonIntervals(allAvailableSlots, minDuration);

            outputFile << "\nAvailable time slots:\n";
            if (commonFreeSlots.empty()) {
                outputFile << "No common time slots available for the specified duration.\n";
            } else {
                for (const auto& interval : commonFreeSlots) {
                    outputFile << "[" << minutesToTime(interval.start) << ", " 
                             << minutesToTime(interval.end) << "]\n";
                }
            }
            outputFile << "----------------------------------------\n";
            
            caseNumber++;
        }

    // Catch any errors that happen during file proccessing and write it both
    // console and output
    } catch (const exception& e) {
        cerr << "Error processing input file: " << e.what() << endl;
        outputFile << "Error processing input file: " << e.what() << endl;
        return 1;
    }

    // Close files and send a complete message
    inputFile.close();
    outputFile.close();

    cout << "Results have been written to output.txt" << endl;
    return 0;
}