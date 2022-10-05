//
// Created by Yunhui Bae on 2022/10/03.
//

#include "../inc/server.hpp"

void Server::pass(Message &msg)
{//   서버 접속 시 패스워드와 같은지 확인해주는 명령어
    if (getClient(msg.__fd)->__nickname != NULL)
        return;
    if (msg.__parameters.size() > 0)//parameters getter 필요할듯 혹은 friend 사용
    {
        if (*msg.__params.begin() == __password)
        {
            print("Password accepted");
            send_message(msg.__client->__socket, "NOTICE :Password accepted.");
            getClient(msg.__client)->__allowed = ALLOW;
        }
        else
            send_message(msg.__fd, "NOTICE :Incorrect password.");
        //client 소켓이랑 그냥 소켓의 차이점을 모르겠음. 서버 얘기인가?
    }
}

Client *Server::getClient(std::string nick)
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
    if (msg.__parameters.size() < 1)
        return ERR_NEEDMOREPARAMS(NICK);
    if (getClient(*msg.__parameters.begin()) != NULL)
        return ERR_NICKNAMEINUSE(*msg.__parameters.begin());
    msg.__client->__nickname = *msg.__parameters.begin();
    if (msg.__client->setClient())
        welcome(*msg.__client);
}

void Server::re_nick(Message &msg)
{
    if (msg.__parameters.size() < 1)
        return ERR_NEEDMOREPARAMS(NICK);
    if (msg.__client->__nickname == *msg.params.begin())
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
        welcome(*msg.__client);
}