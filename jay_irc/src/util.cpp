//
// Created by jaewkim on 2022/09/29.
//
#include "../inc/util.hpp"

unsigned long djb2(std::string str)
{
    unsigned long hash  = 5381;
    unsigned int size   = str.length();
    unsigned int i      = 0;
    for (int i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + (str[i]);
    }
    return hash;
}