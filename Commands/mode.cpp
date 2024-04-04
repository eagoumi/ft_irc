#include "Commands.hpp"
#include <cstddef>

/******************************/
/* MODIFIED BY TOFA7A SRY     */
/* I think This method        */
/* has to be moved to         */
/* Channel Class better       */
/* I moved the others too     */
/* that have `Channel::`      */
/******************************/
// bool Commands::getMode(std::string letter, std::string channelName)
// {
//     std::map<std::string, bool> modes;
//     modes = db->getChannel(channelName)->gettingModes(letter);
//     for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
//     {
//         std::cout << it->first << "       " << it->second << std::endl;
//     }
//     if (!modes.empty() && modes[letter] == true)
//         return true;
//     return false;
// }

void Commands::mode()
{
    puts("1");
    std::cout << "param number = " << _paramCounter << std::endl;

    std::string channelName = getNextParam().first;
    std::string modesStr = getNextParam().first;
    std::string modeArg;

    currChannel = db->getChannel(channelName);


    // if (command.size() < 2)
    // {
    //     sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + getCommand() + " :Not enough parameters\n");
    //     return;
    // }
    if (currChannel == NULL)
    {
        currUser->ServertoClients(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return ;
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :No such channel\n");
    }
    // else if (existOperatorChannel(db->getUser(fd)->getNickName(), channelName) == false)
    else if (currChannel->isUserOperator(currUser->getUserId()) == false)
    {
        currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(db->getUser(fd)->getNickName(), channelName));
        // sendResponse(fd, ":" + db->getUser(fd)->getNickName() /*client*/ + " " + channelName + " :You're not channel operator\n");
    }
    else {
        // currChannel->setModes(modesStr);
        char sign = '+'; //by default if no sign is specified within given modeStr
        for (size_t i = 0; i < modesStr.length(); i++) {

            modesStr[i] == '+' ? sign = '+' : (modesStr[i] == '-' ? sign = '-' : sign);

            char currModeLetter = modesStr[i];

            if (currModeLetter == 'i' || currModeLetter == 'k' || currModeLetter == 'l' || currModeLetter == 'o' || currModeLetter == 't') {

                if (sign == '+' && (currChannel->getMode(currModeLetter) == false || currModeLetter == 'k')) {
                    if (currModeLetter == 'k') {

                    }
                    else if (currModeLetter == 'l') {

                        modeArg = getNextParam().first;
                        size_t limit = static_cast<size_t>(atoi(modeArg.c_str()));
                        currChannel->setLimit(limit > 0 ? limit : 1);
                    }
                    else if (currModeLetter == 'o') {
                        modeArg = getNextParam().first;
                        User* Operator = db->existUser(modeArg);
                        if (!Operator) 
                            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :User does not exist\n");
                        else
                            currChannel->addOperator(Operator->getUserId());
                    }

                    currChannel->setMode(currModeLetter);
                }
                else if (sign == '-' && currChannel->getMode(currModeLetter) == true) {
                    if (currModeLetter == 'k') {

                    }
                    else if (currModeLetter == 'o') {
                        User* Operator = db->existUser(modeArg);
                        if (!Operator) 
                            sendResponse(fd, ":" + db->getUser(fd)->getNickName() + " " + channelName + " :User does not exist\n");
                        else {
                            if (currChannel->isUserOperator(Operator->getUserId()) == true &&
                                    currChannel->isUserOperator(currUser->getUserId()) == true)
                                currChannel->deleteOperator(Operator);
                        }
                    }

                    currChannel->removeMode(currModeLetter);
                }
            }
        }
    }



    /**************************/
    /* MODIFIED BY TOFA7A SRY */
    /* this one I think is    */
    /* Like a setter set modes*/
    /* it can be done in the  */
    /* Channel() constructer  */
    /* just once created      */
    /**************************/
    // else
    // {
    //     currChannel->initializeModes(modeStr);
    //     currChannel->gettingModes("t");
    //     currChannel->gettingModes("i");
    //     currChannel->gettingModes("o");
    //     currChannel->gettingModes("l");
    //     currChannel->gettingModes("k");
    // }

    /**************************/
    /* MODIFIED BY TOFA7A SRY */
    /* instead of appending   */
    /* here, the channel mode */
    /* string will be get from*/
    /* the channel            */
    /* and btw this seems     */
    /* setting modes without  */
    /* checking on operater   */
    /* permisions             */
    /**************************/
    // if(_paramCounter == 2) {

    //     // RPL_UMODEIS(Nickname, Mode)
    //     std::string mode = currChannel->getModeStr();
    //     //     mode = "+";
    //     // if(getMode("t", channelName) == true)
    //     //     mode.append("t");
    //     // if(getMode("i", channelName) == true)
    //     //     mode.append("i");
    //     // if(getMode("o", channelName) == true)
    //     //     mode.append("o");
    //     // if(getMode("l", channelName) == true)
    //     //     mode.append("l");
    //     // if(getMode("k", channelName) == true)
    //     //     mode.append("k");
    //     std::cout << "mode = "<< mode << std::endl;
    //     currUser->ServertoClients(RPL_UMODEIS(currUser->getNickName(), mode));
    // }

    /**************************/
    /* MODIFIED BY TOFA7A SRY */
    /* */
    /* */
    /* */
    /* */
    /**************************/
    // if (_paramCounter > 2) {
        // if (getMode("l", channelName) == true)
        // {
        //     size_t limit = static_cast<size_t>(atoi(modeArg.c_str()));
        //     if (limit > 0)
        //         currChannel->setLimit(limit);
        //     else
        //         currChannel->setLimit(1);
        // }

        // if (currChannel->getMode('l') == true) {

        //     size_t limit = static_cast<size_t>(atoi(modeArg.c_str()));
        //     if (limit > 0)
        //         currChannel->setLimit(limit);
        //     else
        //         currChannel->setLimit(1);
        // }
        
        // if (currChannel->getMode('o') == true) {

        //     User* Operator = db->existUser(modeArg);
        //     if (currChannel->isUserOperator(currUser->getUserId()) == false)
        //         currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        //     else if (currChannel->isUserOperator(currUser->getUserId()) == true)
        //         currChannel->addOperator(Operator->getUserId());
        // }
        // else if (currChannel->getMode('o') == false) {

        //     User* Operator = db->existUser(modeArg);
        //     if (currChannel->isUserOperator(Operator->getUserId()) == true &&
        //             currChannel->isUserOperator(currUser->getUserId()) == true)
        //         currChannel->deleteOperator(Operator);
        //     // currChannel->addOperator(modeArg);
        // }

        // User* fdOperator = db->existUser(modeArg);
        // if (getMode("o", channelName) == true && currChannel->isUserOperator(currUser->getUserId()) == false)
        //     currUser->ServertoClients(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        // else if (getMode("o", channelName) == true && currChannel->isUserOperator(currUser->getUserId()) == true)
        // {
        //     currChannel->addOperator(fdOperator->getUserId());
        // }
        // else if (getMode("o", channelName) == false && currChannel->isUserOperator(currUser->getUserId()) == true && currChannel->isUserOperator(fdOperator->getUserId()) == true)
        // {
        //     currChannel->deleteOperator(fdOperator);
        //     // currChannel->addOperator(modeArg);
        // }
    // }

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
