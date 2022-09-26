//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/server.hpp"

Server::Server(const std::string &port, const std::string &password)
: __port(port), __password(password) {
	__port_int = std::atoi(port.c_str());
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
}

void Server::receive_message(Session &session, int fd) {
	ssize_t size;
	char buf[512];

	while ((size = recv(fd, buf, 512, 0)) == -1 && errno != EINTR);
	buf[size] = '\0';

	if (size <= 0)
	{
		if (size == -1 && errno & (EAGAIN | EWOULDBLOCK | EINTR))
			return;
		disconnect_client(session, fd);
	}
	else
		broad_cast(session, buf, fd);
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