//
// Created by Jaewook Kim on 10/5/22.
//

#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <utility>

#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

unsigned long djb2(std::string str);

class Server {
public:
	Server();
	~Server();
	void kick();
	void set_kick();
	std::map<unsigned long, void (Server::*)()> __cmd_list;
};


#endif //TEST_TEST_H
