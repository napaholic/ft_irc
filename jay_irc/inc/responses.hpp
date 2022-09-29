#ifndef RESPONSES_HPP
# define RESPONSES_HPP

#define RPL_LIST                "322" // "<channel> <# visible> :<topic>"
#define RPL_LISTEND             "323" // ":End of LIST"
#define RPL_CHANNELMODEIS       "324" // "<channel> <mode> <mode params>"
#define RPL_UNIQOPIS            "325" // "<channel> <nickname>"

#define RPL_NOTOPIC             "331" // "<channel> :No topic is set"
#define RPL_TOPIC               "332" // "<channel> :<topic>"
#define RPL_INVITELIST          "346" // "<channel> <invitemask>"
#define RPL_ENDOFINVITELIST     "347" // "<channel> :End of channel invite list"
#define RPL_EXCEPTLIST          "348" // "<channel> <exceptionmask>"
#define RPL_ENDOFEXCEPTLIST     "349" // "<channel> :End of channel exception list"
#define RPL_NAMREPLY            "353" //"( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )"

#define RPL_ENDOFNAMES          "366" // "<channel> :End of NAMES list"
#define RPL_BANLIST             "367" // "<channel> <banmask>"
#define RPL_ENDOFBANLIST        "368" // "<channel> :End of channel ban list"

#define ERR_NOSUCHNICK          "401" // "<nickname> :No such nick/channel"
#define ERR_NOSUCHSERVER        "402" // "<server name> :No such server"
#define ERR_NOSUCHCHANNEL       "403" // "<channel name> :No such channel"
#define ERR_TOOMANYCHANNELS     "405" // "<channel name> :You have joined too many channels"
#define ERR_TOOMANYTARGETS      "407" // "<target> :<error code> recipients. <abort message>"
#define ERR_NONICKNAMEGIVEN     "431" // ":No nickname given"
#define ERR_ERRONEUSNICKNAME    "432" // "<nick> :Erroneous nickname"
#define ERR_NICKNAMEINUSE       "433" // "<nick> :Nickname is already in use"
#define ERR_NICKCOLLISION       "436" // "<nick> :Nickname collision KILL from <user>@<host>"
#define ERR_UNAVAILRESOURCE     "437" // "<nick/channel> :Nick/channel is temporarily unavailable"
#define ERR_USERNOTINCHANNEL    "441" // "<nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL        "442" // "<channel> :You're not on that channel"
#define ERR_RESTRICTED          "484" // ":Your connection is restricted!"
#define ERR_NEEDMOREPARAMS      "461" // "<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED    "462" // ":Unauthorized command (already registered)"
#define ERR_KEYSET              "467" // "<channel> :Channel key already set"
#define ERR_CHANNELISFULL       "471" // "<channel> :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE         "472" // "<char> :is unknown mode char to me for <channel>"
#define ERR_INVITEONLYCHAN      "473" // "<channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN      "474" // "<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY       "475" // "<channel> :Cannot join channel (+k)"
#define ERR_BADCHANMASK         "476" // "<channel> :Bad Channel Mask"
#define ERR_NOCHANMODES         "477" // "<channel> :Channel doesn't support modes"
#define ERR_CHANOPRIVSNEEDED    "482" // "<channel> :You're not channel operator"
#define ERR_UMODEUNKNOWNFLAG    "501" // ":Unknown MODE flag"
#define ERR_USERSDONTMATCH      "502" // ":Cannot change mode for other users"
#endif