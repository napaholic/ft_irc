#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAXLINE 10
#define PORTNUM 3600
#define SOCK_SETSIZE 1021

int main(int argc, char **argv)
{
	int listen_fd, client_fd;
	socklen_t addrlen;
	int maxfd = 0;
	int readn;
	char buf[MAXLINE];
	fd_set readfds, allfds;

	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	struct sockaddr client_addr;
	struct addrinfo *ai = NULL;
	struct addrinfo hints;

	// preprocessing
    fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //IPV4 + IPV6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
	// ai = linked list, DNS서버로부터 받은 네트워크 주소 정보(IP 주소)를 돌려주는 output 매개변수
	if (getaddrinfo(0, "3600", &hints, &ai) != 0)
		exit(1);

	// binding
    int optval = 1;
    for (struct addrinfo *p = ai; p != NULL; p = p->ai_next)
    {
        printf("this cannon name is :%s\n", p->ai_canonname);
        printf("this cannon name is :%d\n", p->ai_family);
        printf("this cannon name is :%d\n", p->ai_socktype);
        printf("this cannon name is :%d\n", p->ai_addr->sa_family);
        printf("this cannon name is :%s\n", p->ai_addr->sa_data);
        printf("this cannon name is :%d\n", p->ai_protocol);
        printf("===============================================\n");
    }
    
    
	for (struct addrinfo *p = ai; p != NULL; p = p->ai_next)
	{
		listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listen_fd == -1)
			continue;
		// 재사용(bind) 가능한 socket fd로 설정
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        if (bind(listen_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(listen_fd);
            continue;
        }
        printf("maxfd is : %d\n", listen_fd);
        break;
	}
	
    if(listen(listen_fd, 10) == -1)
	{
		perror("listen error");
		return 1;
	}  
	
	// readfds = 복사본
	FD_ZERO(&allfds);
    FD_ZERO(&readfds);
	FD_SET(listen_fd, &allfds);

	maxfd = listen_fd;
    printf("maxfd is : %d\n", maxfd);
	while(1)
	{
		// non-blocking
	    timeout.tv_sec = 2;
	    timeout.tv_usec = 0;

        readfds = allfds;
	    if (select(maxfd + 1, &readfds, STDIN_FILENO, NULL, &timeout) == -1)
		    perror("Could not call select.");
        printf("secon select maxfd is : %d\n", maxfd);
        for (int i = 0; i <= maxfd; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
				// new connection
                if (i == listen_fd)
                {
	                addrlen = sizeof(client_addr);
	                client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addrlen);
	                if (client_fd == -1)
	                {
	                	perror("Error: Accept.");
	                	exit(1);
	                }
	                fcntl(client_fd, F_SETFL, O_NONBLOCK);
	                FD_SET(client_fd, &allfds);
	                if (client_fd > maxfd)
	                	maxfd = client_fd;
                    printf("Accept %d OK\n", client_fd);
                }
                else // receive & send other client
                {
                    if ((readn = recv(i, buf, MAXLINE - 1, 0)) == 0)
                    {
                        printf("close\n");
					    close(i);
					    FD_CLR(i, &allfds);
                    }
                    else if (readn == -1)
                    {
                        printf("readn = -1\n");
                    }
                    else {
                        for (int j = listen_fd + 1; j <= maxfd; j++)
                        {
                            if (FD_ISSET(j, &allfds)) {
                                buf[readn] = '\0';
                                if(send(j, buf, strlen(buf), 0) == -1)
                                {
                                    perror("send error");
                                }
                            }
                        }
                    }
                }
            }
        } 
	}
    freeaddrinfo(ai);
}
