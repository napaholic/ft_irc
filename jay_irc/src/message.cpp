#include "../inc/message.hpp"

Message::Message(Client* client, char *buf): __client(client) {
    std::string s;
	std::stringstream ss((std::string(buf)));

	while (ss >> s)
		__parameters.push_back(s);
	
	if (__parameters[0][0] == ':')
	{
		__prefix = __parameters[0].substr(1, __parameters.size());
		__parameters.erase(__parameters.begin());
	}
    __command = djb2(*__parameters.begin());
    __parameters.erase(__parameters.begin());
}

Message::~Message() {}