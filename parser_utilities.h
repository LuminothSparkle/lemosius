#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include<vector>
#include<utility>

#include"token.h"

template<typename P>
token* match(token*& t, P pred, const char* mes = "Syntax Error") {
    if(!pred(*t)) {
        throw std::make_pair(*t,mes);
    }
    return t++;
}

token* match(token*& t, token_type type, const char* mes = "Syntax Error") {
    return match(t, [&](token_type t) {
      return t == type;
    }, mes);
}

bool is_visibility(token_type type) {
   return type == PUBLIC_K || type == PRIVATE_K;
}

bool is_position(token_type type) {
   return type == PREFIX_K || type == SUFFIX_K || type == INFIX_K;
}

bool is_asociativity(token_type type) {
   return type == LEFT_K || type == RIGHT_K;
}

bool is_literal(token_type type) {
   return type == IDENTIFIER_L || type == NUMBER_L;
}

//Aun por modificar y solo hecho para probar la gramatica.
bool is_operator(token& t) {
    return t == OPERATOR_L || t == ASSIGNMENT_O;
}

#endif
