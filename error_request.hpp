#ifndef __ERROR_REQUEST_HPP__
#define __ERROR_REQUEST_HPP__

//Errors
#define ERR_NOSUCHNICK			401
#define ERR_TOOMANYPARAM(Client)			"4001 " + Client + ": Too Many Paramatre, Please try again!"
#define ERR_ALLREADYENTERPASS(Client)			"4002 " + Client + ": You have already entered the password!"
#define ERR_CORRECTPASS(Client)			"4003 " + Client + ": Your password is correct ✅"
#define ERR_MISSPASS(Client)			"4004 " + Client + ": Please enter the password to connect to the server first. 🚫"
#define ERR_NOSUCHSERVER		402
#define ERR_NOSUCHCHANNEL(NickName, Chanel)             "403 " + NickName + " " + Chanel + " :No such channel"
#define ERR_CANNOTSENDTOCHAN	404
#define ERR_TOOMANYCHANNELS		405
#define ERR_WASNOSUCHNICK		406
#define ERR_TOOMANYTARGETS		407
#define ERR_NOORIGIN			409
#define ERR_NORECIPIENT			411
#define ERR_NOTEXTTOSEND		412
#define ERR_NOTOPLEVEL			413
#define ERR_WILDTOPLEVEL		414
#define ERR_UNKNOWNCOMMAND(Client, Command)		"421 " + Client + Command + ": Unknown command!"
#define ERR_NOMOTD				422
#define ERR_NOADMININFO			423
#define ERR_FILEERROR			424
#define ERR_NONICKNAMEGIVEN(Client)		"431 " + Client + ": No nickname given"
#define ERR_ERRONEUSNICKNAME(NickName)	"432 " + nickname + ": Erroneus nickname!"
#define ERR_NICKNAMEINUSE		433
#define ERR_NICKCOLLISION		436
#define ERR_USERNOTINCHANNEL	441
#define ERR_NOTONCHANNEL(NickName, Chanl)              "442 " + NickName + " " + Chanl + " :You're not on that Channel"
#define ERR_USERONCHANNEL		443
#define ERR_NOLOGIN				444
#define ERR_SUMMONDISABLED		445
#define ERR_USERSDISABLED		446
#define ERR_NOTREGISTERED(Client)		"451 " + Client + ": You have not registered"
#define ERR_NEEDMOREPARAMS(NickName, Parameter)		"461 " + NickName + Parameter + ": Not enough parameters, Please try again!"
#define ERR_ALREADYREGISTRED	462
#define ERR_NOPERMFORHOST		463
#define ERR_PASSWDMISMATCH(Client)		"464 " + Client + ": Password Incorrect, Please try again!"
#define ERR_YOUREBANNEDCREEP	465
#define ERR_KEYSET				467
#define ERR_CHANNELISFULL		471
#define ERR_UNKNOWNMODE			472
#define ERR_INVITEONLYCHAN		473
#define ERR_BANNEDFROMCHAN		474
#define ERR_BADCHANNELKEY		475
#define ERR_NOPRIVILEGES		481
#define ERR_CHANOPRIVSNEEDED	482
#define ERR_CANTKILLSERVER		483
#define ERR_NOOPERHOST			491
#define ERR_UMODEUNKNOWNFLAG	501
#define ERR_USERSDONTMATCH		502

//Reply s
#define RPL_WELCOME(Nickname, Network, user, Host)  "001 " + Nickname + " :Welcome to the " + Network + " Network, " + Nickname + "!" + " [ " + user + "@" + Host + " ]"
#define RPL_YOURHOST(Nickname, Host, Version)		    "002 " + Nickname + " :Your host is " + Host + ", running version " + Version + "!"
#define RPL_NONE				300
#define RPL_USERHOST			302
#define RPL_ISON				303
#define RPL_AWAY				301
#define RPL_UNAWAY				305
#define RPL_NOWAWAY				306
#define RPL_WHOISUSER			311
#define RPL_WHOISSERVER			312
#define RPL_WHOISOPERATOR		313
#define RPL_WHOISIDLE			317
#define RPL_ENDOFWHOIS			318
#define RPL_WHOISCHANNELS		319
#define RPL_WHOWASUSER			314
#define RPL_ENDOFWHOWAS			369
#define RPL_LISTSTART			321
#define RPL_LIST				322
#define RPL_LISTEND				323
#define RPL_CHANNELMODEIS		324
#define RPL_NOTOPIC				331
#define RPL_TOPIC				332
#define RPL_INVITING			341
#define RPL_SUMMONING			342
#define RPL_VERSION				351
#define RPL_WHOREPLY			352
#define RPL_ENDOFWHO			315
#define RPL_NAMREPLY			353
#define RPL_ENDOFNAMES			366
#define RPL_LINKS				364
#define RPL_ENDOFLINKS			365
#define RPL_BANLIST				367
#define RPL_ENDOFBANLIST		368
#define RPL_INFO				371
#define RPL_ENDOFINFO			374
#define RPL_MOTDSTART(Nickname) 	"375 " + Nickname + " :- <server> Message of the day - "
#define RPL_MOTD(Nickname, MessageLine)				"372 " + Nickname + " : " + MessageLine
#define RPL_ENDOFMOTD(Nickname)			"376 " + Nickname + " :End of /MOTD command."
#define RPL_YOUREOPER			381
#define RPL_REHASHING			382
#define RPL_TIME				391
#define RPL_USERSSTART			392
#define RPL_USERS				393
#define RPL_ENDOFUSERS			394
#define RPL_NOUSERS				395
#define RPL_TRACELINK			200
#define RPL_TRACECONNECTING		201
#define RPL_TRACEHANDSHAKE		202
#define RPL_TRACEUNKNOWN		203
#define RPL_TRACEOPERATOR		204
#define RPL_TRACEUSER			205
#define RPL_TRACESERVER			206
#define RPL_TRACENEWTYPE		208
#define RPL_TRACELOG			261
#define RPL_STATSLINKINFO		211
#define RPL_STATSCOMMANDS		212
#define RPL_STATSCLINE			213
#define RPL_STATSNLINE			214
#define RPL_STATSILINE			215
#define RPL_STATSKLINE			216
#define RPL_STATSYLINE			218
#define RPL_ENDOFSTATS			219
#define RPL_UMODEIS(Nickname, Mode)				"221 " + Nickname + " " + Mode
#define RPL_STATSLLINE			241
#define RPL_STATSUPTIME			242
#define RPL_STATSOLINE			243
#define RPL_STATSHLINE			244
#define RPL_LUSERCLIENT			251
#define RPL_LUSEROP				252
#define RPL_LUSERUNKNOWN		253
#define RPL_LUSERCHANNELS		254
#define RPL_LUSERME				255
#define RPL_ADMINME				256
#define RPL_ADMINLOC1			257
#define RPL_ADMINLOC2			258
#define RPL_ADMINEMAIL			259


#endif