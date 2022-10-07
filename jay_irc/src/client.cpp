//
// Created by Yunhui Bae on 9/16/22.
//

#include "../inc/client.hpp"


Client::Client(int socket, struct sockaddr_in client_addr)
    :__socket(socket), __client_addr(client_addr)
{
    memset(buf, 0, sizeof(buf));
    __nickname = "";
    __servername = "ft_irc@42seoul.com";
    __allowed = 0;
    std::cout << "Unknown client" << socket << "created.\n";
}

Client::~Client()
{
    close(__socket);
}

Client::Client(const Client &rhs)
    :__socket(rhs.__socket)
{
    memset(buf, 0, sizeof(buf));
}

int Client::setClient()
{
    if (__nickname.length() && __username.length())
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

int Client::getSocket() const
{
    return __socket;
}