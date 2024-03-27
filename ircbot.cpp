#include <string>
#include <ctime>
#include <iterator>
#include <locale>
#include <iostream>
#include <sstream>

// std::string convertToTimeString(time_t unixTime) {
//     std::ostringstream oss;
//     std::tm* tm = localtime(&unixTime);
//     oss << std::setw(2) << std::setfill('0') << tm->tm_hour << ":"
//         << std::setw(2) << std::setfill('0') << tm->tm_min << ":"
//         << std::setw(2) << std::setfill('0') << tm->tm_sec;
//     return oss.str();
// }

// time_t convertToUnixTime(const std::string& timeStr) {
//     std::tm tm = {};
//     std::istringstream iss(timeStr);
//     iss >> std::get_time(&tm, "%H:%M:%S");
//     return mktime(&tm);
// }

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
    oss << std::setw(2) << std::setfill('0') << hours; //<< ":"
        // << std::setw(2) << std::setfill('0') << minutes << ":"
        // << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}


int main()
{
    // curl  -H "Authorization: Bearer fe3f0ba1cc0c19c65b6b89d63d177ba74a5efed616a135ab6c52118eea64fe59" "https://api.intra.42.fr/v2/users/iltafah/locations_stats?begin_at=2024-03-01" > uwu.json
    // curl  -H "Authorization: Bearer fe3f0ba1cc0c19c65b6b89d63d177ba74a5efed616a135ab6c52118eea64fe59" "https://api.intra.42.fr/v2/users/iltafah/locations?page[number]=1&page[size]=100&range%5Bbegin_at%5D=2024-03-01,2024-03-26"
    // curl  -H "Authorization: Bearer fe3f0ba1cc0c19c65b6b89d63d177ba74a5efed616a135ab6c52118eea64fe59" "https://api.intra.42.fr/v2/users/iltafah/locations?page[number]=1&page[size]=100&range%5Bbegin_at%5D=2024-03-01T00:00:00.483Z,2024-03-26T22:40:02.483Z"
    // std::string command = "curl  -H \"Authorization: Bearer fe3f0ba1cc0c19c65b6b89d63d177ba74a5efed616a135ab6c52118eea64fe59\" https://api.intra.42.fr/v2/users/iltafah";
    // FILE *fp = popen(command.c_str(), "r");


    time_t t = time(NULL);
  struct tm tm = *localtime(&t);
//   printf("now: %d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  char now[20];
  sprintf(now, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  std::string begin_at = std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) + "-01";
  std::string end_at = std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) + "-" + std::to_string(tm.tm_mday);
//   std::cout << begin_at << std::endl;


    std::string command = "curl  -sH \"Authorization: Bearer 7e9338e97edf632650504cbb0c3c41f96f46938ff4d3a3363ab9b87531362a2c\" https://api.intra.42.fr/v2/users/iltafah/locations_stats?begin_at=";//2024-03-01";
    command += begin_at;

    std::string jsonContent("");
    FILE *fp;
    
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        puts("Unable to open process");
        return(1);
    }
    char myString[100];
    while(fgets(myString, 100, fp)) {
        // printf("%s", myString);
        jsonContent += myString;
    }

    // std::cout << jsonContent << std::endl;

    std::stringstream stream(jsonContent);

    std::string token;
    std::vector<std::string> tokens;
    // time_t epoch = 0;
    while(std::getline(stream, token, ',')) {
        std::string time = token.substr(token.find(":\"")+2, 8);
        tokens.push_back(time);
        // if ( strptime(time.c_str(), "%H:%M:%S", &tm) != NULL )
        //     epoch += mktime(&tm);
        // epoch += convertToUnixTime(time);
        // std::cout << epoch << std::endl;
    }
    std::string result = addTimes(tokens);
    std::cout << "Logtime for iltafah from " + begin_at + " to " + end_at + " is :" << std::endl;
    std::cout << "Result: " << result << " Hours \U0001F61C" << std::endl;
    // std::cout << convertToTimeString(epoch);

    //     if(fp == NULL)
    //     {
    //         puts("Unable to open process");
    //         return(1);
    //     }
    // for (int index = 1; index < 30; index++) 
    // {
    //     std::string jsonContent("");
    //     FILE *fp;
    //     char myString[100];
    //     // int index = 1;
    //     std::string command = "curl  -sH \"Authorization: Bearer f3cb17d8b828b02b564dd5c08c1604ca681ac31eb04d8fa697c7b0759d37cbf4\" \"https://api.intra.42.fr/v2/campus/16/users?page[size]=100&page[number]=";
        
    //     std::string result = command + std::to_string(index) + "&sort=created_at\"";
    //     // command.push_back(index + '0');
    //     // command.push_back('\"');

    //     fp = popen(result.c_str(), "r");
    //     if(fp == NULL)
    //     {
    //         puts("Unable to open process");
    //         return(1);
    //     }
    //     while(fgets(myString, 100, fp)) {
    //         // printf("%s", myString);
    //         jsonContent += myString;
    //     }
    //     if (jsonContent.find("iltafah") !=std::string::npos ) {
    //         std::cout << "found" << std::endl;
    //         exit(0);
    //     }
    //     pclose(fp);
    //     // system(command.c_str());
    // }
}
//62225