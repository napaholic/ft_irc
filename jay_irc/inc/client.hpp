//
// Created by Yunhui Bae on 9/16/22.
//

#ifndef FT_IRC_CLIENT_H
#define FT_IRC_CLIENT_H

#include "./main.h"

#define ALLOW 1
#define DENY 0


class Client {
public:
    Client(int socket, sockaddr_in client_addr);
    ~Client();
    Client(const Client &rhs);
    int getSocket() const;
    void setClient();

private:
    int __socket;
    struct sockaddr_in __client_addr;
    std::string __nickname;
    std::string __username;
    std::string __hostname;
    std::string __servername;
    std::string __realname;

    std::string __prefix;
    
    int __allowed;

    char buf[512];

    std::string __operators;
};
#endif //FT_IRC_CLIENT_H
