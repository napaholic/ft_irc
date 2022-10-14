//
// Created by jaewkim on 2022/09/23.
//

#include "../inc/server.hpp"

Server::Server(const std::string &port, const std::string &password) : __port(port), __password(password)
{
    __port_int = std::atoi(port.c_str());
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("NICK"), &Server::nick));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("QUIT"), &Server::quit));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("PASS"), &Server::pass));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("JOIN"), &Server::join));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("USER"), &Server::user));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("TOPIC"), &Server::topic));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("INVITE"), &Server::invite));
    //__cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("MODE"), &Server::mode));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("PRIVMSG"), &Server::privmsg));
    //    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("KICK"), &Server::KICK));
    //    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("MODE"), &Server::MODE));
    //    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("PRIVMSG"), &Server::PRIVMSG));
    //    __cmd_list.insert(std::pair<unsigned long, (Server::*)()>(djb2("NOTICE"), &Server::NOTICE));

    // hash 맵 key는 string 해쉬값, value는 함수포인터 주소. 인자는 패러미터 string
    //  map<long long, class::method>
}

void Server::run(Session &session)
{
    for (int i = 0; i < session.__fd_max + 1; i++)
    {
        if (FD_ISSET(i, &session.__reads) == 0)
            continue;
        if (i == session.__fd) //
            accept_client(session);
        else
            receive_message(session, i);
    }
}

void Server::accept_client(Session &session)
{
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
    // client 생성
    Client *tmp = new Client(client_fd, client_addr);
    __clients.insert(tmp);
    std::cout << "new connection from: " << client_fd << std::endl;
}

void Server::receive_message(Session &session, int fd)
{
    ssize_t size;
    char buf[510];
    Client *tmp_client;
    Channel *tmp_channel;
    Message *msg;
    // map<string, int> a;
    // a["KICK"]

    while ((size = recv(fd, buf, 512, 0)) == -1 && errno != EINTR)
        ;
    buf[size] = '\0';

    if (size <= 0) //특정 fd가 오류가있을시 || 클라이언트종료 해당 fd 연결해제
    {
        if (size == -1 && errno & (EAGAIN | EWOULDBLOCK | EINTR))
            return;
        disconnect_client(session, fd);
    }
    else //커맨드에 따른 동작수행
    {
        tmp_client = findClient(fd);
        tmp_client->setMessage(buf);
        msg = tmp_client->getMessage();
        // std::cout << buf;
        // broad_cast(session, buf, fd);
        if (__cmd_list.find(msg->getCommand()) != __cmd_list.end())
        {
            std::cout << "cmd by: " << tmp_client->getSocket() << " " << buf;
            CALL_MEMBER_FN(*this, __cmd_list[msg->getCommand()])(*tmp_client);
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

void Server::disconnect_client(Session &session, int fd)
{ // jaewkim 알아올게
    close(fd);
    FD_CLR(fd, &session.__all);
}

void Server::send_message(int fd, const char buf[])
{
    if (send(fd, buf, strlen(buf), 0) == -1)
        return;
} //좀 정의해야됨

void Server::send_message(int fd, std::string str)
{
    str.append("\r\n");
    char *buf = const_cast<char *>(str.c_str());
    if (send(fd, buf, strlen(buf), 0) == -1)
        return;
} //좀 정의해야됨

void Server::send_message(Channel *channel, std::string text)
{
    for (std::set<Client *>::iterator it = channel->getActiveClients().begin();
    it != channel->getActiveClients().end(); ++it)
        send_message((*it)->getSocket(), text);
}

void Server::broad_cast(Session &session, char *buf, int fd)
{
    for (int i = session.__fd + 1; i <= session.__fd_max; i++)
    {
        if (i != fd)
            send_message(i, buf);
    }
}

Server::~Server()
{
    // delete __channels;
}

//   서버 접속 시 패스워드와 같은지 확인해주는 명령어
void Server::pass(Client &client)
{
    Message &msg = *(client.getMessage());

    if (client.getAllowed())
        return send_message(client.getSocket(), ERR_ALREADYREGISTRED);
    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS("PASS"));
    if (*msg.getParameters().begin() == __password)
        client.setAllowed(1);
}

Client *Server::findClient(int fd)
{
    std::set<Client *>::iterator it = __clients.begin();
    while (it != __clients.end())
    {
        if ((*it)->getSocket() == fd)
            return (*it);
        ++it;
    }
    return NULL;
}

Client *Server::findClient(std::string nick)
{
    std::set<Client *>::iterator it = __clients.begin();
    while (it != __clients.end())
    {
        if ((*it)->getNickname() == nick)
            return (*it);
        ++it;
    }
    return NULL;
}

// Channel *Server::getChannel(int channel_key)
//{
//     std::map<int, Channel>::iterator it = __channels->begin();
//     while (it != __channels->end())
//     {
//         if ((*it).first == channel_key)
//             return &(*it).second;
//         ++it;
//     }
//     return NULL;
// }

Channel *Server::findChannel(std::string channel)
{
    std::set<Channel *>::iterator it = __channels.begin();
    while (it != __channels.end())
    {
        if ((*it)->__name == channel)
            return (*it);
        ++it;
    }
    return NULL;
}

bool Server::isErrorNick(std::string nick)
{
    if (nick.size() > 9)
        return true;
    if (!std::isalpha(nick[0]))
        return true;
    for (size_t i = 1; i < nick.size(); i++)
    {
        if (std::isalnum(nick[i]))
            continue;
        if (std::strchr(SPECIAL, nick[i]))
            continue;
        return true;
    }
    return false;
}

void Server::newNickname(Client &client)
{
    Message &msg = *(client.getMessage());

    client.setNickname(*msg.getParameters().begin());
    if (client.allowClient())
        send_message(client.getSocket(), RPL_WELCOME(client.getNickname()));
}

void Server::changeNickname(Client &client)
{
    Message &msg = *(client.getMessage());

    if (client.getNickname() == *msg.getParameters().begin())
        return;
    client.setNickname(*msg.getParameters().begin());
    std::cout << "nickname changed to " << client.getNickname() << std::endl;
    findClient(client.getNickname())->make_prefix();
    std::cout << client.getPrefix() << std::endl;
}

void Server::nick(Client &client)
{
    Message &msg = *(client.getMessage());

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NONICKNAMEGIVEN);
    if (findClient(*(msg.getParameters().begin())) != NULL)
        return send_message(client.getSocket(), ERR_NICKNAMEINUSE(*(msg.getParameters().begin())));
    if (isErrorNick(*(msg.getParameters().begin())) == true)
        return send_message(client.getSocket(), ERR_ERRONEUSNICKNAME(*(msg.getParameters().begin())));

    switch (client.getAllowed())
    {
    case 1:
        newNickname(client);
        break;
    case 2:
        changeNickname(client);
        break;
    default:
        break;
    }
}

void Server::user(Client &client)
{
    Message &msg = *(client.getMessage());

    if (client.getAllowed() == 2)
        return send_message(client.getSocket(), ERR_ALREADYREGISTRED);
    if (msg.getParameters().size() < 4)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS("USER"));

    client.setUsername(*msg.getParameters().begin());
    client.setHostname(*(++msg.getParameters().begin()));
    client.setRealname(*(++(++(++msg.getParameters().begin()))));
    if (client.allowClient())
        send_message(client.getSocket(), RPL_WELCOME(client.getNickname()));
}

void Server::quit(Client &client)
{
    std::string announce =
        client.getMessage()->getParamSize() > 0 ? client.getMessage()->combineParameters() : client.getNickname();

    for (std::set<Channel *>::iterator it = __channels.begin(); it != __channels.end(); ++it)
    {
        if ((*it)->isClient(client.getNickname()))
            (*it)->eraseClient(client.getNickname());
    }
    send_message(__port_int, announce);
    close(client.getSocket());
}

void Server::createChannel(const std::string &name, Client *client)
{
    Channel *channel = new Channel(name, client);
    __channels.insert(channel);
    client->setChannelName(name);
}

void Server::join(Client &client)
{
    Message &msg = *(client.getMessage());

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS("JOIN"));

    for (std::vector<std::string>::const_iterator it = msg.getParameters().begin(); it != msg.getParameters().end();
         ++it)
    {
        std::string channel_name = *it;
        if (channel_name[0] != '#')
        {
            send_message(client.getSocket(), ERR_BADCHANMASK(channel_name));
            continue;
        }
        Channel *channel = findChannel(channel_name);
        if (channel == NULL)
            createChannel(channel_name, &client);
        else
            channel->addClient(&client);
        if (channel->__topic != "")
        {
            std::string ret = RPL_TOPIC(channel_name, client.getNickname());
            send_message(client.getSocket(), ret);
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
    if (channel->isClient(msg.__client->__nickname) == 0)
    {
        send_message(msg.__client->__socket, ERR_NOTONCHANNEL(channel->__name));
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

void Server::invite(Client &client) // RPL_AWAY
{
    Message &msg = *(client.getMessage());

    if (msg.getParameters().size() < 2)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS("INVITE"));
    std::string nickname = *msg.getParameters().begin();
    if (findClient(nickname) == NULL)
        return send_message(client.getSocket(), ERR_NOSUCHNICK(nickname));
    Channel *channel = findChannel(*(++msg.getParameters().begin()));
    // I think it should be changed that inviter is operator of the channel.
    // if (findClient(nickname) == NULL)
    //     return send_message(__port_int, ERR_NOTONCHANNEL(nickname));
    if (channel->isClient(nickname) == true)
        return send_message(__port_int, ERR_USERONCHANNEL(nickname, channel->getName()));

    channel->addClient(&client);
    send_message(client.getSocket(), RPL_INVITING(channel->getName(), nickname));
}

std::vector<std::string> Server::split(std::string str, char Delimiter)
{
    std::istringstream iss(str); // istringstream에 str을 담는다.
    std::string buffer;          // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼

    std::vector<std::string> result;

    // istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
    while (std::getline(iss, buffer, Delimiter))
    {
        if (buffer.size() > 0)
        {
            result.push_back(buffer);
        }
    }

    return result;
}

void Server::privmsg(Client &client)
{
    Message &msg = *(client.getMessage());
    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NORECIPIENT("PRIVMSG"));
    if (msg.getParameters().size() == 1)
        return send_message(client.getSocket(), ERR_NOTEXTTOSEND);

    std::vector<std::string> targets = split(*msg.getParameters().begin(), ',');
    std::string text = *(++msg.getParameters().begin());

    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        std::string target = *it;
        if (target[0] == '#')
        {
            if (findChannel(target) == 0)
            {
                send_message(client.getSocket(), ERR_NOSUCHCHANNEL(target));
                continue;
            }
            send_message(findChannel(target), text);
        }
        else
        {
            if (findClient(target) == 0)
            {
                send_message(client.getSocket(), ERR_NOSUCHNICK(target));
                continue;
            }
            send_message(findClient(target)->getSocket(), text);
        }
    }
}

//void Server::modeChannel(std::string target, Client &client, std::vector<std::string> &parameters)
//{
//    Channel *channel = findChannel(target);
//
//    if (channel == NULL)
//        return send_message(client.getSocket(), ERR_NOSUCHCHANNEL(target));
//    if (channel->isOperator(client.getNickname()) == false)
//        return send_message(client.getSocket(), ERR_CHANOPRIVSNEEDED(target));
//
//}

//void Server::mode(Client &client)
//{
//     Message &msg = *(client.getMessage());
//
//    if (msg.getParameters().size() == 0)
//        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS("MODE"));
//
//    std::vector<std::string> params = msg.getParameters().begin();
//    std::string target = *msg.getParameters().begin();
//    if (target[0] == '#')
//        return (modeChannel(target, client, params));
//}