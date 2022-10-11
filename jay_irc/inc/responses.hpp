#ifndef RESPONSES_HPP
# define RESPONSES_HPP

#include <string.h>

//#define RPL_WELCOME(nick) ("Welcome to the Internet Relay Network" + nick + "\r\n")                                 // 001
#define RPL_WELCOME(nick) std::string(nick).append(" Welcome to the Internet Relay Network\r\n")                                // 001
#define RPL_UMODEIS(user_mode) (user_mode + "\r\n")                                                                 // 221
#define RPL_UNIQOPIS(channel, nick) (channel + " " + nick + "\r\n")                                                 // 325
#define RPL_NOTOPIC(channel) std::string(channel).append(" :No topic is set\r\n")                                                    // 331
#define RPL_TOPIC(channel, topic) (channel + " :" + topic + "\r\n")                                                 // 332
#define RPL_INVITING(channel, nick) (channel + " " + nick + "\r\n")                                                 // 341
#define RPL_YOUREOPER "You are now an IRC operator\r\n"                                                             // 381

#define ERR_NOSUCHNICK(nick) std::string(nick).append(" :No such nick\r\n")                                                          // 401
#define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n")                                              // 403
#define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel\r\n")                                    // 404
#define ERR_NORECIPIENT(command) ("No recipient given " + command + "\r\n")                                         // 411
#define ERR_NOTEXTTOSEND "No text to send\r\n"                                                                      // 412
#define ERR_NONICKNAMEGIVEN "No nickname given\r\n"                                                                 // 431
#define ERR_ERRONEUSNICKNAME(nick) std::string(nick).append(" :Erroneous nickname\r\n")                                              // 432
#define ERR_NICKNAMEINUSE(nick) std::string(nick).append(" :Nickname is already in use\r\n")                                         // 433
#define ERR_NICKCOLLISION(nick, user, host) (nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n") // 436
#define ERR_UNAVAILRESOURCE(resource) (resource + " :Nick/channel is temporarily unavailable\r\n")                  // 437
#define ERR_USERNOTINCHANNEL(nick, channel) (nick + " " + channel + " :They aren't on that channel\r\n")            // 441
#define ERR_NOTONCHANNEL(channel) std::string(channel).append(" :You're not on that channel\r\n")                                    // 442
#define ERR_USERONCHANNEL(user, channel) (user + " " + channel + " :is already on channel\r\n")                     // 443
//#define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters\r\n")                                       // 461
#define ERR_NEEDMOREPARAMS(command) std::string(command).append(" :Not enough parameters\r\n")                                       // 461
#define ERR_ALREADYREGISTRED "Unauthorized command (already registered)\r\n"                                        // 462
#define ERR_PASSWDMISMATCH "Password incorrect\r\n"                                                                 // 464
#define ERR_BADCHANMASK(channel) std::string(channel).append(" :Bad Channel Mask\r\n")                                               // 476
#define ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator\r\n")                               // 482
#define ERR_RESTRICTED "Your connection is restricted!\r\n"                                                         // 484
#define ERR_UMODEUNKNOWNFLAG "Unknown MODE flag\r\n"                                                                // 501 
#define ERR_USERSDONTMATCH "Cannot change mode for other users\r\n"                                                 // 502
#endif