/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hynam <hynam@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 18:37:56 by hynam             #+#    #+#             */
/*   Updated: 2022/09/16 15:53:19 by hynam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>

#define MAX_QUEUE 10

class Client {};
class Channel {};

class Server {
public:
    // Server();
    explicit Server(const std::string &port, const std::string &password);
    void    check_port(const std::string &port);
    void    run();
	void    create_socket();
	void    select_socket();
	void    accept_client();
	void    receive_message(int fd);
	void    disconnect_client(int fd);
    ~Server();
private:
    const std::string   __port;
    const std::string   __password;
    std::vector<Client> *__clients;
    std::map<std::string, Channel>  *__channels;

    fd_set  __all;
    fd_set  __reads;
    int     __fd;
    int     __fd_max;
    struct sockaddr_in  __addr;
};

#endif
