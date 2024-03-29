#include <iostream>
#include <ctime>
#include <sstream>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iterator>
#include <locale>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <iomanip>
#include <stdio.h>

// Function to increment a date string by one day
// std::string incrementDate(const std::string& dateString) {
//     // Parse the input date string
//     std::tm date = {};
//     std::istringstream iss(dateString);
//     iss >> std::get_time(&date, "%Y-%m-%d");

//     // Convert the date to a time_t value (seconds since epoch)
//     std::time_t time = std::mktime(&date);
//     if (time == -1) {
//         std::cerr << "Error: Unable to convert date string to time_t." << std::endl;
//         return "";
//     }

//     // Increment the time by one day (86400 seconds)
//     time += 86400;

//     // Update the date struct using the modified time value
//     std::tm* nextDayPtr = std::localtime(&time);
//     std::tm nextDay = *nextDayPtr;

//     // Convert the incremented date back to a string
//     std::ostringstream oss;
//     oss << std::put_time(&nextDay, "%Y-%m-%d");
//     return oss.str();
// }
// Function to increment a date by one day

// std::string incrementDate(const std::string& dateString) {
//     // Convert the date string to a time structure
//     struct tm tm;
//     strptime(dateString.c_str(), "%Y-%m-%d", &tm);

//     // Add one day to the date
//     tm.tm_mday += 1;

//     // Convert the modified time structure back to a time_t value
//     time_t next = mktime(&tm);

//     // Convert the time_t value back to a string
//     char buffer[11]; // 10 characters for "YYYY-MM-DD" + null terminator
//     strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);

//     return buffer;
// }

// int main() {
//     std::string baseDate = "2024-02-01";
//     std::string nextDate = incrementDate(baseDate);
//     std::cout << "Next day: " << nextDate << std::endl;

//     return 0;
// }

// time_t convertToUnixTime(const std::string& dateString) {
//     struct tm tm;
//     strptime(dateString.c_str(), "%Y-%m-%d", &tm);
//     return mktime(&tm);
// }

// int main()
// {
//   time_t base = convertToUnixTime("2024-02-05");
//   std::cout << ctime(&base) << std::endl;
//   for(int i = 0; i < 4; ++i) {
//     struct tm* tm = localtime(&base);
//     tm->tm_mday += i;
//     time_t next = mktime(tm);
//     std::cout << ctime(&next);
//  }
//  return 0;
// }


// int main() {
//     struct tm date;
//     date.tm_year = 2024 - 1900;  // Year minus 1900
//     date.tm_mon = 1;             // Month (0-11, so 1 represents February)
//     date.tm_mday = 1;            // Day of the month
//     date.tm_hour = 0;            // Hour
//     date.tm_min = 0;             // Minute
//     date.tm_sec = 0;             // Second

//     // Convert to time_t
//     time_t unix_time = mktime(&date);

//     // Increment the day
//     date.tm_mday++;
//     date.tm_mday++;

//     // Convert back to time_t
//     unix_time = mktime(&date);

//     // Convert back to real date
//     struct tm* new_date = gmtime(&unix_time);

//     // Print the new date
//     std::cout << "New Date: " << new_date->tm_year + 1900 << "-"
//               << new_date->tm_mon + 1 << "-" << new_date->tm_mday << std::endl;

//     return 0;
// }

int main() {
    struct tm date;
    date.tm_year = 2024 - 1900;  // Year minus 1900
    date.tm_mon = 1;             // Month (0-11, so 1 represents February)
    date.tm_mday = 1;            // Day of the month
    date.tm_hour = 0;            // Hour
    date.tm_min = 0;             // Minute
    date.tm_sec = 0;             // Second

    // Convert to time_t
    time_t unix_time = mktime(&date);

    // Increment the day
    date.tm_mday++;

    // Convert back to time_t
    unix_time = mktime(&date);

    // Check for month and year rollovers
    while (date.tm_mday == 1) {
        date.tm_mon++;  // Increment month
        if (date.tm_mon > 11) {
            date.tm_mon = 0;    // Reset month to January
            date.tm_year++;     // Increment year
        }
        unix_time = mktime(&date);
    }

    // Convert back to real date
    struct tm* new_date = gmtime(&unix_time);

    // Print the new date
    std::cout << "New Date: " << new_date->tm_year + 1900 << "-"
              << new_date->tm_mon + 1 << "-" << new_date->tm_mday << std::endl;

    return 0;
}