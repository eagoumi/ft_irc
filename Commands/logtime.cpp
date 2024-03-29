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

#define LEN_OF_TIME 8

static std::pair<std::string, std::string> getLogTimeDate() {
// The start or end date format is invalid please use YYYY-MM-DD.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    std::stringstream ss;
    std::string year, month, day;

    ss << tm.tm_year + 1900 << " " << tm.tm_mon + 1 << " " << tm.tm_mday;
    ss >> year; ss >> month; ss >> day;

    std::string begin_at = year + "-" + month + "-01";
    std::string end_at = year + "-" + month + "-" + day;

    return (std::make_pair(begin_at, end_at));
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
        puts("Unable to open process");
    while(fgets(content, 100, fp))
        jsonContent += content;
    return jsonContent;
}

static std::string get42Token() {

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
    }
    return token;
}

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

    std::ostringstream oss;
    oss << /*std::setw(2) << std::setfill('0') <<*/ hours;
    return oss.str();
}

void Commands::logtime() {

    std::string token42 = get42Token();

    std::string login = getNextParam().first; std::transform(login.begin(), login.end(), login.begin(), ::tolower);
    std::pair<std::string, std::string> defalutLogtimeDate = getLogTimeDate();
    std::string begin_at, end_at;

    if (_paramCounter >= 3) begin_at = getNextParam().first; else begin_at = defalutLogtimeDate.first;
    if (_paramCounter >= 4) end_at = getNextParam().first; else end_at = defalutLogtimeDate.second;

    std::string locations_statsCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login + "/locations_stats\\?begin_at\\=";
    locations_statsCmd += begin_at;
    locations_statsCmd += "\\&end_at\\=" + end_at;

    std::cout << locations_statsCmd << std::endl;

    std::string jsonContent = executeCmd(locations_statsCmd);
    std::cout << jsonContent << std::endl;
    if (jsonContent == "{}") { sendResponse(fd, "This student isn't available on IBA7LAWN N IRC\n"); return; }
    
    std::string token;
    std::vector<std::string> hours;
    std::stringstream stream(jsonContent);
    while(std::getline(stream, token, ',')) {
        std::string time = token.substr(token.find(":\"")+2, LEN_OF_TIME);
        hours.push_back(time);
    }
    std::string result = addTimes(hours);
    sendResponse(fd, "Logtime for " + login + " from " + begin_at + " to " + end_at + " is :\n");
    sendResponse(fd, "Result: " + result + " Hours \U0001F61C\n");
}