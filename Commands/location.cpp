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

    std::string token42 = get42Token(); if (token42.empty()) { sendResponse(fd, "$UID42 or $SECRET42 are invalid\n"); return ; } ;

    std::string login = getNextParam().first; std::transform(login.begin(), login.end(), login.begin(), ::tolower);
    
    std::string locationCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login;

    std::string jsonContent = executeCmd(locationCmd);
    if (jsonContent == "{}") { sendResponse(fd, "This student isn't available on IBA7LAWN N IRC\n\n"); return; }
    
    std::string locationValue = getJsonValue("location", jsonContent);

    if (locationValue != "null") sendResponse(fd, "Currently on: " + locationValue + " 🖥️\n\n");
    else sendResponse(fd, "Not available :c\n\n");
}