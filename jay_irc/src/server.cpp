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
    __cmd_list.insert(
        std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("INVITE"), &Server::invite));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("MODE"), &Server::mode));
    __cmd_list.insert(
        std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("PRIVMSG"), &Server::privmsg));
    __cmd_list.insert(
        std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("NOTICE"), &Server::notice));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("PART"), &Server::part));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("KICK"), &Server::kick));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("NAMES"), &Server::names));
    __cmd_list.insert(std::make_pair<unsigned long, void (Server::*)(Client & client)>(djb2("LIST"), &Server::list));

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
    char *tmp = strcpy(tmp, buf);
    strcat(tmp, "\r\n");
    if (send(fd, tmp, strlen(tmp), 0) == -1)
        return;
    // send_message(fd, std::string(buf));
} //좀 정의해야됨

void Server::send_message(int fd, std::string str)
{
    str.append("\r\n");
    char *buf = const_cast<char *>(str.c_str());
    // std::cout << buf << std::endl;
    if (send(fd, buf, strlen(buf), 0) == -1)
        return;
} //좀 정의해야됨

void Server::send_message(Channel *channel, Client *client, std::string text)
{
    for (std::set<Client *>::iterator it = channel->getActiveClients().begin(); it != channel->getActiveClients().end();
         ++it)
    {
        if (findClient((*it)->getSocket()) == client)
            continue;
        send_message((*it)->getSocket(), text);
    }
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
    std::string user = client.getNickname();

    if (client.getAllowed())
        return send_message(client.getSocket(), ERR_ALREADYREGISTRED(user));
    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "PASS"));
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
        if ((*it)->getName() == channel)
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
    std::string user = client.getNickname();

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NONICKNAMEGIVEN(user));
    if (findClient(*(msg.getParameters().begin())) != NULL)
        return send_message(client.getSocket(), ERR_NICKNAMEINUSE(user, *(msg.getParameters().begin())));
    if (isErrorNick(*(msg.getParameters().begin())) == true)
        return send_message(client.getSocket(), ERR_ERRONEUSNICKNAME(user, *(msg.getParameters().begin())));

    send_message(client.getSocket(), client.makeReply());
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
    std::string user = client.getNickname();

    if (client.getAllowed() == 2)
        return send_message(client.getSocket(), ERR_ALREADYREGISTRED(user));
    if (msg.getParameters().size() < 4)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "USER"));

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
        if ((*it)->isClientInChannel(client))
            (*it)->eraseClient(&client);
    }
    send_message(__port_int, announce);
    close(client.getSocket());
}

Channel *Server::createChannel(const std::string &name, Client *client)
{
    Channel *channel = new Channel(name, client);
    __channels.insert(channel);
    client->setChannelName(name);
    return (channel);
}

void Server::join(Client &client)
{
    Message &msg = *(client.getMessage());
    std::string user = client.getNickname();

    if (msg.getParameters()[0] == "#")
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "JOIN"));
    std::string channel_name = *msg.getParameters().begin();
    if (channel_name[0] != '#')
        return send_message(client.getSocket(), ERR_BADCHANMASK(user, channel_name));
    Channel *channel = findChannel(channel_name);
    if (channel == NULL)
    {
        channel = createChannel(channel_name, &client);
        channel->addOperator(&client);
    }
    else
        channel->addClient(&client);
    send_message(client.getSocket(), client.makeReply());
    if (channel->getTopic() != "")
        return send_message(client.getSocket(), RPL_TOPIC(user, channel_name, client.getNickname()));
}

void Server::part(Client &client)
{
    Message &msg = *(client.getMessage());
    std::string user = client.getNickname();

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "PART"));

    std::string channel_name = *msg.getParameters().begin();
    Channel *channel = findChannel(channel_name);
    if (channel == NULL)
        return send_message(client.getSocket(), ERR_NOSUCHCHANNEL(user, channel_name));
    if (!channel->isClientInChannel(client))
        return send_message(client.getSocket(), ERR_NOTONCHANNEL(user, channel_name));
    else
    {
        send_message(client.getSocket(), client.makeReply());
        channel->eraseClient(&client);
    }
}

void Server::kick(Client &client)
{
    Message &msg = *(client.getMessage());
    std::string user = client.getNickname();

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "KICK"));

    std::string channel_name = *msg.getParameters().begin();
    Client *targetClient = findClient(*(++msg.getParameters().begin()));

    if (channel_name[0] != '#')
        return send_message(client.getSocket(), ERR_BADCHANMASK(user, channel_name));
    Channel *channel = findChannel(channel_name);
    if (channel == NULL)
        return send_message(client.getSocket(), ERR_NOSUCHCHANNEL(user, channel_name));
    if (!channel->isClientInChannel(client))
        return send_message(client.getSocket(), ERR_NOTONCHANNEL(user, channel_name));
    if (!channel->findOperator(client))
        return send_message(client.getSocket(), ERR_CHANOPRIVSNEEDED(user, channel_name));
    else
    {
        channel->eraseClient(targetClient);
    }
}

void Server::topic(Client &client)
{
    std::string user = client.getNickname();

    if (client.getMessage()->getParamSize() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "TOPIC"));
    Channel *channel = findChannel(*client.getMessage()->getParameters().begin());
    if (channel->isClientInChannel(client) == 0)
        return send_message(client.getSocket(), ERR_NOTONCHANNEL(user, channel->getName()));
    if (client.getMessage()->getParamSize() == 1)
        return send_message(client.getSocket(), RPL_NOTOPIC(user, channel->getName()));
    std::string topic = *(++client.getMessage()->getParameters().begin());
    channel->setTopic(topic);
    send_message(client.getSocket(), RPL_TOPIC(user, channel->getName(), client.getNickname()));
}

void debug(const std::string &msg)
{
    std::cerr << "DBG :" << msg << std::endl;
}

void Server::invite(Client &client) // RPL_AWAY
{
    debug("Enter Server::invite()");
    Message &msg = *(client.getMessage());
    std::string user = client.getNickname();

    debug("Enter Server::invite() line 422");
    if (msg.getParameters().size() < 2)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "INVITE"));
    debug("Enter Server::invite() line 425");
    std::string nickname = *msg.getParameters().begin();
    Client *invitee = findClient(nickname);
    debug("Enter Server::invite() line 427");
    if (invitee == NULL)
        return send_message(client.getSocket(), ERR_NOSUCHNICK(user, nickname));
    debug("Enter Server::invite() line 430");
    debug(*(++msg.getParameters().begin()));
    Channel *channel = findChannel(*(++msg.getParameters().begin()));
    if (channel == NULL)
        return;
    debug("Enter Server::invite() line 432");
    if (channel->isClientInChannel(*invitee) == true)
        return send_message(client.getSocket(), ERR_USERONCHANNEL(channel->getName(), invitee->getNickname()));
    debug("Enter Server::invite() line 435");
    channel->addClient(invitee);
    debug("Enter Server::invite() line 437");
    send_message(client.getSocket(), RPL_INVITING(channel->getName(), invitee->getNickname()));
}

std::vector<std::string> Server::splitPrivmsgTarget(std::string str, char Delimiter)
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
    std::string user = client.getNickname();

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NORECIPIENT(user, "PRIVMSG"));
    if (msg.getParameters().size() == 1)
        return send_message(client.getSocket(), ERR_NOTEXTTOSEND(user));

    std::vector<std::string> targets = splitPrivmsgTarget(*msg.getParameters().begin(), ',');
    std::string text = *(++msg.getParameters().begin());

    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        std::string target = *it;
        if (target[0] == '#')
        {
            if (findChannel(target) == 0)
            {
                send_message(client.getSocket(), ERR_NOSUCHCHANNEL(user, target));
                continue;
            }
            send_message(findChannel(target), &client, client.makeReply());
        }
        else
        {
            if (findClient(target) == 0)
            {
                send_message(client.getSocket(), ERR_NOSUCHNICK(user, target));
                continue;
            }
            send_message(findClient(target)->getSocket(), client.makeReply());
        }
    }
}

void Server::notice(Client &client)
{
    Message &msg = *(client.getMessage());
    if (msg.getParameters().size() == 0)
        return;
    if (msg.getParameters().size() == 1)
        return;

    std::vector<std::string> targets = splitPrivmsgTarget(*msg.getParameters().begin(), ',');
    std::string text = *(++msg.getParameters().begin());

    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        std::string target = *it;
        if (target[0] == '#')
        {
            if (findChannel(target) == 0)
                continue;
            send_message(findChannel(target), &client, client.makeReply());
        }
        else
        {
            if (findClient(target) == 0)
                continue;
            send_message(findClient(target)->getSocket(), client.makeReply());
        }
    }
}

void Server::modeChannel(std::string target, Client &client, std::vector<std::string>::iterator param)
{
    Channel *channel = findChannel(target);
    std::string option_o = *(++param);
    std::string nickName = *(++(++param));
    std::string user = client.getNickname();

    if (channel == NULL)
        return send_message(client.getSocket(), ERR_NOSUCHCHANNEL(user, target));
    if (channel->findOperator(client) == false)
        return send_message(client.getSocket(), ERR_CHANOPRIVSNEEDED(user, target));
    if (channel->findClient(nickName) == NULL)
        return send_message(client.getSocket(), ERR_NOTONCHANNEL(user, nickName));

    if (option_o == "+o")
    {
        channel->addOperator(channel->findClient(nickName));
    }
    else if (option_o == "-o")
    {
        channel->delOperator(channel->findClient(nickName));
    }
    else
        return send_message(client.getSocket(), ERR_UNKNOWNMODE(user, option_o));
}

void Server::mode(Client &client)
{
    Message &msg = *(client.getMessage());
    std::string user = client.getNickname();

    if (msg.getParameters().size() == 0)
        return send_message(client.getSocket(), ERR_NEEDMOREPARAMS(user, "MODE"));

    std::string target = *msg.getParameters().begin();
    if (target[0] == '#')
        return (modeChannel(target, client, msg.getParameters().begin()));
}

void Server::list(Client &client)
{
    Message &msg = *(client.getMessage());
    std::string user = client.getNickname();

    if (__channels.size() == 0)
        ;
    else if (msg.getParameters().size() == 1)
    {
        std::string target = *msg.getParameters().begin();
        Channel *channel = findChannel(target);
        send_message(client.getSocket(),
                     RPL_LIST(client.getNickname(), channel->getName(),
                              std::to_string(channel->getActiveClients().size()), channel->getTopic()));
    }

    else
    {
        for (std::set<Channel *>::iterator it = __channels.begin(); it != __channels.end(); ++it)
            send_message(client.getSocket(),
                         RPL_LIST(client.getNickname(), (*it)->getName(),
                                  std::to_string((*it)->getActiveClients().size()), (*it)->getTopic()));
    }
    send_message(client.getSocket(), RPL_LISTEND(client.getNickname()));
}

void Server::names(Client &client)
{
    Message &msg = *(client.getMessage());
    std::string result = "";
    Channel *tmp_ch;

    if (msg.getParameters().size() == 0)
        return list(client);
    std::vector<std::string>::iterator params = ++(msg.getParameters().begin());
    if ((*params)[0] == '#')
    {
        std::vector<std::string> channelList = splitPrivmsgTarget(*params, ',');
        std::vector<std::string>::iterator it = channelList.begin();
        while (it != channelList.end())
        {
            if ((tmp_ch = findChannel((*it).substr((*it)[1], (*it).length()))) != NULL)
            {
                result += tmp_ch->listingActiveClient(client);
            }
            else
            {
                result = RPL_ENDOFNAMES((*it).substr((*it)[1], (*it).length()));
                send_message(client.getSocket(), result);
                return;
            }
            result += "\n";
            ++it;
        }
        send_message(client.getSocket(), RPL_NAMREPLY(result));
    }
}