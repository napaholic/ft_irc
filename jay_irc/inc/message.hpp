#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "main.h"

class Client;

class Message {
    
    friend class Server;
    
public:
    Message(int fd, char *buf);
    ~Message();

private:
    std::string __prefix;
    std::vector<std::string>    __parameters;
    unsigned long long __command;
    int __fd;
    Client *__client;
};

#endif