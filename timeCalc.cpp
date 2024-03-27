#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

// Function to add multiple time durations represented as strings
std::string addTimes(const std::vector<std::string>& times) {
    int totalSeconds = 0;
    for (size_t i = 0; i < times.size(); ++i) {
        std::istringstream iss(times[i]);
        int hours, minutes, seconds;
        char colon;
        iss >> hours >> colon >> minutes >> colon >> seconds;
        totalSeconds += hours * 3600 + minutes * 60 + seconds;
    }

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}

int main() {
    std::vector<std::string> times;
    times.push_back("07:17:08");
    times.push_back("03:20:15");
    times.push_back("01:30:00");
    times.push_back("08:45:30");
    std::string result = addTimes(times);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
