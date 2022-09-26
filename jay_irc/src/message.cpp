#include "../inc/message.hpp"

Message::Message(int fd, char *buf): __fd(fd) {
    std::string s;
	std::stringstream ss((std::string(buf)));
	
	while (ss >> s)
		__parameters.push_back(s);
	
	if (__parameters[0][0] == ':')
	{
		__prefix = __parameters[0].substr(1, __parameters.size());
		__parameters.erase(__parameters.begin());
	}
    __command = *__parameters.begin();
    __parameters.erase(__parameters.begin());
}

Message::~Message() {}