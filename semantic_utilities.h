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

std::string unquoted_str(const token& t) {
    auto throw_error = [&t](const auto& it) {
        using namespace std::string_literals;
        throw std::make_pair( token{t.type,{it,t.end()}}, "Semantic Error: String literal is ill-formed"s );
    };
    auto it = t.begin();
    auto next = std::find(it,t.end( ),'\\');
    std::string res(it,next);
    it = next;
    while(it != t.end( )) {
        it = std::next(next);
        constexpr std::string_view escaped = R"(abtnvfre0"'\)";
        constexpr std::string_view replacement = "\a\b\t\n\v\f\r\x1B\0\"\'\\";
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
                throw_error(it);
            }
            while(is_hex && push_value(res,it));
        }
        else {
            throw_error(it);
        }
        next = std::find(it,t.end( ),'\\');
        res.append(it,next);
        it = next;
    }
    return res.substr(1,res.size( ) - 2);
}

#endif
