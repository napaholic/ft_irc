/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hynam <hynam@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 18:47:09 by hynam             #+#    #+#             */
/*   Updated: 2022/09/16 16:25:22 by hynam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_server.hpp"

Server::Server(const std::string &port, const std::string &password)
	: __port(port), __password(password) 
{
    check_port(port);
	// fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);
	// fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	create_socket();
	__clients = new std::vector<Client>;
}

Server::~Server() {
	close(__fd);
	delete __clients;
}

void	Server::check_port(const std::string &port) {
	int port_num = std::atoi(port.c_str());
	if (port_num < 1024 || port_num > 49151)
		throw std::runtime_error("port number must be from 1024 to 49151");
}

void	Server::run() {
	while (1)
	{
		select_socket();
		for (int i = 0; i <= __fd_max; i++)
		{
			if (FD_ISSET(i, &__reads))
			{
				if (i == __fd)
				{
					accept_client();
					continue;
				}
				receive_message(i);
			}
		}
	}
}

void	Server::create_socket() {
	__fd = socket(AF_INET, SOCK_STREAM, 0);
	if (__fd == -1)
		throw std::runtime_error("Cannot open socket");
	
	int optval = 1;
	if (setsockopt(__fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		throw std::runtime_error("Cannot set socket option");
	
	memset(&__addr, 0, sizeof(__addr));
	this->__addr.sin_family = AF_UNSPEC;
	this->__addr.sin_addr.s_addr = INADDR_ANY;
	this->__addr.sin_port = htons(std::atoi(__port.c_str()));
	
	if (bind(__fd, (struct sockaddr *)&__addr, sizeof(__addr)) == -1)
		throw std::runtime_error("Cannot bind server socket");
	
	if (listen(__fd, MAX_QUEUE) == -1)
		throw std::runtime_error("Error Create Socket");

	FD_ZERO(&__all);
	FD_ZERO(&__reads);
	FD_SET(__fd, &__all);
	__fd_max = __fd;
}

void	Server::select_socket() {
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	__reads = __all;
	if (select(__fd_max + 1, &__reads, 0, 0, &timeout) < 0)
		throw std::runtime_error("select error");
}

void	Server::accept_client() {
	int	client_fd;
	struct sockaddr_in	client_addr;
	socklen_t	addrlen = sizeof(struct sockaddr_in);

	memset(&client_addr, 0, sizeof(client_addr));
	client_fd = accept(__fd, (struct sockaddr *)&client_addr, &addrlen);
	if (client_fd == -1)
		throw std::runtime_error("Cannot accept new connection");
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	FD_SET(client_fd, &__all);
	if (client_fd > __fd_max)
		__fd_max = client_fd;
	/**
	 * @TODO 
	 * Client 생성 & Client에게 메세지 send
	 */
}

void	Server::receive_message(int fd) {
	ssize_t size;
	char buf[512];
	
	while ((size = recv(fd, buf, 512, 0)) == -1 && errno != EINTR);
	buf[size] = '\0';

	if (size <= 0)
	{
		if (size == -1 && errno & (EAGAIN | EWOULDBLOCK | EINTR))
			return;
		disconnect_client(fd);
	}
    //수정 -> 해당 fd 만 닫고 데이터 처리  나중에처리.
}

void	Server::disconnect_client(int fd) {
	std::cout << "disconnect" << std::endl;
	close(fd);
	FD_CLR(fd, &__all);
}
