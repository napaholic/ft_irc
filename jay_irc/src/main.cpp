//
// Created by Jaewook Kim on 9/9/22.
//

#include "../inc/main.h"

int main(int argc, char **argv)
{
    try
    {
        if (argc != 3)
            throw std::invalid_argument("please ./ircserv port password");
        signal(SIGPIPE, SIG_IGN);
        Session session(argv[1], argv[2]);
        Server server(argv[1], argv[2]);
        while (1)
        {
            session.select_socket();
            server.run(session);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
