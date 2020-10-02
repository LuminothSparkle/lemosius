#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include"lexer.h"

#include<array>
#include<utility>
#include<algorithm>

bool is_access(const token& t) {
   return t == PUBLIC_K || t == PRIVATE_K;
}

bool is_position(const token& t) {
   return t == PREFIX_K || t == SUFFIX_K || t == INFIX_K;
}

bool is_asociativity(const token& t) {
   return t == LEFT_K || t == RIGHT_K;
}

bool is_literal(const token& t) {
   return t == IDENTIFIER_L || t == NUMBER_L;
}

//Aun por modificar y solo hecho para probar la gramatica.
bool is_operator(const token& t) {
    return t == OPERATOR_L || t == ASSIGNMENT_O;
}

auto match_any(std::initializer_list<token_type> il) {
   return [&il](token_type t) {
      return std::find(il.begin(), il.end(), t) != il.end();
   };
}

template<typename P>
token* optional_match(token*& t, P pred) {
    return (pred(*t) ? t++ : nullptr);
}

token* optional_match(token*& t, token_type type) {
    return optional_match(t, match_any({ type }));
}

// Template de match con predicado
template<typename P>
token* match(token*& t, P pred, const std::string& mes = "") {
    if(!pred(*t)) {
        throw std::make_pair(*t, "Syntax Error: " + mes);
    }
    return t++;
} // end function match

token* match(token*& t, token_type type, const std::string& mes = "") {
    return match(t, match_any({ type }), mes);
} // end function match

std::string to_string(token* t, const std::string& prefix = "" , const std::string& suffix = "") {
    return (t != nullptr ? prefix + t->str( ) + suffix : "");
}

#endif
