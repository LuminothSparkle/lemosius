#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H
#include<vector>
#include<utility>
#include"token.h"

template<typename P>
token* match(token*& t, P pred) {
    if(!pred(*t)) {
        throw std::make_pair(*t,"Syntax Error");
    }
    return t++;
}

token* match(token*& t, token_type type) {
    return match(t, [&](token_type t) {
      return t == type;
    });
}

bool is_visibility(token_type type) {
   return type == PUBLIC_K || type == PRIVATE_K;
}

bool is_position(token_type type) {
   return type == PREFIX_K || type == SUFFIX_K || type == INFIX_K;
}

bool is_precedence(token_type type) {
   return type == LEFT_K || type == RIGHT_K;
}

// las que convengan, aunque posibilemente para este lenguaje tan chico no haya más

#endif
