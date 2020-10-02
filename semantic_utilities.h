#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include"lexer.h"

#include<string>
#include<string_view>
#include<utility>
#include<limits>

template<typename T>
bool is_public(const T& t) {
    return t.access == nullptr || *t.access == PUBLIC_K;
};

bool isoctal(char c) {
    return isdigit(c) && c < '8';
}

std::string unquoted_str(const token& t) {
    auto throw_error = [&t]() {
        using namespace std::string_literals;
        throw std::make_pair( t, "Semantic Error: String literal is ill-formed"s);
    };
    auto first = t.source.begin( ), last = std::find(first,t.source.end( ),'\\');
    std::string res(first,last - first);
    while( last != t.source.end( ) ) {
        std::advance(last,1);
        constexpr std::string_view escaped = R"(abtnvfre0"'\)";
        constexpr std::string_view replacement = "\a\b\t\n\v\f\r\x1B\0\"\'\\";
        std::size_t idx = std::find(escaped.begin( ),escaped.end( ),*last) - escaped.begin( );
        if(idx != escaped.size( )) {
            res.push_back(replacement[idx]);
            std::advance(last,1);
        }
        else if(*last == 'x' || isoctal(*last)) {
            bool is_hex           = *last == 'x';
            std::size_t max_chars = (is_hex ? 2 : 3);
            std::size_t base      = (is_hex ? 16 : 8);
            std::advance(last,is_hex);
            auto push_value = [&max_chars,&base,&res](const char*& val_begin) {
                try {
                    std::size_t next_pos = 0;
                    auto val = stoull(std::string(val_begin,max_chars),&next_pos,base);
                    res.push_back(val);
                    val_begin += next_pos;
                    return val <= std::numeric_limits<unsigned char>::max();
                }
                catch(const std::invalid_argument& e) {
                    return false;
                }
            };
            if(!push_value(last)) {
                throw_error();
            }
            while(is_hex && push_value(last));
        }
        else {
            throw_error();
        }

        first = last; last = std::find(first,t.source.end( ),'\\');
        res.append(first,last);
    }
    return res.substr(1,res.size() - 2);
}

#endif
