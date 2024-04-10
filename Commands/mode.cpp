#include "Commands.hpp"
#include <cstddef>

void Commands::mode()
{
    size_t i;
    std::string channelName = getNextParam().first;
    std::string modesStr = getNextParam().first;
    std::vector<std::string> modeArg = getNextParam().second;
    currChannel = db->getChannel(channelName);
    if (currChannel == NULL)
    {
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return;
    }
    else if (_paramCounter == 2)
    {
        std::stringstream ss;
        std::string args = " ";
        std::string currModes = currChannel->getModes();
        size_t itKey = currChannel->getModes().find('k');
        size_t itLimit = currChannel->getModes().find('l');

        if (itKey != std::string::npos)
            args += (currChannel->getKey() + " ");

        if (itLimit != std::string::npos)
        {
            ss << currChannel->getLimit();
            args += (ss.str() + " ");
        }
        _logger.ServertoClient(RPL_CHANNELMODEIS(currUser->getNickName(), channelName, currChannel->getModes(), args));
    }
    else
    {
        char sign = '+';
        if (currChannel->isUserOperator(currUser->getUserId()) == false)
            _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        else
        {
            for (i = 0; i < modesStr.length(); i++)
            {
                modesStr[i] == '+' ? sign = '+' : (modesStr[i] == '-' ? sign = '-' : sign);

                char currModeLetter = modesStr[i];
                if (currModeLetter == 'i' || currModeLetter == 'k' || currModeLetter == 'l' || currModeLetter == 'o' || currModeLetter == 't')
                {
                    if (sign == '+' && (currChannel->getMode(currModeLetter) == false || currModeLetter == 'k' || currModeLetter == 'o' || currModeLetter == 'l'))
                    {
                        if (currModeLetter == 'k')
                        {
                            currChannel->setKey(modeArg.front());
                            modeArg.erase(modeArg.begin());
                        }
                        else if (currModeLetter == 'l')
                        {
                            size_t limit = static_cast<size_t>(atoi(modeArg.front().c_str()));
                            currChannel->setLimit(limit > 0 ? limit : 1);
                            modeArg.erase(modeArg.begin());
                        }
                        else if (currModeLetter == 'o' && _paramCounter == 3)
                        {
                            _logger.ServertoClient(ERR_SPECIFYPARAM(currUser->getNickName(), channelName));
                            continue;
                        }

                        else if (currModeLetter == 'o' && _paramCounter == 4)
                        {
                            User *Operator = db->existUser(modeArg.front());
                            if (!Operator)
                            {
                                _logger.ServertoClient(ERR_NOUSERS(currUser->getNickName(), channelName));
                                continue;
                            }
                            else if (currChannel->isNickExist(modeArg.front()) == false)
                            {
                                _logger.ServertoClient(ERR_USERNOTINCHANNEL(currUser->getNickName(), modeArg.front(), channelName));
                                continue;
                            }
                            else
                            {
                                currChannel->addOperator(Operator->getUserId());
                                SendMessageToMembers(currChannel, currUser, "MODE " + channelName + " +o " + modeArg.front());
                                modeArg.erase(modeArg.begin());
                            }
                        }

                        currChannel->setMode(currModeLetter);
                    }
                    else if (sign == '-' && currChannel->getMode(currModeLetter) == true)
                    {
                        if (currModeLetter == 'k')
                        {
                            currChannel->clearKey();
                        }
                        else if (currModeLetter == 'o' && _paramCounter == 3)
                        {
                            _logger.ServertoClient(ERR_SPECIFYPARAM(currUser->getNickName(), channelName));
                            continue;
                        }

                        else if (currModeLetter == 'o' && _paramCounter == 4)
                        {
                            User *Operator = db->existUser(modeArg.front());
                            if (!Operator)
                            {
                                _logger.ServertoClient(ERR_NOUSERS(currUser->getNickName(), channelName));
                                continue;
                            }
                            else if (currChannel->isNickExist(modeArg.front()) == false)
                            {
                                _logger.ServertoClient(ERR_USERNOTINCHANNEL(currUser->getNickName(), modeArg.front(), channelName));
                                // modeArg.erase(modeArg.begin());
                                continue; 
                            }
                            else
                            {
                                if (currChannel->isUserOperator(Operator->getUserId()) == true && currChannel->isUserOperator(currUser->getUserId()) == true)
                                {
                                    currChannel->deleteOperator(Operator);
                                    SendMessageToMembers(currChannel, currUser, "MODE " + channelName + " -o " + modeArg.front());
                                    modeArg.erase(modeArg.begin());
                                }
                            }
                        }
                        currChannel->removeMode(currModeLetter);
                    }
                }
            }
        }
    }
}
