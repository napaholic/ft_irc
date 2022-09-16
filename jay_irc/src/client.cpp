//
// Created by Yunhui Bae on 9/16/22.
//

#include "../inc/client.h"


Client::Client(int socket)
    :__socket(socket)
{
    memset(buf, 0, sizeof(buf));
    std::cout << "Unknown client" << socket << "created.\n";
}

Client::~Client()
{
    close(socket);
}

Client::Client(const Client &rhs)
    :__socket(rhs.socket)
{
    memset(buf, 0, sizeof(buf));
}

int Client::getSocket() const
{
    return __socket;
}