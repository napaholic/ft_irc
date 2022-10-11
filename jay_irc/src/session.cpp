//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/session.hpp"

Session::Session(const std::string &port, const std::string &password)
:	__port(port), __password(password)
{
	check_port(port);
	create_socket();
}

Session::~Session() {
	close(__fd);
}

void	Session::check_port(const std::string &port)
{
	int port_num = std::atoi(port.c_str());
	if (port_num < 1024 || port_num >49151)//포트, 리눅스, 네트워크를 공부해야된다.
		throw std::runtime_error("port number must be from 1024 to 49151");
}

void	Session::create_socket() {
	__fd = socket(AF_INET, SOCK_STREAM, 0);
	if (__fd == -1)
		throw std::runtime_error("Cannot open socket");
	
	int optval = 1;
	if (setsockopt(__fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		throw std::runtime_error("Cannot set socket option");
	
	memset(&__addr, 0, sizeof(__addr));
	__addr.sin_family = AF_INET;
	__addr.sin_addr.s_addr = INADDR_ANY;
	__addr.sin_port = htons(std::atoi(__port.c_str()));
	
	
	if (bind(__fd, (struct sockaddr *)&__addr, sizeof(__addr)) == -1)
		throw std::runtime_error("Cannot bind server socket");
	if (listen(__fd, MAX_QUEUE) == -1)
		throw std::runtime_error("Error Create Socket");
	
	FD_ZERO(&__all);
	FD_ZERO(&__reads);
	FD_SET(__fd, &__all);
	__fd_max = __fd;
}

void Session::select_socket() {
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	
	__reads = __all;
	if (select(__fd_max + 1, &__reads, 0, 0, &timeout) < 0)
		throw std::runtime_error("select error");
}


















