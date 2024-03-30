#include "Commands.hpp"


bool Commands::getMode(std::string letter){
    std::map<std::string, bool> modes;
    modes = db->getChannel(this->getChannel())->gettingModes(letter[0]);
    // for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
    // {
    //     std::cout << it->first << "       " << it->second << std::endl;
    // }
    // std::cout << "HOOOOW = " << modes[letter] << " THAT'S WHYYYY " << letter << std::endl;
    if(modes[letter] == true)
        return true;
    return false;

}


std::map<std::string, bool> Channel::gettingModes(char toFind)
{
    std::string::size_type it = mode.find(toFind);
    std::string toFindStr(1, toFind);
    if (it != std::string::npos)
    {
        it--;
        if (mode[it] == '+' || (mode[0] == '+' && mode[it] != '-'))
        {
            modeSeted[toFindStr] = true;
        }        
    }
    return modeSeted;
}


void    Channel::initializeModes(std::string modeStr){
    mode = modeStr;
    modeSeted["i"];
    modeSeted["t"];
    modeSeted["k"];
    modeSeted["o"];
    modeSeted["l"];
}



void Commands::mode()
{
    modeStr = getModeString();
    if (command.size() < 2)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
        return;
    }
    if (db->getChannel(this->getChannel()) == NULL)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getChannel() + " :No such channel\n");
    }
    else if (existOperatorChannel(db->getUser(fd)->getNickName()) == false)
    {
        sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + getChannel() + " :You're not channel operator\n");
    }
    db->getChannel(this->getChannel())->initializeModes(modeStr);
    db->getChannel(this->getChannel())->gettingModes('t'); 
    db->getChannel(this->getChannel())->gettingModes('i');
    db->getChannel(this->getChannel())->gettingModes('o');
    db->getChannel(this->getChannel())->gettingModes('l');
    db->getChannel(this->getChannel())->gettingModes('k');
    // for (std::map<std::string, bool>::iterator it = modeSeted.begin(); it != modeSeted.end(); it++)
    // {
    //     std::cout << it->first << "       " << it->second << std::endl;
    // }
    // if(gettingModes('t', mode, modeSeted) == true){

    // }

}
