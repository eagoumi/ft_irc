#include "Commands.hpp"

bool Commands::getMode(std::string letter, std::string channelName)
{
    std::map<std::string, bool> modes;
    modes = db->getChannel(channelName)->gettingModes(letter);
    for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
    {
        std::cout << it->first << "       " << it->second << std::endl;
    }
    if (!modes.empty() && modes[letter] == true)
        return true;
    return false;
}

std::map<std::string, bool> Channel::gettingModes(std::string toFind)
{

    std::string::size_type it = modeS.find(toFind);

    if (it != std::string::npos)
    {
        it--;
        if (modeS[it] == '+' || (modeS[0] == '+' && modeS[it] != '-'))
        {
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


void Commands::mode()
{
    puts("1");
    std::cout << "param number = " << _paramCounter << std::endl;
    std::string channelName = getNextParam().first;
    modeStr = getNextParam().first;
    std::string modeArg = getNextParam().first;
    currChannel = db->getChannel(channelName);
    // if (command.size() < 2)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    puts("2");
    if (db->getChannel(channelName) == NULL)
    {
    puts("3");
        currUser->ServertoClients(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return ;
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
    }
    // else if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false)
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
    puts("4");
        currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
    }
    else
    {
    puts("5");
        currChannel->initializeModes(modeStr);
        currChannel->gettingModes("t");
        currChannel->gettingModes("i");
        currChannel->gettingModes("o");
        currChannel->gettingModes("l");
        currChannel->gettingModes("k");
    }
    puts("6");
    if(_paramCounter == 2)
    {
    puts("7");
        // RPL_UMODEIS(Nickname, Mode)
        std::string mode;
            mode = "+";
        if(getMode("t", channelName) == true)
            mode.append("t");
        if(getMode("i", channelName) == true)
            mode.append("i");
        if(getMode("o", channelName) == true)
            mode.append("o");
        if(getMode("l", channelName) == true)
            mode.append("l");
        if(getMode("k", channelName) == true)
            mode.append("k");
        std::cout << "mode = "<< mode << std::endl;
        currUser->ServertoClients(RPL_UMODEIS(currUser->getNickName(), mode));
    }
    if (_paramCounter > 2)
    {
    puts("8");
        if (getMode("l", channelName) == true)
        {
            size_t limit = static_cast<size_t>(atoi(modeArg.c_str()));
            if (limit > 0)
                currChannel->setLimit(limit);
            else
                currChannel->setLimit(1);
        }
        User* fdOperator = db->existUser(modeArg);
        if (getMode("o", channelName) == true && currChannel->isUserOperator(currUser->getUserId()) == false)
            currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        else if (getMode("o", channelName) == true && currChannel->isUserOperator(currUser->getUserId()) == true)
        {
            currChannel->addOperator(fdOperator->getUserId());
        }
        else if (getMode("o", channelName) == false && currChannel->isUserOperator(currUser->getUserId()) == true && currChannel->isUserOperator(fdOperator->getUserId()) == true)
        {
            currChannel->deleteOperator(fdOperator);
            // currChannel->addOperator(modeArg);
        }
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
