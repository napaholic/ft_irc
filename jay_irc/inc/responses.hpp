#ifndef RESPONSES_HPP
#define RESPONSES_HPP

#include <string.h>
#define RPL_WELCOME(nick) (":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network")
#define RPL_NOTOPIC(user, channel) (":ft_irc 331 " + user + " " + channel + " :No topic is set")
#define RPL_TOPIC(user, channel, topic) (":ft_irc 332 " + user + " " + channel + " :" + topic)
#define RPL_INVITING(user, channel, nick) (":ft_irc 341 " + user + " " + channel + " " + nick)
#define RPL_LISTSTART ":ft_irc 321 Channel :Users  Name"
#define RPL_LIST(user, channel, members, topic) (":ft_irc 322 " + user + " " + channel + " " + members + " :" + topic)
#define RPL_LISTEND(user) (":ft_irc 323 " + user + "  :End of LIST")

#define ERR_NOSUCHNICK(user, nick) (":ft_irc 401 " + user + " " + nick + " :No such nick")
#define ERR_NOSUCHCHANNEL(user, channel) (":ft_irc 403 " + user + " " + channel + " :No such channel/channel")
//#define ERR_NORECIPIENT(user, command) std::string(":ft_irc 401 " + user).append(std::string(" No recipient given
//").append(command))
#define ERR_NORECIPIENT(user, command)                                                                                 \
    (std::string(":ft_irc 401 ").append(user)).append(std::string(" No recipient given ").append(command))

#define ERR_NOTEXTTOSEND(user) (":ft_irc 412 " + user + " :No text to send")

#define ERR_NONICKNAMEGIVEN(user) (":ft_irc 431 " + user + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(user, nick) (":ft_irc 432 " + nick + " :Erroneous nickname")

#define ERR_NICKNAMEINUSE(user, nick) (":ft_irc 433 " + nick + " :Nickname is already in use")

#define ERR_NOTONCHANNEL(user, channel) (":ft_irc 442 " + user + " " + channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(user, channel) (":ft_irc 443 " + user + " " + channel + " :is already on channel")
#define ERR_NEEDMOREPARAMS(user, command)                                                                              \
    std::string(":ft_irc 461 " + user + " ").append(std::string(command).append(" :Not enough parameters"))
#define ERR_ALREADYREGISTRED(user) ":ft_irc 462 " + user + " Unauthorized command (already registered)"
#define ERR_BADCHANMASK(user, channel) (":ft_irc 476 " + user + " " + channel + " :Bad Channel Mask")
#define ERR_CHANOPRIVSNEEDED(user, channel) (":ft_irc 482 " + user + " " + channel + " :You're not channel operator")
#define ERR_UNKNOWNMODE(user, unknownMode) (":ft_irc 472 " + user + " " + unknownMode + " :is unknown mode char to me")

#define RPL_NAMREPLYTMP(channel, nicks) (channel + " :" + nicks)
#define RPL_NAMREPLY(user, lineByLineNamReply) (":ft_irc 353 " + user + " " + lineByLineNamReply)
#define RPL_ENDOFNAMES(user, channel) (":ft_irc 366 " + user + " " + channel + " :End of /NAMES list")

#endif