#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iostream>
#include <iterator>
#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>

std::string convertToTimeString(time_t unixTime) {
    std::ostringstream oss;
    std::tm* tm = localtime(&unixTime);
    oss << std::setw(2) << std::setfill('0') << tm->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << tm->tm_min << ":"
        << std::setw(2) << std::setfill('0') << tm->tm_sec;
    return oss.str();
}

time_t convertToUnixTime(const std::string& timeStr) {
    std::tm tm = {};
    std::istringstream iss(timeStr);
    iss >> std::get_time(&tm, "%H:%M:%S");
    return mktime(&tm);
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
  printf("now: %d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  std::string begin_at = std::to_string(tm.tm_year + 1900) + "-" + std::to_string(tm.tm_mon + 1) + "-01";
  std::cout << begin_at << std::endl;


    std::string command = "curl  -sH \"Authorization: Bearer 2da25f5f7f4ddc8ca881650943f96e7a5e64cdb32c1a95458075db1e90e4fc7c\" https://api.intra.42.fr/v2/users/iltafah/locations_stats?begin_at=";//2024-03-01";
    std::string jsonContent("");
    FILE *fp;
    
    command += begin_at;
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

    std::cout << jsonContent << std::endl;

    std::stringstream stream(jsonContent);

    std::string token;
    time_t epoch = 0;
    while(std::getline(stream, token, ',')) {
        std::string time = token.substr(token.find(":\"")+2, 8);
        // if ( strptime(time.c_str(), "%H:%M:%S", &tm) != NULL )
        //     epoch += mktime(&tm);
        epoch += convertToUnixTime(time);
        std::cout << epoch << std::endl;
    }
    std::cout << convertToTimeString(epoch);

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