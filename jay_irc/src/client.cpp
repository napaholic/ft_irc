//
// Created by Yunhui Bae on 9/16/22.
//

#include "../inc/client.h"


Client::Client(int socket, struct sockaddr_in client_addr)
    :__socket(socket), __client_addr(client_addr)
{
    memset(buf, 0, sizeof(buf));
    std::cout << "Unknown client" << socket << "created.\n";
}

Client::~Client()
{
    close(socket);
}

Client::Client(const Client &rhs)
    :__socket(rhs.__socket)
{
    memset(buf, 0, sizeof(buf));
}

void Client::setClient()
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