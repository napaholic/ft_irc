//
// Created by Jaewook Kim on 9/9/22.
//

#ifndef FT_IRC_MAIN_H
#define FT_IRC_MAIN_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <utility>

#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <sstream>


#include "./session.hpp"
#include "./channel.hpp"
#include "./client.hpp"
#include "./message.hpp"
#include "./server.hpp"
#include "./util.hpp"
#include "./responses.hpp"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

#endif //FT_IRC_MAIN_H
