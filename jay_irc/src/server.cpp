//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/server.hpp"

Server::Server(const std::string &port, const std::string &password)
: __port(port), __password(password) {
	__port_int = std::atoi(port.c_str());
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("NICK"), &Server::NICK));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("QUIT"), &Server::QUIT));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("JOIN"), &Server::JOIN));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("USER"), &Server::USER));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("TOPIC"), &Server::TOPIC));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("INVITE"), &Server::INVITE));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("KICK"), &Server::KICK));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("MODE"), &Server::MODE));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("PRIVMSG"), &Server::PRIVMSG));
    __cmd_list.insert(std::pair<unsigned long, void(*)(Message &)>(djb2("NOTICE"), &Server::NOTICE));
    
    //hash 맵 key는 string 해쉬값, value는 함수포인터 주소. 인자는 패러미터 string
    // map<long long, class::method>
}

void Server::run(Session &session) {
	for (int i = 0; i < session.__fd_max + 1; i++)
	{
		if (FD_ISSET(i, &session.__reads) == 0) continue;
		if (i == session.__fd)
			accept_client(session);
		else
			receive_message(session, i);
	}
}

void Server::accept_client(Session &session) {
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	
	memset(&client_addr, 0, sizeof(client_addr));
	client_fd = accept(session.__fd, (struct sockaddr *)&client_addr, &addrlen);
	if (client_fd == -1)
		throw std::runtime_error("Cannot accept new connection");
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	FD_SET(client_fd, &session.__all);
	if (client_fd > session.__fd_max)
		session.__fd_max = client_fd;
    //client 생성
}

void Server::receive_message(Session &session, int fd) {
	ssize_t size;
	char buf[510];
    // map<string, int> a;
    // a["KICK"]

	while ((size = recv(fd, buf, 512, 0)) == -1 && errno != EINTR);
	buf[size] = '\0';

	if (size <= 0)//특정 fd가 오류가있을시 || 클라이언트종료 해당 fd 연결해제
	{
		if (size == -1 && errno & (EAGAIN | EWOULDBLOCK | EINTR))
			return;
		disconnect_client(session, fd);
	}
	else//커맨드에 따른 동작수행
	{
		Message msg(fd, buf);
		//broad_cast(session, buf, fd);
        if (__cmd_list.find(djb2(msg.__command)) != __cmd_list.end())
        {
            (this->*(*__cmd_list).second)(msg);
        }
        else
        {
            //responses
        }
	}
    /*
     * * 1. 파싱
     *      1-2. 오류체크 커맨드, 프리픽스, 이상 체크 패러미터는 string 으로 넘긴다.
     *  2. 동작수행
     *  3. 반환.
     *  note.
     *  파싱은 서버에서한다.
     *  서버는 각 커맨드에 대한 맵을 가지고있다 key 는 커맨드 스트링 해쉬값, value 는 함수포인터
     *
     */
	//수정 -> 해당 fd 만 닫고 데이터 처리  나중에처리.
}

void Server::disconnect_client(Session &session, int fd) {//jaewkim 알아올게
	close(fd);
	FD_CLR(fd, &session.__all);
}

void Server::send_message(int fd, char *buf) {
	if (send(fd, buf, strlen(buf), 0) == -1)
		return ;
} //좀 정의해야됨

void Server::broad_cast(Session &session, char *buf, int fd)
{
	for (int i = session.__fd + 1; i <= session.__fd_max; i++)
	{
		if (i != fd)
			send_message(i, buf);
	}
}

Server::~Server() {
	delete __channels;
	delete __clients;
}