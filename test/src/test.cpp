//
// Created by Jaewook Kim on 10/5/22.
//

#include "../inc/test.h"

unsigned long djb2(std::string str)
{
	unsigned long hash  = 5381;
	unsigned int size   = str.length();
	unsigned int i      = 0;
	for (int i = 0; i < size; i++) {
		hash = ((hash << 5) + hash) + (str[i]);
	}
	return hash;
}

Server::Server() {
	__cmd_list.insert(std::make_pair<unsigned long, void (Server::*)()>(djb2("KICK"), &Server::kick));
}

void Server::set_kick() {
	CALL_MEMBER_FN(*this, &Server::kick)();
    CALL_MEMBER_FN(*this, __cmd_list[djb2("KICK")])();
	((*this).*(__cmd_list[djb2("KICK")]))();
}

Server::~Server(){

}

void Server::kick() {
	printf("process kick\n");
}

int main()
{
	Server a;
	a.set_kick();
	return (1);
}