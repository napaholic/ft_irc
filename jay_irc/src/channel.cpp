//
// Created by jaewkim on 2022/09/26.
//
#include "../inc/channel.hpp"

Channel::Channel(const std::string &name, const std::string &nick)
    : __name(name), __topic(""), __mode(0), __key("")
{
    addClient(nick);
    setPermissions(nick, opt_o);
}

Channel::Channel(const std::string &name)
    : __name(name), __topic(""), __mode(0), __key("")
{}

Channel::~Channel() {}

void    Channel::addClient(const std::string &nick)
{
    if (!isClient(nick) && !isBanned(nick))
        __nicks.insert(std::pair<std::string, unsigned char>(nick, opt_o));
    else
        std::cout << "Could not add client\n";
}

bool	Channel::isClient(const std::string &nick)
{
    for(std::map<std::string, unsigned char>::iterator it = __nicks.begin();
         it != __nicks.end(); ++it)
    {
        if (it->first == nick)
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
    for(std::map<std::string, unsigned  char>::iterator it = __nicks.begin();
         it != __nicks.end(); ++it)
    {
        if (it->first == nick)
        {
            __nicks.erase(it);
            return ;
        }
    }
}

unsigned char Channel::get_permissions(const std::string &nick)
{
    if (!isClient(nick))
        return (opt_err);
    for (std::map<std::string, unsigned char>::iterator it = __nicks.begin();
         it != __nicks.end(); ++it)
    {
        if (it->first == nick)
            return (it->second);
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

void	Channel::setPermissions(const std::string &nick, unsigned char perm)
{
    //perm solo admite un cambio de modo (+x a -x, no -a+e+x+e)
    std::map<std::string, unsigned char>::iterator it = __nicks.find(nick);
    if (it == __nicks.end())
        return;
    it->second |= perm;
}

void	Channel::setMode(unsigned char mode)
{
    __mode |= mode;
}

void Channel::printChannel()
{
    for (std::map<std::string, unsigned char>::iterator it = __nicks.begin();
         it != __nicks.end(); ++it)
    {
        std::cout << "-:" << it->first << "\t" << it->second << "\t\n";
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
//    for (std::map<std::string, unsigned char>::iterator it = __nicks.begin();
//         it != __nicks.end(); ++it)
//    {
//        std::cout << "NICK" << (*it).first << std::endl;
//        if ((*it).second.at(0) == '+')//뒤에 +가 아니라 무슨옵션일때 '@'문자 붙이는지 알아야됨.
//            t.append("@");//우리 코드는 string 이 아니라 비트옵션이기때문에 추후 수정 필요.
//        t.append((*it).first);
//        t.append(" ");
//    }
//    return (t);
//}

void	Channel::change_nick(const std::string &old_nick, const std::string &nick)
{
    std::cout<< old_nick << nick << std::endl;
    if (isClient(old_nick))
    {
        __nicks.insert(std::pair<std::string, unsigned char>(nick, get_permissions(old_nick)));
        eraseClient(old_nick);
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