#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include"lexer.h"

#include<array>
#include<utility>
#include<algorithm>

bool is_access(token_type type) {
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

template<typename P>
token* optional_match(token*& t, P pred) {
    return (pred(*t) ? t++ : nullptr);
}

template<std::size_t N>
token* optional_match(token*& t, const std::array<token_type,N>& types) {
    return optional_match(t,[&types](token_type t) {
        return std::find(types.begin(), types.end(), t) != types.end();
    });
} // end function optional_match

token* optional_match(token*& t, token_type type) {
    return optional_match(t, std::array<token_type,1>({type}));
}

// Template de match con predicado 
template<typename P>
token* match(token*& t, P pred, const std::string& mes = "") {
    if(!pred(*t)) {
        throw std::make_pair(*t, "Syntax Error: " + mes);
    }
    return t++;
} // end function match

template<std::size_t N>
token* match(token*& t, const std::array<token_type,N>& types, const std::string& mes = "") {
    return match(t,[&types](token_type t) {
        return std::find(types.begin(), types.end(), t) != types.end();
    }, mes);
} // end function match

token* match(token*& t, token_type type, const std::string& mes = "") {
    return match(t, std::array<token_type,1>({type}), mes);
} // end function match



#endif
