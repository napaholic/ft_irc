//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/server.hpp"

Server::Server(const std::string &port, const std::string &password)
: __port(port), __password(password) {
	__port_int = std::atoi(port.c_str());
	__cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("NICK"), &Server::nick));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("QUIT"), &Server::quit));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("PASS"), &Server::pass));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("JOIN"), &Server::join));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("USER"), &Server::user));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("TOPIC"), &Server::topic));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Message &msg)>(djb2("INVITE"), &Server::invite));
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
		if (i == session.__fd)//
			accept_client(session);
		else
			//receive_message(session, i);
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
    Client *tmp = new Client(client_fd, client_addr);
    __clients.push_back(tmp);
    send_message(tmp->__socket, "LOGIN FIRST");
    std::cout << "new connection" << client_fd << std::endl;
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
		Message msg(getClient(fd), buf);
        std::cout << buf << std::endl;
		//broad_cast(session, buf, fd);
        if (__cmd_list.find(msg.__command) != __cmd_list.end())
        {
			std::cout << "some one use command" << std::endl;
            CALL_MEMBER_FN(*this, __cmd_list[msg.__command])(msg);
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

void Server::send_message(int fd, const char buf[]) {
    std::cout << buf << std::endl;
	if (send(fd, buf, strlen(buf), 0) == -1)
		return ;
} //좀 정의해야됨

void Server::send_message(int fd, std::string str) {
    char *buf = const_cast<char *>(str.c_str());
    std::cout << buf << std::endl;
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
}

void Server::pass(Message &msg)
{
    //   서버 접속 시 패스워드와 같은지 확인해주는 명령어
	std::cout << "some one use pass" << std::endl;
    std::cout << msg.__client->__allowed << std::endl;
    if (msg.__client->__allowed)
    {
        send_message(msg.__client->__socket, ERR_ALREADYREGISTRED);
        return;
    }
    if (msg.__parameters.size() == 0)
    {
        send_message(msg.__client->__socket, ERR_NEEDMOREPARAMS("PASS"));
        return;
    }
    else
    {
        std::cout << "else" << std::endl;
        if (*msg.__parameters.begin() == __password)
            msg.__client->__allowed = 1;
    }
}

Client *Server::getClient(int fd)
{
    std::vector<Client *>::iterator it = __clients.begin();
    while (it != __clients.end())
    {
        if ((*it)->__socket == fd)
            return (*it);
        ++it;
    }
    return NULL;
}

Client *Server::getClient(std::string nick)
{
    std::vector<Client *>::iterator it = __clients.begin();
    while (it != __clients.end())

    {
        if ((*it)->__nickname == nick)
            return (*it);
        ++it;
    }
    return NULL;
}

//Channel *Server::getChannel(int channel_key)
//{
//    std::map<int, Channel>::iterator it = __channels->begin();
//    while (it != __channels->end())
//    {
//        if ((*it).first == channel_key)
//            return &(*it).second;
//        ++it;
//    }
//    return NULL;
//}

Channel *Server::getChannel(std::string channel)
{
    std::map<int, Channel>::iterator it = __channels->begin();
    while (it != __channels->end())
    {
        if ((*it).second.__name == channel)
            return &(*it).second;
        ++it;
    }
    return NULL;
}

bool Server::err_nick(std::string nick)
{
    if (nick.size() > 9)
        return false;
    if (!std::isalpha(nick[0]))
        return false;
    for (size_t i = 1; i < nick.size(); i++) {
        if (std::isalnum(nick[i]))
            continue;
        if (std::strchr(SPECIAL, nick[i]))
            continue;
        return false;
    }
    return true;
}

void Server::new_nick(Message &msg)
{
    if (msg.__parameters.size() == 0)
    {
        send_message(msg.__client->__socket, ERR_NONICKNAMEGIVEN);
        return;
    }
    if (getClient(*msg.__parameters.begin()) != NULL)
    {
        send_message(msg.__client->__socket, ERR_NICKNAMEINUSE(*msg.__parameters.begin()));
        return;
    }
    if (!err_nick(*msg.__parameters.begin()))
    {
        send_message(msg.__client->__socket, ERR_ERRONEUSNICKNAME(*msg.__parameters.begin()));
        return;
    }
    msg.__client->__nickname = *msg.__parameters.begin();
    if (msg.__client->setClient())
    {
        send_message(msg.__client->__socket, RPL_WELCOME(msg.__client->__nickname));
    }
}

void Server::re_nick(Message &msg)
{
    if (msg.__parameters.size() == 0)
    {
        send_message(msg.__client->__socket, ERR_NONICKNAMEGIVEN);
        return;
    }
    if (msg.__client->__nickname == *msg.__parameters.begin())
        return;
    if (getClient(*msg.__parameters.begin()) != NULL)
    {
        send_message(msg.__client->__socket, ERR_NICKNAMEINUSE(*msg.__parameters.begin()));
        return;
    }
    if (err_nick(*msg.__parameters.begin()))
    {
        send_message(msg.__client->__socket, ERR_ERRONEUSNICKNAME(*msg.__parameters.begin()));
        return;
    }
    msg.__client->__nickname = *msg.__parameters.begin();
    msg.__client->make_prefix();
}

void Server::nick(Message &msg)
{
    if (msg.__client->__allowed == 1)
        new_nick(msg);
    else if (msg.__client->__allowed == 2)
        re_nick(msg);
}

void Server::user(Message &msg)
{
    if (msg.__client->__allowed == 2)
    {
        send_message(msg.__client->__socket, ERR_ALREADYREGISTRED);
        return;
    }
    if (msg.__parameters.size() < 4)
    {
        send_message(msg.__client->__socket, ERR_NEEDMOREPARAMS("USER"));
        return;
    }
    msg.__client->__username = *msg.__parameters.begin();
    msg.__client->__hostname = *(++msg.__parameters.begin());
    msg.__client->__realname = *(++(++(++msg.__parameters.begin())));
    if (msg.__client->setClient()) {
        send_message(msg.__client->__socket, RPL_WELCOME(msg.__client->__nickname));
    }
}

void Server::quit(Message &msg)
{
    std::string announce = msg.__parameters.size() > 0 ? *msg.__parameters.begin() : msg.__client->__nickname;

    for (std::map<int, Channel>::iterator it = __channels->begin(); it != __channels->end(); ++it)
    {
        if (it->second.isClient(msg.__client->__nickname))
            it->second.eraseClient(msg.__client->__nickname);
    }
    for (std::vector<Client *>::iterator it = __clients.begin(); it != __clients.end(); ++it)
    {
//        if ((*it)->__nickname == msg.__client->__nickname)
//            __clients->erase(it);// 10월 11일 jaewkim::CLIENT 삭제가 왜 필요한지 논의가필요합니다.
    }
    delete msg.__client;
    send_message(__port_int, announce);
}

void Server::join(Message &msg)
{
    if (msg.__parameters.size() == 0)
    {
        send_message(msg.__client->__socket, ERR_NEEDMOREPARAMS("JOIN"));
        return;
    }
    for (std::vector<std::string>::iterator it = msg.__parameters.begin(); it != msg.__parameters.end(); ++it)
    {
        std::string channel_name = *it;
        if (channel_name[0] != '#') {
            send_message(__port_int, ERR_BADCHANMASK(channel_name));
            continue;
        }
        Channel *channel = getChannel(channel_name);
        if (channel == NULL)
            channel = new Channel(channel_name, msg.__client->__nickname);
        else {
            if (channel->isClient(msg.__client->__nickname))
                channel->addClient(msg.__client->__nickname);
        }
        if (channel->__topic != "") {
            std::string ret = RPL_TOPIC(channel_name, msg.__client->__nickname);
            send_message(msg.__client->__socket, ret);
        }
    }
}

void Server::topic(Message &msg)
{
    if (msg.__parameters.size() == 0)
    {
        send_message(msg.__client->__socket, ERR_NEEDMOREPARAMS("TOPIC"));
        return;
    }
    Channel *channel = getChannel(*msg.__parameters.begin());
    if (channel->isClient(msg.__client->__nickname) == 0) {
        send_message(__port_int, ERR_NOTONCHANNEL(channel->__name));
    }
    if (msg.__parameters.size() == 1)
    {
        send_message(msg.__client->__socket, RPL_NOTOPIC(channel->__name));
        return;
    }
    std::string topic = *(++msg.__parameters.begin());
    channel->__topic = topic;
    std::string ret = RPL_TOPIC(channel->__name, msg.__client->__nickname);
    send_message(msg.__client->__socket, ret);
}

void Server::invite(Message &msg)//RPL_AWAY
{
    if (msg.__parameters.size() < 2)
    {
        send_message(msg.__client->__socket, ERR_NEEDMOREPARAMS("INVITE"));
        return;
    }
    std::string nickname = *msg.__parameters.begin();
    if (getClient(nickname) == NULL)
    {
        send_message(msg.__client->__socket, ERR_NOSUCHNICK(nickname));
        return;
    }
    Channel *channel = getChannel(*(++msg.__parameters.begin()));
    if (getClient(nickname) == NULL) {
        send_message(__port_int, ERR_NOTONCHANNEL(nickname));
        return;
    }
    if (channel->isClient(nickname) == 1) {
        std::string ret = ERR_USERONCHANNEL(nickname, channel->__name);
        send_message(__port_int, ret);
        return;
    }
    channel->addClient(nickname);
    std::string ret = RPL_INVITING(channel->__name, nickname);
    send_message(msg.__client->__socket, ret);
}
