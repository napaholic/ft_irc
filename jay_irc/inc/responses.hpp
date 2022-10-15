#ifndef RESPONSES_HPP
# define RESPONSES_HPP

#include <string.h>

//#define RPL_WELCOME(nick) ("Welcome to the Internet Relay Network" + nick)                                 // 001
#define RPL_WELCOME(nick) std::string(nick).append(" Welcome to the Internet Relay Network")                                // 001
#define RPL_UMODEIS(user_mode) (user_mode)                                                                 // 221
#define RPL_UNIQOPIS(channel, nick) (channel + " " + nick)                                                 // 325
#define RPL_NOTOPIC(channel) std::string(channel).append(" :No topic is set")                                                    // 331
#define RPL_TOPIC(channel, topic) (channel + " :" + topic)                                                 // 332
#define RPL_INVITING(channel, nick) (channel + " " + nick)                                                 // 341
#define RPL_YOUREOPER "You are now an IRC operator"                                                             // 381

//#define ERR_NOSUCHNICK(nick) std::string(nick).append(" :No such nick")                                                          // 401
#define ERR_NOSUCHNICK(nick) (nick + " :No such nick")                                                          // 401
#define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel")                                              // 403
#define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel")                                    // 404
#define ERR_NORECIPIENT(command) std::string("No recipient given ").append(command)                                     // 411
#define ERR_NOTEXTTOSEND "No text to send"                                                                      // 412
#define ERR_NONICKNAMEGIVEN "No nickname given"                                                                 // 431
#define ERR_ERRONEUSNICKNAME(nick) std::string(nick).append(" :Erroneous nickname")                                              // 432
#define ERR_NICKNAMEINUSE(nick) std::string(nick).append(" :Nickname is already in use")                                         // 433
#define ERR_NICKCOLLISION(nick, user, host) (nick + " :Nickname collision KILL from " + user + "@" + host) // 436
#define ERR_UNAVAILRESOURCE(resource) (resource + " :Nick/channel is temporarily unavailable")                  // 437
#define ERR_USERNOTINCHANNEL(nick, channel) (nick + " " + channel + " :They aren't on that channel")            // 441
#define ERR_NOTONCHANNEL(channel) std::string(channel).append(" :You're not on that channel")                                    // 442
#define ERR_USERONCHANNEL(user, channel) (user + " " + channel + " :is already on channel")                     // 443
#define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters")                                       // 461
//#define ERR_NEEDMOREPARAMS(command) std::string(command).append(" :Not enough parameters")                                       // 461
#define ERR_ALREADYREGISTRED "Unauthorized command (already registered)"                                        // 462
#define ERR_PASSWDMISMATCH "Password incorrect"                                                                 // 464
#define ERR_BADCHANMASK(channel) std::string(channel).append(" :Bad Channel Mask")                                               // 476
#define ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator")                               // 482
#define ERR_RESTRICTED "Your connection is restricted!"                                                         // 484
#define ERR_UMODEUNKNOWNFLAG "Unknown MODE flag"                                                                // 501
#define ERR_USERSDONTMATCH "Cannot change mode for other users"
#define ERR_UNKNOWNMODE(unknownMode) (unknownMode + " :is unknown mode char to me")
// 502
#endif