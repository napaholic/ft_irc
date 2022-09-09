cmake_minimum_required(VERSION 3.23)
project(ft_irc)

set(CMAKE_CXX_STANDARD 14)

include_directories(ref_git/IRC_server/inc)
include_directories(ref_git/simple-irc-server)

add_executable(ft_irc
        ref_git/IRC_server/inc/Channel.hpp
        ref_git/IRC_server/inc/Client.hpp
        ref_git/IRC_server/inc/ft_irc.hpp
        ref_git/IRC_server/inc/Message.hpp
        ref_git/IRC_server/inc/reply.hpp
        ref_git/IRC_server/inc/Server.hpp
        ref_git/IRC_server/src/Channel.cpp
        ref_git/IRC_server/src/Client.cpp
        ref_git/IRC_server/src/commands.cpp
        ref_git/IRC_server/src/libft.cpp
        ref_git/IRC_server/src/main.cpp
        ref_git/IRC_server/src/Message.cpp
        ref_git/IRC_server/src/Server.cpp
        ref_git/IRC_server/src/ssl.cpp
        ref_git/IRC_server/src/utils.cpp
        ref_git/simple-irc-server/channel.cpp
        ref_git/simple-irc-server/channel.h
        ref_git/simple-irc-server/config.cpp
        ref_git/simple-irc-server/config.h
        ref_git/simple-irc-server/defines.h
        ref_git/simple-irc-server/main.cpp
        ref_git/simple-irc-server/mainframe.cpp
        ref_git/simple-irc-server/mainframe.h
        ref_git/simple-irc-server/parser.cpp
        ref_git/simple-irc-server/parser.h
        ref_git/simple-irc-server/server.cpp
        ref_git/simple-irc-server/server.h
        ref_git/simple-irc-server/session.cpp
        ref_git/simple-irc-server/session.h
        ref_git/simple-irc-server/user.cpp
        ref_git/simple-irc-server/user.h
        webserv/client.cpp
        webserv/server.cpp
        select.cpp
        test.cpp)
