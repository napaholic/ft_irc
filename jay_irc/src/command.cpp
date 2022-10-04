//
// Created by Yunhui Bae on 2022/10/03.
//

#include "../inc/server.hpp"

void Server::pass(Message &msg)
{//   서버 접속 시 패스워드와 같은지 확인해주는 명령어
//    if (getClient(msg.socket)->type != "unknown")
//        return; //type가 무슨 의미인지 모르겠다.
    if (msg.__parameters.size() > 0)//parameters getter 필요할듯 혹은 friend 사용
    {
        if (*msg.params.begin() == __password)
        {
            print("Password accepted");
            send_message(msg.orig->socket, "NOTICE :Password accepted.");
            //getClient(msg.socket)->type = "accepted";
        }
        else
            send_message(msg.socket, "NOTICE :Incorrect password.");
    }
}

void Server::nick(Message &msg)
{//msg.orig는 클라이언트의 주소를 의미. 우리 계정은 fd로 관리하나?
    if (msg.orig->type == "accepted")
        new_nick(msg);
    else if (msg.orig->type == "client")
        re_nick(msg);
    else if (msg.orig->type == "server")
        new_nick(msg);
    else if (msg.orig->type == "preserver")
    {
        new_nick(msg);
        msg.orig->type = "server";
        sendmsg(msg.orig->socket, std::string("NICK ").append(port));
        sendusers(msg);
        sendchannels(msg);
    }
}