#include "Commands.hpp"

bool Commands::getMode(std::string letter, std::string channelName)
{
    // this->currChannel = db->getChannel(this->channelName);
    std::map<std::string, bool> modes;
    // std::string channelName = getNextParam().first;
    modes = db->getChannel(channelName)->gettingModes(letter);
    for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
    {
        std::cout << it->first << "       " << it->second << std::endl;
    }
    if (modes[letter] == true)
        return true;
    return false;
}

std::map<std::string, bool> Channel::gettingModes(std::string toFind)
{
    // size_t it;
    // std::cout << "test1" << std::endl;
    // if(modeS != "")
    // {
    //     std::cout << "cause of segfaulting = " << toFind << "     and     " << this->modeS << std::endl;
    //     return modeSeted;
    // }

    std::string::size_type it = modeS.find(toFind);

    if (it != std::string::npos)
    {
        it--;
        if (modeS[it] == '+' || (modeS[0] == '+' && modeS[it] != '-'))
        {
            puts("test3");
            modeSeted[toFind] = true;
        }
        else
        {
            puts("test4");
            modeSeted[toFind] = false;
        }
    }
    return modeSeted;
}

void Channel::initializeModes(std::string modeStr)
{
    modeS = modeStr;
    modeSeted["i"];
    modeSeted["t"];
    modeSeted["k"];
    modeSeted["o"];
    modeSeted["l"];
}

// std::string Commands::tolower(std::string modeStr)
// {
//     for(int i = 0; modeStr[i]; i++)
//     {
//          tolower(modeStr[i]);
//     }
// }

void Commands::mode()
{
    std::string channelName = getNextParam().first;
    modeStr = getNextParam().first;
    std::string modeArg = getNextParam().first;
    currChannel = db->getChannel(channelName);
    // if (command.size() < 2)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    if (db->getChannel(channelName) == NULL)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
        
    }
    else if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
    }
    else
    {
        currChannel->initializeModes(modeStr);
        currChannel->gettingModes("t");
        currChannel->gettingModes("i");
        currChannel->gettingModes("o");
        currChannel->gettingModes("l");
        currChannel->gettingModes("k");
    }
    if (getMode("l", channelName) == true)
    {
        size_t limit = static_cast<size_t>(atoi(modeArg.c_str()));
        if (limit > 0)
            currChannel->setLimit(limit);
        else
            currChannel->setLimit(1);
    }
    if (getMode("o", channelName) == true && existMemberChannel(modeArg, channelName) == true)
    {
        size_t fdo = existUser(modeArg);
        currChannel->addOperator(fdo);
    }

    // if(getMode("l") == true)
    // {
    // db->getChannel(this->channelName)->setLimit(get)
    // }
    // for (std::map<std::string, bool>::iterator it = modeSeted.begin(); it != modeSeted.end(); it++)
    // {
    //     std::cout << it->first << "       " << it->second << std::endl;
    // }
    // if(gettingModes('t', mode, modeSeted) == true){

    // }
}
