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
            //getClient(msg.socket)->type = "accepted";
        }
        else
            send_message(msg.__fd, "NOTICE :Incorrect password.");
        //client 소켓이랑 그냥 소켓의 차이점을 모르겠음. 서버 얘기인가?
    }
}

void Server::new_nick(Message &msg)
{
    if (msg.__parameters.size() < 1)
        return ERR_NEEDMOREPARAMS(NICK);
}

void Server::nick(Message &msg)
{//msg.client는 클라이언트의 주소를 의미. 우리 계정은 fd로 관리하나?
    if (msg.__client->__nickname == NULL)
        new_nick(msg);
    else
        re_nick(msg);
}