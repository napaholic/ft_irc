//
// Created by jaewkim on 2022/09/26.
//
#include "../inc/channel.hpp"

Channel::Channel(const std::string &name, Client *oper, int ch_id)
    : __name(name), __topic(""), __mode(0), __key(""), __ch_id(ch_id)
{
    addClient(oper);
    setPermissions(oper, opt_o);
}

Channel::Channel(const std::string &name, int ch_id)
    : __name(name), __topic(""), __mode(0), __key(""), __ch_id(ch_id)
{}

Channel::~Channel() {}

void    Channel::addClient(Client *client)
{
    if (!isClient(client->getNickname()) && !isBanned(client->getNickname()))

		__active_clients.insert(__active_clients.begin(), client);
    else
        std::cout << "Could not add client\n";
}

bool	Channel::isClient(const std::string &nick)
{
    for(std::set<Client *>::iterator it = __active_clients.begin();
         it != __active_clients.end(); ++it)
    {
        if ((*it)->getNickname() == nick)
            return (1);
    }
    return (0);
}

bool	Channel::isBanned(const std::string &nick)
{
    for(std::vector<std::string>::iterator it = __banned.begin();
         it != __banned.end(); ++it)
    {
        if (*it == nick)
            return (1);
    }
    return (0);
}

void	Channel::eraseClient(const std::string &nick)
{
    if (!isClient(nick))
        return;
    for(std::set<Client *>::iterator it = __active_clients.begin();
         it != __active_clients.end(); ++it)
    {
        if ((*it)->getNickname() == nick)
        {
			__active_clients.erase(it);
            return ;
        }
    }
}

unsigned char Channel::get_permissions(const std::string &nick)
{
    if (!isClient(nick))
        return (opt_err);
    for (std::list<std::string>::iterator it = __operator_list.begin();
         it != __operator_list.end(); ++it)
    {
        if (*it == nick)
            return (opt_o);
    }
    return (opt_err);
}

void	Channel::addBanned(const std::string &nick)
{
    if (!isBanned(nick))
    {
        if (isClient(nick))
            eraseClient(nick);
        __banned.push_back(nick);
    }
}

void	Channel::eraseBanned(const std::string &nick)
{
    if (!isBanned(nick))
        return;
    for(std::vector<std::string>::iterator it = __banned.begin();
         it != __banned.end(); ++it)
    {
        if (*it == nick)
        {
            it = __banned.erase(it);
            return ;
        }
    }
}

Client *Channel::findClientbyNick(std::string &nick)
{
	std::set<Client *>::iterator it = __active_clients.begin();
	while(it != __active_clients.end())
	{
		if ((*it)->getNickname() == nick)
			return (*it);
	}
	return NULL;
}

void	Channel::setPermissions(Client *client, unsigned char perm)
{
	std::set<Client *>::iterator it = __active_clients.find(client);
	if (it == __active_clients.end())
		return;
	__operator_list.insert(__operator_list.end(), client->getNickname());
	setMode(perm);
}

void	Channel::setMode(unsigned char mode)
{
    __mode |= mode;
}

void Channel::printChannel()
{
    for (std::set<Client *>::iterator it = __active_clients.begin();
         it != __active_clients.end(); ++it)
    {
        std::cout << "-:" << (*it)->getNickname() << "\t\n";
    }
	std::cout << "-:" << "channel operator list" << std::endl;
	for (std::list<std::string>::iterator it = __operator_list.begin(); it != __operator_list.end(); ++it)
	{
		std::cout << (*it) << " is operator" << "\t\n";
	}
}

void Channel::setKey(const std::string &key)
{
    this->__key = key;
    setMode(opt_k);
}

//std::string Channel::sendUserList(std::string serverip, std::string nick)
//{
//    std::string a[] = {
//        ":", serverip, " ", RPL_NAMREPLY, " ", nick, " @ ",
//        this->__name, " :", "NULL"};
//    std::string t = buildString(a);
//    for (std::map<std::string, unsigned char>::iterator it = __active_clients.begin();
//         it != __active_clients.end(); ++it)
//    {
//        std::cout << "NICK" << (*it).first << std::endl;
//        if ((*it).second.at(0) == '+')//뒤에 +가 아니라 무슨옵션일때 '@'문자 붙이는지 알아야됨.
//            t.append("@");//우리 코드는 string 이 아니라 비트옵션이기때문에 추후 수정 필요.
//        t.append((*it).first);
//        t.append(" ");
//    }
//    return (t);
//}

Client    *Channel::getClient(std::string nick)
{
    std::set<Client *>::iterator it = __active_clients.begin();
    while (it != __active_clients.end())
    {
        if ((*it)->getNickname() == nick)
            return (*it);
        ++it;
    }
    return NULL;
}

void	Channel::change_nick(std::string &old_nick, std::string &nick)
{
    std::cout<< old_nick << nick << std::endl;
    if (isClient(old_nick))
    {
		//if (isClient(nick))
		//	return;
		//findClientbyNick(old_nick);
        getClient(old_nick)->__nickname = nick;
    }
    if (isBanned(old_nick))
    {
        for (std::vector<std::string>::iterator it = __banned.begin();
             it != __banned.end(); ++it)
        {
            if (*it == old_nick)
            {
                it = __banned.erase(it);
                __banned.push_back(nick);
            }
        }
    }
}

void	Channel::add_operater(std::string &str)
{
    if (std::find(__operator_list.begin(), __operator_list.end(), str) != __operator_list.end())
        __operator_list.insert(__operator_list.begin(), std::string(str));
}

void	Channel::del_operator(std::string &str)
{
    std::list<std::string>::iterator it;
    if ((it = std::find(__operator_list.begin(), __operator_list.end(), str)) != __operator_list.end())
        __operator_list.erase(it);
}