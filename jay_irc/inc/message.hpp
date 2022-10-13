#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "main.h"

class Client;

class Message
{
  private:
    // Memeber variables
    std::string __prefix;
    std::vector<std::string> __parameters;
    unsigned long __command;

  public:
    // Memebe functions
    Message(char *buf);
    ~Message();

    // Element access
	const std::vector<std::string> &getParameters() const;
	const unsigned long &getCommand() const;
};

#endif
