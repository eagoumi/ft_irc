#include "Commands.hpp"
#include <string>
#include <ctime>
#include <iterator>
#include <locale>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdio.h>
// extern "C" FILE *popen(const char *command, const char *mode);

static std::string addTimes(const std::vector<std::string>& times) {

    int totalSeconds = 0;
    for (size_t i = 0; i < times.size(); ++i) {
        std::istringstream iss(times[i]);
        int hours, minutes, seconds;
        char colon;
        iss >> hours >> colon >> minutes >> colon >> seconds;
        totalSeconds += hours * 3600 + minutes * 60 + seconds;
    }

    int hours = totalSeconds / 3600;
    // int minutes = (totalSeconds % 3600) / 60;
    // int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours; //<< ":"
        // << std::setw(2) << std::setfill('0') << minutes << ":"
        // << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}

void Commands::logtime() {

    getNextParam();

    std::string login = getNextParam()[0];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
//   printf("now: %d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
//   char now[20];
//   sprintf(now, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    std::stringstream ss;
    std::string year;
    std::string month;
    std::string day;

    ss << tm.tm_year + 1900;
    ss >> year;
    ss << tm.tm_mon + 1;
    ss >> month;
    ss << tm.tm_mon + 1;
    ss >> day;

  std::string begin_at = /*std::to_string(tm.tm_year + 1900)*/year + "-" + month + "-01";
  std::string end_at = year + "-" + month + "-" + day;
//   std::cout << begin_at << std::endl;


    std::string command = "curl  -sH \"Authorization: Bearer 7e9338e97edf632650504cbb0c3c41f96f46938ff4d3a3363ab9b87531362a2c\" https://api.intra.42.fr/v2/users/" + login + "/locations_stats?begin_at=";//2024-03-01";
    command += begin_at;

    std::string jsonContent("");
    FILE *fp;
    
    fp = popen(command.c_str(), "r");
    if(fp == NULL)
    {
        puts("Unable to open process");
        // return(1);
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
}