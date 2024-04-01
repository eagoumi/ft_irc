#include "Commands.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <stdio.h>

static std::string getJsonValue(std::string const& property, std::string const& jsonContent) {

    // {"access_token":"abcd","token_type":"bearer","expires_in":6765,"scope":"public","created_at":1711662095,"secret_valid_until":1713826844}
    std::string jsonValue;
    size_t propertyStartingIndex = jsonContent.find(property);
    if (propertyStartingIndex != std::string::npos) {
        size_t valueStartingIndex = propertyStartingIndex + property.length() + 2;
        valueStartingIndex += jsonContent[valueStartingIndex] == '\"' ? 1 : 0;
        for (; valueStartingIndex < jsonContent.length(); valueStartingIndex++) {
            if (jsonContent[valueStartingIndex] == '\"' || jsonContent[valueStartingIndex] == ',')
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


void Commands::location() {

    std::string token42 = get42Token(); if (token42.empty()) return ;

    std::string login = getNextParam().first; std::transform(login.begin(), login.end(), login.begin(), ::tolower);
    
    std::string locationCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login;

    std::string jsonContent = executeCmd(locationCmd);
    if (jsonContent == "{}") { sendResponse(fd, "This student isn't available on IBA7LAWN N IRC\n"); return; }
    
    std::string locationValue = getJsonValue("location", jsonContent);

    if (locationValue != "null") sendResponse(fd, "Currently on: " + locationValue + "\n");
    else sendResponse(fd, "Not available :c\n");
    // std::pair<std::string, std::string> defalutLogtimeDate = getLogTimeDate();
    // std::string begin_at, end_at;

    // if (_paramCounter >= 3) begin_at = getNextParam().first; else begin_at = defalutLogtimeDate.first;
    // if (_paramCounter >= 4) end_at = getNextParam().first; else end_at = defalutLogtimeDate.second;

    // if (checkDateFormat(begin_at) == false || checkDateFormat(end_at) == false || checkDateOrder(begin_at, end_at) == false)
    //     { sendResponse(fd, "The start or end date format is invalid please use YYYY-MM-DD.\n"); return; }

    // std::string locations_statsCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login + "/locations_stats\\?begin_at\\=";
    // locations_statsCmd += begin_at;
    // locations_statsCmd += "\\&end_at\\=" + incrementDate(end_at);


    // std::string jsonContent = executeCmd(locations_statsCmd);
    // if (jsonContent == "{}") { sendResponse(fd, "This student isn't available on IBA7LAWN N IRC\n"); return; }
    

    // /****************************************DEBUG****************************************/
    //     std::cout << "converted date : " << incrementDate("2024-02-01") << std::endl;
    //     std::cout << locations_statsCmd << std::endl;
    //     std::cout << jsonContent << std::endl;
    // /*************************************************************************************/
    
    // std::string token;
    // std::vector<std::string> loggedHours;
    // std::string currDate = incrementDate(begin_at, 0);
    // while (currDate.compare(incrementDate(end_at))) {
    //     loggedHours.push_back(getJsonValue(currDate, jsonContent));
    //     std::cout << "[currDate:"+currDate + "] = ["+getJsonValue(currDate, jsonContent)+"]" << std::endl;
    //     currDate = incrementDate(currDate);
    // }
    // std::string result = getHoursSum(loggedHours);
    // sendResponse(fd, "Logtime for " + login + " from " + begin_at + " to " + end_at + " is \U000023F2  :\n");
    // sendResponse(fd, "Result: " + result + " Hours \U0001F61C\n");
}