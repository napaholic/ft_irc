//
// Created by jaewkim on 2022/09/26.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include "./main.h"

#define const unsigned char opt_o = 1<<0 //0000 0001
#define const unsigned char opt_k = 1<<1 //0000 0010


class Channel
{
    friend class Server;
  private:
    std::string __name;
    std::string __topic;
    std::map<std::string, unsigned char> __nicks;
    std::vector<std::string> __banned;
    unsigned char __mode;
    std::string __key;
    bool local;
    
    Channel();
    Channel	&operator=(const Channel &rhs);
  public:
    Channel(const std::string &name, std::string const &nick);
    Channel(const std::string &name);
    ~Channel();

    void	addClient(const std::string &nick);
    void	addBanned(const std::string &nick);
    void	eraseClient(const std::string &nick);
    void	eraseBanned(const std::string &nick);

    bool	isClient(const std::string &nick);
    bool	isBanned(const std::string &nick);

    const unsigned char get_permissions(const std::string &nick);

    void	setPermissions(const std::string &nick, const std::string &perm);

    void	printChannel();
    void	setMode(std::string const &mode);
    void	setKey(std::string const &key);
    std::string	sendUserList(std::string serverip, std::string nick);

    void	change_nick(const std::string &old_nick, const std::string &nick);

};

#endif // FT_IRC_CHANNEL_HPP
