//
// Created by jaewkim on 2022/09/23.
//

#ifndef JAY_IRC_SESSION_H
#define JAY_IRC_SESSION_H

#include "./main.h"

#define MAX_QUEUE 10

//session roll 서버생성 모든 fd 관리 (소켓생성, 바인딩, 리스닝, 셀렉트까지)

class Session {
	friend class Server;

public:
	Session(const std::string &port, const std::string &password);
	void	check_port(const std::string &port);
	void	create_socket();
	bool	select_socket();
	~Session();
private:
	const std::string	__port;
	const std::string	__password;
	
	fd_set	__all;
	fd_set  __reads;
	int     __fd;
	int     __fd_max;
	struct sockaddr_in  __addr;
};


#endif //JAY_IRC_SESSION_H
