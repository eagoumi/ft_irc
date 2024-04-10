#include "Commands.hpp"
#include <vector>

#define NOT_FOUND NULL

std::string GetLogsUsersChannel(Channel *ch)
{
    std::string MemberStr;
    std::map<USER_ID, User *> Members = ch->getMembers();
    for (std::map<USER_ID, User *>::iterator It_Members = Members.begin(); It_Members != Members.end(); ++It_Members)
    {
        if (ch->isUserOperator(It_Members->first))
            MemberStr += "@";
        MemberStr += It_Members->second->getNickName() + " ";
    }
    return MemberStr;
}

void Commands::PrintLogsAfterJoined(std::string ClientChannel, User& Client, Channel& Channel)
{
    if (!Channel.getTopic().empty())
        _logger.ServertoClient(RPL_TOPIC(Client.getNickName(), Channel.getChannelName(), Channel.getTopic()));
    _logger.ServertoClient(RPL_NAMREPLY(db->getUser(fd)->getNickName(), Channel.getChannelName(), ClientChannel));

    _logger.ServertoClient(RPL_ENDOFNAMES(db->getUser(fd)->getNickName(), Channel.getChannelName()));
}

void Commands::join()
{
    std::string currUserNickname = currUser->getNickName();
    std::vector<std::string> channelNamesList = getNextParam().second;
    std::vector<std::string> channelkeysList = getNextParam().second;

    for (size_t channelIndex = 0; channelIndex < channelNamesList.size(); channelIndex++)
    {
        currChannel = db->getChannel(channelNamesList[channelIndex]);
        if (currChannel == NOT_FOUND)
        {
            db->addNewChannel(channelNamesList[channelIndex], currUser);
            currChannel = db->getChannel(channelNamesList[channelIndex]);
            if (!channelkeysList.empty())
            {
                currChannel->setKey(channelkeysList[channelIndex]);
                currChannel->setMode('k');
            }
            SendMessageToMembers(currChannel, db->getUser(fd), "JOIN :" + currChannel->getChannelName());
            _logger.IRCPrint(":" + _logger.getServerIP() + " MODE " + channelNamesList[channelIndex] + " +t");
            PrintLogsAfterJoined(GetLogsUsersChannel(currChannel), *db->getUser(fd), *currChannel);
        }
        else
        {
            if (currChannel->getMember(fd) != NULL)
                _logger.ServertoClient(ERR_USERONCHANNEL(currUserNickname, channelNamesList[channelIndex])); 
            else if (currChannel->getMode('i') == true && currChannel->isUserInvited(currUser->getUserId()) == false)
                _logger.ServertoClient(ERR_INVITEONLYCHAN(currUserNickname, channelNamesList[channelIndex]));
            else if (currChannel->getMode('l') == true && currChannel->getLimit() <= currChannel->getMembers().size())
                _logger.ServertoClient(ERR_CHANNELISFULL(currUserNickname, channelNamesList[channelIndex]));
            else
            {
                if (currChannel->getMode('k') == true)
                {
                    if(channelkeysList.empty() || channelIndex > channelkeysList.size() - 1 || currChannel->isKeyMatch(channelkeysList[channelIndex]) == false)
                    {
                        _logger.ServertoClient(ERR_BADCHANNELKEY(currUserNickname, channelNamesList[channelIndex]));
                        continue;
                    }
                }
                currChannel->addMember(currUser);
                SendMessageToMembers(currChannel, db->getUser(fd), "JOIN :" + currChannel->getChannelName());
                PrintLogsAfterJoined(GetLogsUsersChannel(currChannel), *db->getUser(fd), *currChannel);
            }
        }
    }
}
