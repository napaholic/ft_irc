//
// Created by jaewkim on 2022/09/26.
//
#include "../inc/channel.hpp"

Channel::Channel(const std::string &name, Client *oper)
    : __name(name), __topic(""), __mode(0), __key(""), __hostClient(*oper)
{
    addClient(oper);
    // setPermissions(oper, opt_o);
}

Channel::~Channel()
{
}

void Channel::addClient(Client *client)
{
    if (isClientInChannel(*client) == false)
        __active_clients.insert(__active_clients.begin(), client);
    else
        std::cout << "Could not add client: Client is already in channel.\n";
}

void Channel::eraseClient(Client *client)
{
    __active_clients.erase(client);
}

bool Channel::isClientInChannel(const Client &client)
{
    for (std::set<Client *>::iterator it = __active_clients.begin(); it != __active_clients.end(); ++it)
    {
        if (**it == client)
            return true;
    }
    return false;
}

// bool	Channel::isBanned(const std::string &nick)
//{
//     for(std::vector<std::string>::iterator it = __banned.begin();
//          it != __banned.end(); ++it)
//     {
//         if (*it == nick)
//             return (1);
//     }
//     return (0);
// }
//

// unsigned char Channel::get_permissions(const std::string &nick)
//{
//     if (!isClient(nick))
//         return (opt_err);
//     for (std::list<std::string>::iterator it = __operator_list.begin();
//          it != __operator_list.end(); ++it)
//     {
//         if (*it == nick)
//             return (opt_o);
//     }
//     return (opt_err);
// }

// void	Channel::addBanned(const std::string &nick)
//{
//     if (!isBanned(nick))
//     {
//         if (isClient(nick))
//             eraseClient(nick);
//         __banned.push_back(nick);
//     }
// }
//
// void	Channel::eraseBanned(const std::string &nick)
//{
//     if (!isBanned(nick))
//         return;
//     for(std::vector<std::string>::iterator it = __banned.begin();
//          it != __banned.end(); ++it)
//     {
//         if (*it == nick)
//         {
//             it = __banned.erase(it);
//             return ;
//         }
//     }
// }

// void	Channel::setPermissions(Client *client, unsigned char perm)
//{
//	std::set<Client *>::iterator it = __active_clients.find(client);
//	if (it == __active_clients.end())
//		return;
//	__operator_list.insert(__operator_list.end(), client->getNickname());
//	setMode(perm);
// }

void Channel::setMode(unsigned char mode)
{
    __mode |= mode;
}

// void Channel::printChannel()
//{
//     for (std::set<Client *>::iterator it = __active_clients.begin(); it != __active_clients.end(); ++it)
//     {
//         std::cout << "-:" << (*it)->getNickname() << "\t\n";
//     }
//     std::cout << "-:"
//               << "channel operator list" << std::endl;
//     for (std::list<std::string>::iterator it = __operator_list.begin(); it != __operator_list.end(); ++it)
//     {
//         std::cout << (*it) << " is operator"
//                   << "\t\n";
//     }
// }

void Channel::setKey(const std::string &key)
{
    this->__key = key;
    setMode(opt_k);
}

// std::string Channel::sendUserList(std::string serverip, std::string nick)
//{
//     std::string a[] = {
//         ":", serverip, " ", RPL_NAMREPLY, " ", nick, " @ ",
//         this->__name, " :", "NULL"};
//     std::string t = buildString(a);
//     for (std::map<std::string, unsigned char>::iterator it = __active_clients.begin();
//          it != __active_clients.end(); ++it)
//     {
//         std::cout << "NICK" << (*it).first << std::endl;
//         if ((*it).second.at(0) == '+')//뒤에 +가 아니라 무슨옵션일때 '@'문자 붙이는지 알아야됨.
//             t.append("@");//우리 코드는 string 이 아니라 비트옵션이기때문에 추후 수정 필요.
//         t.append((*it).first);
//         t.append(" ");
//     }
//     return (t);
// }

Client *Channel::findClient(std::string nick)
{
    for (std::set<Client *>::iterator it = __active_clients.begin(); it != __active_clients.end(); ++it)
    {
        if ((*it)->getNickname() == nick)
            return (*it);
    }
    return NULL;
}

Client *Channel::findClient(Client *client)
{
    for (std::set<Client *>::iterator it = __active_clients.begin(); it != __active_clients.end(); ++it)
    {
        if (*it == client)
            return (*it);
    }
    return NULL;
}

bool Channel::findOperator(const Client &client) const
{
    for (std::set<Client *>::const_iterator it = __operator_list.begin(); it != __operator_list.end(); ++it)
    {
        if (**it == client)
            return true;
    }
    return false;
}

std::string Channel::listingActiveClient(Client &client) const
{
    std::string str = "";
    std::set<Client *>::iterator it = __active_clients.begin();
    while (it != __active_clients.end())
    {
        if ((**it) == client)
        {
            str = str + " @" + (*it)->getNickname();
        }
        else
        {
            str = str + " " + (*it)->getNickname();
        }
        ++it;
    }
    return RPL_NAMREPLYTMP(this->getName(), str);
}

// Need to fix add_del operator has Client * parameter.

void Channel::addOperator(Client *client)
{
    if (findOperator(*client) == false)
        __operator_list.insert(client);
}

void Channel::delOperator(Client *client)
{
    if (findOperator(*client) == true)
        __operator_list.erase(client);
}

const std::string Channel::getTopic()
{
    return __topic;
}
void Channel::setTopic(std::string topic)
{
    __topic = topic;
}

const std::string &Channel::getName() const
{
    return __name;
}

const std::set<Client *> &Channel::getActiveClients() const
{
    return __active_clients;
}
