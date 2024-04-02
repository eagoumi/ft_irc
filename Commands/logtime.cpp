#include "Commands.hpp"
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

static struct tm getTimeStruct(std::string const& date) {

    char dash;
    std::stringstream ss(date);

    struct tm dateData;

    ss >> dateData.tm_year >> dash >> dateData.tm_mon >> dash >> dateData.tm_mday;
    dateData.tm_year -= 1900; dateData.tm_mon -= 1;
    dateData.tm_hour = 1;
    dateData.tm_min = 0;
    dateData.tm_sec = 0;

    return dateData;
}

static bool checkDateOrder(const std::string& begin_at, const std::string& end_at) {

    struct tm beginDate = getTimeStruct(begin_at);
    struct tm endDate = getTimeStruct(end_at);

    mktime(&beginDate); mktime(&endDate);
    if (
        beginDate.tm_year <= endDate.tm_year &&
        beginDate.tm_mon <= endDate.tm_mon   &&
        beginDate.tm_mday <= endDate.tm_mday
    )
        return true;
    return false;
}

static bool checkDateFormat(const std::string& dateString) {

    std::stringstream ss(dateString);

    struct tm date = getTimeStruct(dateString);
    struct tm roundedDate = date;

    mktime(&roundedDate);
    if (
        date.tm_year == roundedDate.tm_year &&
        date.tm_mon == roundedDate.tm_mon   &&
        date.tm_mday == roundedDate.tm_mday
    )   
        return true; 
    return false;
}

static std::string incrementDate(const std::string& dateString, size_t daysToAdd = 1) {

    char dash;
    std::stringstream ss(dateString);

    struct tm date;
    ss >> date.tm_year >> dash >> date.tm_mon >> dash >> date.tm_mday;
    date.tm_year -= 1900; date.tm_mon -= 1;
    date.tm_hour = 1;            // Hour
    date.tm_min = 0;             // Minute
    date.tm_sec = 0;             // Second

    // Convert to unix time, the date will be fixed and rounded
    time_t unix_time = mktime(&date);

    // Increment the day
    date.tm_mday += daysToAdd;

    // Convert to unix time, the date will be fixed and rounded
    unix_time = mktime(&date);

    // Convert back to tm struct to extract date parts
    struct tm* new_date = gmtime(&unix_time);

    std::ostringstream incrementedDate;
        incrementedDate << new_date->tm_year + 1900
        << "-" << std::setw(2) << std::setfill('0') << new_date->tm_mon + 1 
        << "-" << std::setw(2) << std::setfill('0') << new_date->tm_mday; 

    return incrementedDate.str();
}

static std::pair<std::string, std::string> getLogTimeDate() {

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    std::stringstream ss;
    std::string year, month, day;

    ss << tm.tm_year + 1900 << " " << tm.tm_mon + 1 << " " << tm.tm_mday;
    ss >> year; ss >> month; ss >> day;

    std::ostringstream begin_at, end_at;
    begin_at << year << "-" << std::setw(2) << std::setfill('0') << month << "-01";
    end_at << year << "-" << std::setw(2) << std::setfill('0') << month << "-" << std::setw(2) << std::setfill('0') << day; 

    return (std::make_pair(begin_at.str(), end_at.str()));
}

static std::string getJsonValue(std::string const& property, std::string const& jsonContent) {

    // {"access_token":"abcd","token_type":"bearer","expires_in":6765,"scope":"public","created_at":1711662095,"secret_valid_until":1713826844}
    std::string jsonValue;
    size_t propertyStartingIndex = jsonContent.find(property);
    if (propertyStartingIndex != std::string::npos) {
        size_t valueStartingIndex = propertyStartingIndex + property.length() + 2;
        valueStartingIndex += jsonContent[valueStartingIndex] == '\"' ? 1 : 0;
        for (; valueStartingIndex < jsonContent.length(); valueStartingIndex++) {
            if (jsonContent[valueStartingIndex] == '\"')
                break ;
            jsonValue += jsonContent[valueStartingIndex];
        }
    }
    return jsonValue;
}

static std::string executeCmd(std::string const& cmd) {

    FILE *fp;
    char content[100];
    std::string jsonContent("");

    fp = popen(cmd.c_str(), "r");
    if(fp == NULL)
        puts("Go install curl CLI FOR GOD SAKE");
    while(fgets(content, 100, fp))
        jsonContent += content;
    pclose(fp);
    return jsonContent;
}

std::string Commands::get42Token() {

    char *uid = NULL;
    char *secret = NULL;
    static std::string token;
    static std::time_t tokenValidUntil = std::time(0);
    std::string tokenCommand;
    std::time_t currTime;

    currTime = std::time(0);
    if (currTime >= tokenValidUntil) {

        uid = std::getenv("UID42"); secret = std::getenv("SECRET42");
        if (uid && secret) {
            
            tokenCommand = "curl -s -X POST --data \"grant_type=client_credentials&client_id=" + std::string(uid) + "&client_secret=" + secret + "\" https://api.intra.42.fr/oauth/token";
            std::string jsonContent = executeCmd(tokenCommand);

            token = getJsonValue("access_token", jsonContent);
            tokenValidUntil = atol(getJsonValue("secret_valid_until", jsonContent).c_str());
        }
        else
            sendResponse(fd, "Please export your UID & SECRET, Go Get it from Intra or Ask Tofa7a.\n");
    }
    return token;
}

static std::string getHoursSum(const std::vector<std::string>& loggedHours) {

    int totalSeconds = 0;
    for (size_t i = 0; i < loggedHours.size(); ++i) {
        std::istringstream iss(loggedHours[i]);
        int hours, minutes, seconds;
        char colon;
        iss >> hours >> colon >> minutes >> colon >> seconds;
        totalSeconds += hours * 3600 + minutes * 60 + seconds;
    }

    int hours = totalSeconds / 3600;

    std::ostringstream os;
    os << hours;
    return os.str();
}

void Commands::logtime() {

    std::string token42 = get42Token(); if (token42.empty()) return ;

    std::string login = getNextParam().first; std::transform(login.begin(), login.end(), login.begin(), ::tolower);
    std::pair<std::string, std::string> defalutLogtimeDate = getLogTimeDate();
    std::string begin_at, end_at;

    if (_paramCounter >= 3) begin_at = getNextParam().first; else begin_at = defalutLogtimeDate.first;
    if (_paramCounter >= 4) end_at = getNextParam().first; else end_at = defalutLogtimeDate.second;

    if (checkDateFormat(begin_at) == false || checkDateFormat(end_at) == false || checkDateOrder(begin_at, end_at) == false)
        { sendResponse(fd, "The start or end date format is invalid please use YYYY-MM-DD.\n"); return; }

    std::string userCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login;
    std::string jsonContent = executeCmd(userCmd);
    if (jsonContent == "{}") { sendResponse(fd, "This student isn't available on IBA7LAWN N IRC\n"); return; }
    
    std::string locations_statsCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login + "/locations_stats\\?begin_at\\=";
    locations_statsCmd += begin_at;
    locations_statsCmd += "\\&end_at\\=" + incrementDate(end_at);

    jsonContent = executeCmd(locations_statsCmd);
    if (jsonContent == "{}") {
        sendResponse(fd, "Logtime for " + login + " from " + begin_at + " to " + end_at + " is \U000023F2  :\n");
        sendResponse(fd, "Result: 0 Hours \U0001F61C\n"); return ;
    }
    /****************************************DEBUG****************************************/
        std::cout << "converted date : " << incrementDate("2024-02-01") << std::endl;
        std::cout << locations_statsCmd << std::endl;
        std::cout << jsonContent << std::endl;
    /*************************************************************************************/
    
    std::string token;
    std::vector<std::string> loggedHours;
    std::string currDate = incrementDate(begin_at, 0);
    while (currDate.compare(incrementDate(end_at))) {
        loggedHours.push_back(getJsonValue(currDate, jsonContent));
        std::cout << "[currDate:"+currDate + "] = ["+getJsonValue(currDate, jsonContent)+"]" << std::endl;
        currDate = incrementDate(currDate);
    }
    std::string result = getHoursSum(loggedHours);
    sendResponse(fd, "Logtime for " + login + " from " + begin_at + " to " + end_at + " is \U000023F2  :\n");
    sendResponse(fd, "Result: " + result + " Hours \U0001F61C\n");
}