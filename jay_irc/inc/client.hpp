//
// Created by Yunhui Bae on 9/16/22.
//

#ifndef FT_IRC_CLIENT_H
#define FT_IRC_CLIENT_H

#include "./main.h"

class Message;

class Client
{
  private:
    // Member variables
    int __socket;
    struct sockaddr_in __client_addr;

    std::string __ch_name;
    std::string __nickname;
    std::string __username;
    std::string __hostname;
    std::string __servername;
    std::string __realname;

    std::string __prefix;

    Message *__message;

    int __allowed;

    char buf[512];

    std::string __operators;

  public:
    // Member functions
    Client(int socket, sockaddr_in client_addr);
    ~Client();
    Client(const Client &rhs);

    // Element access
    int getSocket() const;
    std::string getChannelName() const;
    const std::string &getNickname() const;
    int getAllowed() const;
    Message *getMessage() const;
    const std::string &getPrefix() const;

    void setChannelName(std::string ch_name);
    void setNickname(const std::string &name);
    void setMessage(char *buffer);
    void setAllowed(int n);
    void setUsername(const std::string &name);
	void setHostname(const std::string &name);
	void setRealname(const std::string &name);

    // others (unknown)
    void make_prefix();
    int allowClient();
};
#endif // FT_IRC_CLIENT_H
