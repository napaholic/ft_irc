//
// Created by Yunhui Bae on 9/16/22.
//

#ifndef FT_IRC_CLIENT_H
#define FT_IRC_CLIENT_H

class Client {
public:
    Client(int socket);
    ~Client();
    Client(const Client &rhs);
    int getSocket() const;

private:
    int __socket;
    std::string __nickname;
    std::string __username;
    std::string __hostname;
    std::string __servername;
    std::string __realname;

    char buf[512];

    std::string __hopcount;
    std::string __operators;
};
#endif //FT_IRC_CLIENT_H
