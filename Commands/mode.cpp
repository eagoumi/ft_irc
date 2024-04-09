#include "Commands.hpp"
#include <cstddef>


void Commands::mode()
{
    std::string channelName = getNextParam().first;
    std::string modesStr = getNextParam().first;
    std::string modeArg;
    currChannel = db->getChannel(channelName);
    if (currChannel == NULL)
    {
        _logger.ServertoClient(ERR_NOSUCHCHANNEL(currUser->getNickName(), channelName));
        return;
    }
    else if (_paramCounter == 2)
    {
        _logger.ServertoClient(RPL_CHANNELMODEIS(currUser->getNickName(), channelName, currChannel->getModes()));
    }
    else
    {
        char sign = '+';
        if (currChannel->isUserOperator(currUser->getUserId()) == false)
            _logger.ServertoClient(ERR_CHANOPRIVSNEEDED(currUser->getNickName(), channelName));
        else
        {
            for (size_t i = 0; i < modesStr.length(); i++)
            {
                modesStr[i] == '+' ? sign = '+' : (modesStr[i] == '-' ? sign = '-' : sign);

                char currModeLetter = modesStr[i];
                if (currModeLetter == 'i' || currModeLetter == 'k' || currModeLetter == 'l' || currModeLetter == 'o' || currModeLetter == 't')
                {
                    if (sign == '+' && (currChannel->getMode(currModeLetter) == false || currModeLetter == 'k' || currModeLetter == 'o'))
                    {
                        if (currModeLetter == 'k')
                        {
                            modeArg = getNextParam().first;
                            currChannel->setKey(modeArg);
                        }
                        else if (currModeLetter == 'l')
                        {
                            modeArg = getNextParam().first;
                            size_t limit = static_cast<size_t>(atoi(modeArg.c_str()));
                            currChannel->setLimit(limit > 0 ? limit : 1);
                        }
                        else if (currModeLetter == 'o' && _paramCounter == 3)
                        {
                            _logger.ServertoClient(ERR_SPECIFYPARAM(currUser->getNickName(), channelName));
                            continue;
                        }

                        else if (currModeLetter == 'o' && _paramCounter == 4)
                        {
                            modeArg = getNextParam().first;
                            User *Operator = db->existUser(modeArg);
                            if (!Operator)
                            {
                                _logger.ServertoClient(ERR_NOUSERS(currUser->getNickName(), channelName));
                                continue;
                            }
                            else if (currChannel->isNickExist(modeArg) == false)
                            {
                                _logger.ServertoClient(ERR_USERNOTINCHANNEL(currUser->getNickName(), modeArg, channelName));
                                continue;
                            }
                            else
                            {
                                currChannel->addOperator(Operator->getUserId());
                                SendMessageToMembers(currChannel, currUser, "MODE " + channelName + " +o " + modeArg);
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
                            modeArg = getNextParam().first;
                            User *Operator = db->existUser(modeArg);
                            if (!Operator)
                            {
                                _logger.ServertoClient(ERR_NOUSERS(currUser->getNickName(), channelName));
                                continue;
                            }
                            else if (currChannel->isNickExist(modeArg) == false)
                            {
                                _logger.ServertoClient(ERR_USERNOTINCHANNEL(currUser->getNickName(), modeArg, channelName));
                                continue;
                            }
                            else
                            {
                                if (currChannel->isUserOperator(Operator->getUserId()) == true && currChannel->isUserOperator(currUser->getUserId()) == true)
                                {
                                    currChannel->deleteOperator(Operator);
                                    SendMessageToMembers(currChannel, currUser, "MODE " + channelName + " -o " + modeArg);
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
