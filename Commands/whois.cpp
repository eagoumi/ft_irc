#include "Commands.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <ios>
#include <string>
#include <ctime>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <iomanip>
#include <stdio.h>


static std::string getJsonValue(std::string const& property, std::string const& jsonContent) {

    // {"access_token":"abcd","token_type":"bearer","expires_in":6765,"scope":"public","created_at":1711662095,"secret_valid_until":1713826844}
    std::string jsonValue;
    size_t propertyStartingIndex = jsonContent.find("\"" + property + "\"");
    if (propertyStartingIndex != std::string::npos) {
        size_t valueStartingIndex = propertyStartingIndex + property.length() + 3;;
        valueStartingIndex += jsonContent[valueStartingIndex] == '\"' ? 1 : 0;
        for (; valueStartingIndex < jsonContent.length(); valueStartingIndex++) {
            if (jsonContent[valueStartingIndex] == '\"' || jsonContent[valueStartingIndex] == ',')
                break ;
            jsonValue += jsonContent[valueStartingIndex];
        }
    }
    return jsonValue;
}

static std::string getJsonObjValue(std::string const& property, std::string const& jsonContent) {

    std::string jsonValue;
    size_t propertyStartingIndex = jsonContent.find("\"" + property + "\"");
    int curlyBracketsCounter = 0;
    if (propertyStartingIndex != std::string::npos) {
        size_t valueStartingIndex = propertyStartingIndex + property.length() + 3;
        if (jsonContent[valueStartingIndex] == '{') {
            for (; valueStartingIndex < jsonContent.length(); valueStartingIndex++) {
                if (jsonContent[valueStartingIndex] == '{')
                        curlyBracketsCounter++;
                else if (jsonContent[valueStartingIndex] == '}')
                    curlyBracketsCounter--;
                if (jsonContent[valueStartingIndex] == '}' && curlyBracketsCounter == 0)
                    break ;
                jsonValue += jsonContent[valueStartingIndex];
            }
        }
    }
    return jsonValue;
}

static std::vector<std::string> getJsonList(std::string const& property, std::string const& jsonContent) {

    std::vector<std::string> jsonObjList;
    int bracketsCounter = 0;
    int curlyBracketsCounter = 0;

    for (size_t propertyStartingIndex = 0; propertyStartingIndex < jsonContent.length();) {
        propertyStartingIndex = jsonContent.find(property, propertyStartingIndex);
        if (propertyStartingIndex == std::string::npos) break;
        size_t objListIndex = propertyStartingIndex + property.length() + 2;
        if (jsonContent[objListIndex] == '[') {
            std::string obj;
            while (objListIndex < jsonContent.length()) {
                if (jsonContent[objListIndex] == '[')
                    bracketsCounter++;
                else if (jsonContent[objListIndex] == ']')
                    bracketsCounter--;
                if (jsonContent[objListIndex] == ']' && bracketsCounter == 0)
                    return jsonObjList;
                else if (jsonContent[objListIndex] == '{')
                    curlyBracketsCounter++;
                else if (jsonContent[objListIndex] == '}')
                    curlyBracketsCounter--;
                if (curlyBracketsCounter >= 1)
                    obj += jsonContent[objListIndex];
                else if (curlyBracketsCounter == 0 && !obj.empty())
                {
                    jsonObjList.push_back(obj);
                    obj.clear();
                }
                objListIndex++;
            }
        }
        propertyStartingIndex += property.length();
    }
    return jsonObjList;
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


void Commands::whois() {

    std::string token42 = get42Token(); if (token42.empty()) return ;

    std::string login = getNextParam().first; std::transform(login.begin(), login.end(), login.begin(), ::tolower);
    
    /*User Validation*/
    std::string userCmd = "curl  -sH \"Authorization: Bearer " + token42 + "\" https://api.intra.42.fr/v2/users/" + login;
    std::string jsonContent = executeCmd(userCmd);
    if (jsonContent == "{}") { sendResponse(fd, "This student isn't available on IBA7LAWN N IRC\n\n"); return; }

    std::string imageLink = getJsonValue("medium", jsonContent);
    if (imageLink != "null") {

        /*Display Image*/
        executeCmd("curl -s " + imageLink + " > image.png");
        int saved_stdout = dup(1);
        dup2(currUser->getUserId(), 1);
        system("curl -s https://iterm2.com/utilities/imgcat | bash /dev/stdin image.png 2> /dev/null");
        dup2(saved_stdout, 1);
        close(saved_stdout);

        /*Display DATA*/
        std::string displayName      = getJsonValue("displayname", jsonContent);
        std::string email            = getJsonValue("email", jsonContent);
        std::string correctionPoints = getJsonValue("correction_point", jsonContent);
        std::string poolDate         = getJsonValue("pool_month", jsonContent) + "/" + getJsonValue("pool_year", jsonContent);
        std::string campusCity       = getJsonValue("name", getJsonList("campus", jsonContent)[0]) ;
        std::string blackholeAT;
        
        size_t width = 25;
        std::ostringstream os;
        os << std::setw(width) << std::left << "Name"             << "▒ " << displayName     << " 🧑🏻👩🏻\n";
        os << std::setw(width) << std::left << "Email"            << "▒ " << email           << " 📨\n";
        os << std::setw(width) << std::left << "Correction Points"<< "▒ " << correctionPoints<< " 🟡\n";
        os << std::setw(width) << std::left << "Campus City"      << "▒ " << campusCity      << " 🏫\n";
        os << std::setw(width) << std::left << "Pool Date"        << "▒ " << poolDate        << " 🏊\n";

        std::vector<std::string> objList = getJsonList("cursus_users", jsonContent);
        for (size_t i = 0; i < objList.size(); i++) {
            std::string level = getJsonValue("level", objList[i]);
            std::string cursusObj = getJsonObjValue("cursus", objList[i]);
            std::string cursusName = getJsonValue("name", cursusObj);
            /*This is What we call it HARD CODE*/
            if (cursusName == "Piscine C décloisonnée")
                os << std::setw(width+2) << std::left << cursusName << "▒ " << level << " 📈\n";
            else
                os << std::setw(width) << std::left << cursusName << "▒ " << level << " 📈\n";
            if (cursusName == "42cursus")
                blackholeAT = getJsonValue("blackholed_at", objList[i]);
        }
        if (blackholeAT != "null")
            os << std::setw(width) << std::left << "Will Be Blackholed At" << "▒ " << blackholeAT.substr(0, 10) + " 🌀\n";
        else
            os << std::setw(width) << std::left << "Will Be Blackholed At" << "▒ " << "You Escaped The Matrix 💊😎\n";

        sendResponse(fd, os.str() + "\n");
    }
    else 
        sendResponse(fd, "Not available :c\n\n");
}