//
// Created by Yunhui Bae on 9/16/22.
//

#include "../inc/client.hpp"

Client::Client(int socket, struct sockaddr_in client_addr)
    : __socket(socket), __client_addr(client_addr), __message(NULL)
{
    memset(buf, 0, sizeof(buf));
    __nickname = "";
    __servername = "ft_irc@42seoul.com";
    __allowed = 0;
    std::cout << "Unknown client " << socket << " created.\n";
}

Client::~Client()
{
    close(__socket);
}

Client::Client(const Client &rhs) : __socket(rhs.__socket)
{
    memset(buf, 0, sizeof(buf));
}

int Client::allowClient()
{
    if (__nickname.length() && __username.length() && __allowed == 1)
    {
        make_prefix();
        __allowed = 2;
        return 1;
    }
    return 0;
}

void Client::make_prefix()
{
    __prefix.clear();
    __prefix = std::string(":").append(__nickname);
    __prefix.append("!");
    __prefix.append(__username);
    __prefix.append("@");
    std::string ip = std::to_string(__client_addr.sin_port);
    __prefix.append(ip);
}

std::string Client::getChannelName() const
{
    return __ch_name;
}

void Client::setChannelName(std::string ch_name)
{
    __ch_name = ch_name;
}

int Client::getSocket() const
{
    return __socket;
}

const std::string &Client::getNickname() const
{
    return __nickname;
}

void Client::setNickname(const std::string &name)
{
    __nickname = name;
}

int Client::getAllowed() const
{
    return __allowed;
}

void Client::setMessage(char *buffer)
{
    if (__message != NULL)
        delete __message;
    __message = new Message(buffer);
}

Message *Client::getMessage() const
{
    return __message;
}

const std::string &Client::getPrefix() const
{
    return __prefix;
}

void Client::setAllowed(int n)
{
    __allowed = n;
}

void Client::setUsername(const std::string &name)
{
    __username = name;
}

void Client::setHostname(const std::string &name)
{
    __hostname = name;
}

void Client::setRealname(const std::string &name)
{
    __realname = name;
}
