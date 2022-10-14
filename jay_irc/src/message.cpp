#include "../inc/message.hpp"

Message::Message(char *buf)
{
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

Message::~Message()
{
}

const std::vector<std::string> &Message::getParameters() const
{
    return __parameters;
}

const unsigned long &Message::getCommand() const
{
    return __command;
}

const std::string Message::combineParameters()
{
	std::string combinedParam;
	for (std::vector<std::string>::iterator it = __parameters.begin(); it != __parameters.end(); ++it)
	{
		combinedParam += (*it);
		if (it != --__parameters.end())
			combinedParam += " ";
	}
	return combinedParam;
}

int Message::getParamSize()
{
	return __parameters.size();
}