//
// Created by jaewkim on 2022/09/26.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include "./main.h"

#define const unsigned char opt_o = 0 //0000 0001
#define const unsigned char opt_k = 2 //0000 0010


class Channel
{
    friend class Server;
public:
	Channel(const std::string &name, const std::string &nick);
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
	void	Channel::add_operater(std::string &str);
	void	Channel::del_operator(std::string &str);
	
private:
    std::string __name;
    std::string __topic;
    std::list __operator_list;
    std::map<std::string, unsigned char> __nicks;
    std::vector<std::string> __banned;
    unsigned char __mode;
    std::string __key;
    
    Channel();
    Channel	&operator=(const Channel &rhs);
};

#endif // FT_IRC_CHANNEL_HPP
