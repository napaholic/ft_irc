//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/server.hpp"

Server::Server(const std::string &port, const std::string &password)
: __port(port), __password(password) {
	__port_int = std::atoi(port.c_str());
}

void Server::run() {

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



void Server::receive_message(int fd) {}//좀 정의해야됨

void Server::disconnect_client(Session &session, int fd) {//jaewkim 알아올게
	close(fd);
	FD_CLR(fd, &session.__all);
}

void Server::send_message(int fd) {} //좀 정의해야됨

Server::~Server() {
	delete __channels;
	delete __clients;
}