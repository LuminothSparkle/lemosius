#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include"lexer.h"

#include<string>
#include<string_view>
#include<utility>
#include<charconv>
#include<cstdlib>

template<typename T>
bool is_public(const T& t) {
    return t.access == nullptr || *t.access == PUBLIC_K;
};

bool isoctal(char c) {
    return isdigit(c) && c < '8';
}

template<typename T>
std::pair<T, bool> get_representation(const std::string_view& s) {
    /*      :( GCC a�n no tiene from_chars para floating point
    long double res;
    auto [end, error] = std::from_chars(s.data( ), s.data( ) + s.size( ), res);
    return { T(res), (!error && p == s.end( ) && T(res) == res) }; */
    std::string buffer(s);
    auto p = const_cast<char*>(buffer.c_str());
    long double res = std::strtold(p,&p);
    if(p != buffer.c_str() + buffer.size()) {
        return { T( ), false };
    }
    return { T(res), T(res) == res };
    /*
    try {      // horrible, pero m�s f�cil de escribir que el que usa std::strold, en particular porque strold requiere una cadena con terminador
       long double res = std::stold(std::string(s));      // nosotros sabemos que nuestros string_views vienen de tokens v�lidos y que acaban en algo que ya no es n�mero,
       return { T(res), T(res) == res };                  // pero prefiero no andarle jugando al vivo
    } catch (...) {
       return { T( ), false };
    }
    */
}

template<typename T>
T get_representation(const token& t, const std::string& error_mes = "") {
    auto [val, success] = get_representation<T>(t.source);
    if(!success) {
        throw std::make_pair( t, "Semantic Error:" + error_mes );
    }
    return val;
}

std::pair<std::string, std::string_view::iterator> unquoted_str(const std::string_view& sv) {
    auto it = sv.begin();
    if(sv.empty() || sv.front() != '"') { //Mejor para no tener problemas, ya que igual se checa en el lexer pero tambien aqui por si las dudas.
        return { "", sv.begin() };
    }
    std::string res;
    for(std::advance(it,1); it != sv.end() && *it != '"'; std::advance(it,1)) {
        if(*it == '\\') {
            std::advance(it,1);
            constexpr std::string_view escaped = R"(abtnvfre0"'\)";
            constexpr std::string_view replacement = "\a\b\t\n\v\f\r\x1B\0\"\'\\";
            if(std::size_t idx = escaped.find(*it); idx < escaped.size( )) {
                res.push_back(replacement[idx]);
                std::advance(it,1);
            }
            else if(bool is_hex = *it == 'x'; is_hex || isoctal(*it) ) {
                std::size_t base = (is_hex ? 16 : 8);
                std::advance(it,is_hex);
                auto obtain_char = [&base,&sv](auto& it,unsigned char& c) {
                    unsigned char val = 0;
                    auto first = it;
                    it = std::from_chars(first,sv.end(),val,base).ptr;
                    return first != it;
                };
                unsigned char c;
                if(!obtain_char(it,c)) {
                    return { res, it };
                }
                res.push_back(c);
                while(is_hex && obtain_char(it,c)) {
                    res.push_back(c);
                }
            }
            else {
                return { res, it };
            }
        }
        else {
            res.push_back(*it);
        }
    }
    return { res, std::next(it,*it == '"') };
}

std::string unquoted_str(const token& t, const std::string& error_mes = "") {
    using namespace std::string_literals;
    auto [str, it] = unquoted_str(t.source);
    if(it != t.end()) {
        throw std::make_pair( token{ t.type, { it,t.end() } }, "Semantic Error:" + error_mes );
    }
    return str;
}



#endif
