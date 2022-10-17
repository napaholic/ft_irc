//
// Created by jaewkim on 2022/09/26.
//
#include "../inc/channel.hpp"

Channel::Channel(const std::string &name, Client *oper)
    : __name(name), __topic(""), __hostClient(*oper)
{
    addClient(oper);
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


void Channel::setMode(unsigned char mode)
{
    __mode |= mode;
}


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
