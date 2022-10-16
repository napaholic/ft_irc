//
// Created by jaewkim on 2022/09/23.
/*
1. 데이터 받고 보내고.
2. 채널클래스, 유저클래스 소유

fd 돌아가면서 체크 및 뉴 커넥션 유저 받는것 역시 서버가한다.

서버는 커맨드만 분리하고 미리 지정된 함수포인터로 나머지 버퍼를 넘긴다.
 */
//

#ifndef JAY_IRC_SERVER_H
#define JAY_IRC_SERVER_H

#include "./main.h"
#include "./util.hpp"

#define CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember))
#define SPECIAL "[]\\`_^{|}-"

class Session;
class Channel;
class Message;

class Server
{
  private:
    const std::string __port;
    const std::string __password;
    int __port_int;
    std::set<Channel *> __channels;
    std::map<unsigned long, void (Server::*)(Client &client)> __cmd_list;
    std::set<Client *> __clients;

  public:
    // Member functions
    Server(const std::string &port, const std::string &password);
    ~Server();

    // Server commands used by client
    void pass(Client &client);
    bool isErrorNick(std::string nick);
    void newNickname(Client &client);
    void changeNickname(Client &client);
    void nick(Client &client);
    void user(Client &client);
    void quit(Client &client);
    void join(Client &client);
    void topic(Client &client);
    void invite(Client &client);
    void privmsg(Client &client);
    void notice(Client &client);
    void mode(Client &client);
    void part(Client &client);
    void kick(Client &client);
    void list(Client &client);
	void names(Client &client);
    std::vector<std::string> splitPrivmsgTarget(std::string str, char Delimiter);


    // Element access
    Client *findClient(std::string nick);
    Client *findClient(int fd);
    Channel *findChannel(std::string channel);

    // Modifiers
    void createChannel(const std::string &name, Client *client);
	void modeChannel(std::string target, Client &client, std::vector<std::string>::iterator param);


    // Unknown functions
    void run(Session &session);
    void accept_client(Session &session);
    void receive_message(Session &session, int fd);
    void disconnect_client(Session &session, int fd);
    void send_message(int fd, const char buf[]);
    void send_message(int fd, std::string str);
    void send_message(Channel *channel, std::string text);
    void broad_cast(Session &session, char *buf, int fd);
};

#endif // JAY_IRC_SERVER_H
