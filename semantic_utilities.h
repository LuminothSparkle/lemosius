#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include"lexer.h"

#include<string>
#include<string_view>
#include<utility>
#include<charconv>

template<typename T>
bool is_public(const T& t) {
    return t.access == nullptr || *t.access == PUBLIC_K;
};

bool isoctal(char c) {
    return isdigit(c) && c < '8';
}

template<typename T>
std::pair<T, bool> get_representation(std::string_view s) {
    /*      :( GCC aún no tiene from_chars para floating point
    long double res;
    auto [end, error] = std::from_chars(s.data( ), s.data( ) + s.size( ), res);
    return { T(res), (!error && p == s.end( ) && T(res) == res) }; */

    try {      // horrible, pero más fácil de escribir que el que usa std::strold, en particular porque strold requiere una cadena con terminador
       long double res = std::stold(std::string(s));      // nosotros sabemos que nuestros string_views vienen de tokens válidos y que acaban en algo que ya no es número,
       return { T(res), T(res) == res };                  // pero prefiero no andarle jugando al vivo
    } catch (...) {
       return { T( ), false };
    }
}

std::string unquoted_str(const token& t) {
    auto it = t.begin();
    auto next = std::find(it,t.end( ),'\\');
    std::string res(it,next);
    it = next;
    while(it != t.end( )) {
        it = std::next(next);
        constexpr std::string_view escaped = R"(abtnvfre0"'\)";
        constexpr std::string_view replacement = "\a\b\t\n\v\f\r\x1B\0\"\'\\";
        using namespace std::string_literals;
        if(std::size_t idx = escaped.find(*it); idx < escaped.size( )) {
            res.push_back(replacement[idx]);
            std::advance(it,1);
        }
        else if(bool is_hex = *it == 'x'; is_hex || isoctal(*it) ) {
            std::size_t base = (is_hex ? 16 : 8);
            std::advance(it,is_hex);
            auto push_value = [&base,&t](auto& res, auto& it) {
                unsigned char val = 0;
                auto next = std::from_chars(it,t.end(),val,base).ptr;
                res.push_back(val);
                std::swap(next,it);
                return next != it;
            };
            if(!push_value(res,it)) {
                throw std::make_pair( token{t.type,{it,t.end()}}, "Semantic Error: Codepoint in string literal is ill-formed"s );
            }
            while(is_hex && push_value(res,it));
        }
        else {
            throw std::make_pair( token{t.type,{it,t.end()}}, "Semantic Error: Unknown escape sequence in string literal"s );
        }
        next = std::find(it,t.end( ),'\\');
        res.append(it,next);
        it = next;
    }
    return res.substr(1,res.size( ) - 2);
}

#endif
