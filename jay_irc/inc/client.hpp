//
// Created by Yunhui Bae on 9/16/22.
//

#ifndef FT_IRC_CLIENT_H
#define FT_IRC_CLIENT_H

#include "./main.h"

class Client {
	friend class Server;
public:
    Client(int socket, sockaddr_in client_addr);
    ~Client();
    Client(const Client &rhs);
    int getSocket() const;
    int setClient();
	std::string getChName();
	void setChName(std::string ch_name);
    void make_prefix();
	std::string getNickname();

private:
    int					__socket;
    struct sockaddr_in	__client_addr;
	std::string			__ch_name;
    std::string			__nickname;
    std::string			__username;
    std::string			__hostname;
    std::string			__servername;
    std::string			__realname;

    std::string			__prefix;
    
    int					__allowed;

    char				buf[512];

    std::string			__operators;
};
#endif //FT_IRC_CLIENT_H
