#ifndef RESPONSES_HPP
#define RESPONSES_HPP

#include <string.h>
#define RPL_WELCOME(nick) (":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network")
#define RPL_NOTOPIC(channel) (":ft_irc 331 " + channel + " :No topic is set")
#define RPL_TOPIC(channel, topic) (":ft_irc 332 " + channel + " :" + topic)
#define RPL_INVITING(channel, nick) (":ft_irc 341 " + channel + " " + nick)
#define RPL_LISTSTART ":ft_irc 321 Channel :Users  Name"
#define RPL_LIST(nick, channel, members, topic) (":ft_irc 322 " + nick + " " + channel + " " + members + " :" + topic)
#define RPL_LISTEND ":ft_irc 323 :End of LIST"

#define ERR_NOSUCHNICK(nick) (":ft_irc 401 " + nick + " :No such nick")
#define ERR_NOSUCHCHANNEL(channel) (":ft_irc 403 " + channel + " :No such channel/channel")
#define ERR_NORECIPIENT(command) std::string(":ft_irc 401 ").append(std::string("No recipient given ").append(command))
#define ERR_NOTEXTTOSEND ":ft_irc 412 :No text to send"
#define ERR_NONICKNAMEGIVEN ":ft_irc 431 :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick) (":ft_irc 432 " + nick + " :Erroneous nickname")
#define ERR_NICKNAMEINUSE(nick) (":ft_irc 433 " + nick + " :Nickname is already in use")
#define ERR_NOTONCHANNEL(channel) (":ft_irc 442 " + channel + " :You're not on that channel")
#define ERR_USERONCHANNEL(user, channel) (":ft_irc 443 " + user + " " + channel + " :is already on channel")
#define ERR_NEEDMOREPARAMS(command) std::string(":ft_irc 461 ").append(std::string(command).append(" :Not enough parameters"))
#define ERR_ALREADYREGISTRED ":ft_irc 462 Unauthorized command (already registered)"
#define ERR_BADCHANMASK(channel) (":ft_irc 476 " + channel + " :Bad Channel Mask")
#define ERR_CHANOPRIVSNEEDED(channel) (":ft_irc 482 " + channel + " :You're not channel operator")
#define ERR_UNKNOWNMODE(unknownMode) (":ft_irc 472 " + unknownMode + " :is unknown mode char to me")
#define RPL_NAMREPLYTMP(channel, nicks) (channel + " :" + nicks)
#define RPL_NAMREPLY(lineByLineNamReply) (":ft_irc 353" + lineByLineNamReply)								//353
#define RPL_ENDOFNAMES(channel) (":ft_irc 366 " + channel + " :End of /NAMES list")

#endif