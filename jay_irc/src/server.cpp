//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/server.hpp"

Server::Server(const std::string &port, const std::string &password)
: __port(port), __password(password) {
	__port_int = std::atoi(port.c_str());
	__cmd_list.insert(std::make_pair<unsigned long, void (Server::*)()>(djb2("KICK"), &Server::nick));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)()>(djb2("QUIT"), &Server::quit));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("JOIN"), &Server::JOIN));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)()>(djb2("USER"), &Server::user));
	__cmd_list.insert(std::make_pair<unsigned long, void (Server::*)()>(djb2("KICK"), &Server::user));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("TOPIC"), &Server::TOPIC));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("INVITE"), &Server::INVITE));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("KICK"), &Server::KICK));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("MODE"), &Server::MODE));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("PRIVMSG"), &Server::PRIVMSG));
//    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("NOTICE"), &Server::NOTICE));
    
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
            CALL_MEMBER_FN(*this, __cmd_list[djb2(msg.__command)])();
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

void Server::pass(Message &msg)
{//   서버 접속 시 패스워드와 같은지 확인해주는 명령어
    if (msg.__client->__nickname != "")
        return;
    if (msg.__parameters.size() > 0)//parameters getter 필요할듯 혹은 friend 사용
    {
        if (*msg.__parameters.begin() == __password)
        {
            send_message(msg.__client->__socket, "NOTICE :Password accepted.");
            msg.__client->__allowed = ALLOW;
        }
        else
            send_message(msg.__fd, "NOTICE :Incorrect password.");
        //client 소켓이랑 그냥 소켓의 차이점을 모르겠음. 서버 얘기인가?
    }
}

Client* Server::getClient(std::string nick)
{
    std::vector<Client*>::iterator it = __clients.begin();
    while (it != __clients.end())
    {
        if ((*it)->__nickname == nick)
            return *it;
        ++it;
    }
    return NULL;
}

void Server::new_nick(Message &msg)
{
    char *ret;

    if (msg.__parameters.size() < 1)
    {
        ret = ERR_NEEDMOREPARAMS(NICK);//수정 필요
        send_message(msg.__client->__socket, ret);
        return;
    }
    if (getClient(*msg.__parameters.begin()) != NULL)
    {
        ret = ERR_NICKNAMEINUSE(msg.__parameters.begin());//수정 필요
        send_message(msg.__client->__socket, ret);
        return;
    }
    msg.__client->__nickname = *msg.__parameters.begin();
    if (msg.__client->setClient())
    {
        ret = RPL_WELCOME(*msg.__client->__nickname);
        send_message(msg.__client->socket, ret);
    }
}

void Server::re_nick(Message &msg)
{
    if (msg.__parameters.size() < 1)
        return ERR_NEEDMOREPARAMS(NICK);
    if (msg.__client->__nickname == *msg.__parameters.begin())
        return;
    if (getClient(*msg.__parameters.begin()) != NULL)
        return ERR_NICKNAMEINUSE(*msg.__parameters.begin());
    getClient(msg.__client->__nickname)->__nickname = *msg.__parameters.begin();
    getClient(msg.__client->__nickname)->make_prefix();
}

void Server::nick(Message &msg)
{
    if (msg.__client->__nickname == NULL && msg.__client->__allowed)
        new_nick(msg);
    else
        re_nick(msg);
}

void Server::user(Message &msg)
{
    if (!msg.__client->__allowed)
        return ;
    if (msg.__parameters.size() < 4)
        return ;
    msg.__client->__username = *msg.__parameters.begin();
    msg.__client->__hostname = *(++msg.__parameters.begin());
    msg.__client->__realname = *(++(++(++msg.__parameters.begin())));
    if (msg.__client->setClient())
        RPL_WELCOME(*msg.__client->__nickname);
}

void Server::quit(Message &msg)
{
    std::string announce = msg.__parameters.size() > 0 ? msg.__parameters.begin() : *msg.__client->__nickname;

    for (std::map<int, Channel>::iterator it = __channels.begin(); it != __channels.end(); ++it)
    {
        if ((*it).isClient(msg.__client->__nickname))
        {
            (*it).eraseClient(msg.__client->__nickname);
        }
    }
 //채널 맵 돌아다니면서 클라이언트 삭제하기
    delete msg.__client;
}
