#include "Commands.hpp"

bool Commands::getMode(std::string letter){
    // this->currChannel = db->getChannel(this->getChannel());
    std::map<std::string, bool> modes;
    // puts("segv1");
    modes = db->getChannel(this->getChannel())->gettingModes(letter);
    // puts("segv2");
    for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
    {
        std::cout << it->first << "       " << it->second << std::endl;
    }
    if(modes[letter] == true)
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
    }
    return modeSeted;
}


void    Channel::initializeModes(std::string modeStr){
    modeS = modeStr;
    modeSeted["i"];
    modeSeted["t"];
    modeSeted["k"];
    modeSeted["o"];
    modeSeted["l"];
}



void Commands::mode()
{
    modeStr = getModeString();
    // if (command.size() < 2)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    if (db->getChannel(this->getChannel()) == NULL)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No such channel\n");
    }
    else if (existOperatorChannel(db->getUser(fd)->getNickName()) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");
    }
    else
    {
        db->getChannel(this->getChannel())->initializeModes(modeStr);
        db->getChannel(this->getChannel())->gettingModes("t"); 
        db->getChannel(this->getChannel())->gettingModes("i");
        db->getChannel(this->getChannel())->gettingModes("o");
        db->getChannel(this->getChannel())->gettingModes("l");
        db->getChannel(this->getChannel())->gettingModes("k");
    }
    // if(getMode("l") == true)
    // {
        // db->getChannel(this->getChannel())->setLimit(get)
    // }
    // for (std::map<std::string, bool>::iterator it = modeSeted.begin(); it != modeSeted.end(); it++)
    // {
    //     std::cout << it->first << "       " << it->second << std::endl;
    // }
    // if(gettingModes('t', mode, modeSeted) == true){

    // }

}
