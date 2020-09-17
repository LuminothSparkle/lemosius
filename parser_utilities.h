#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H
#include<vector>
#include<utility>
#include"token.h"

using it_tok = std::vector<token>::iterator;

void match(it_tok& t, token_type type) {
    if(*t != type) {
        throw std::make_pair(*t,"Syntax Error");
    }
    t++;
}

#endif
