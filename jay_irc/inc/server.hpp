//
// Created by jaewkim on 2022/09/23.
/*
1. 데이터 받고 보내고.
2. 채널클래스, 유저클래스 소유

fd 돌아가면서 체크 및 뉴 커넥션 유저 받는것 역시 서버가한다.

서버는 커맨드만 분리하고 미리 지정된 함수포인터로 나머지 버퍼를 넘긴다.
 */
//

#ifndef JAY_IRC_SERVER_H
#define JAY_IRC_SERVER_H

#include "./util.hpp"
#include "./main.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
#define SPECIAL "[]\\`_^{|}-"

class Session;
class Channel;
class Message;

class Server {
public:
	Server(const std::string &port, const std::string &password);
	void	run(Session &session);
	void 	accept_client(Session &session);
	void	receive_message(Session &session, int fd);
	void	disconnect_client(Session &session, int fd);
	void	send_message(int fd, const char buf[]);
    void	send_message(int fd, std::string str);
	void	broad_cast(Session &session, char *buf, int fd);
	Client	*getClient(std::string nick);
    Client	*getClient(int fd);
    Channel *getChannel(std::string channel);
	bool    err_nick(std::string nick);
    void	new_nick(Message &msg);
	void	re_nick(Message &msg);
    void	pass(Message &msg);
	void	nick(Message &msg);
	void	user(Message &msg);
	void	quit(Message &msg);
    void    join(Message &msg);
    void    topic(Message &msg);
    void    invite(Message &msg);
	~Server();
	
private:
	const std::string 	__port;
	const std::string	__password;
	int					__port_int;
	std::map<int, Channel> *__channels;
    std::map<unsigned long, void (Server::*)(Message &msg)> __cmd_list;
	std::vector<Client *> __clients;
};


#endif //JAY_IRC_SERVER_H
