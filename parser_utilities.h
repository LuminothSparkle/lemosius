#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include"lexer.h"

#include<array>
#include<utility>
#include<algorithm>
#include <unordered_map>
#include <unordered_set>

bool is_access(const token& t) {
   return t == PUBLIC_K || t == PRIVATE_K;
}

bool is_position(const token& t) {
   return t == PREFIX_K || t == SUFFIX_K || t == INFIX_K;
}

bool is_associativity(const token& t) {
   return t == LEFT_K || t == RIGHT_K;
}

bool is_literal(const token& t) {
   return t == IDENTIFIER_L || t == NUMBER_L;
}

auto match_any(const std::initializer_list<token_type>& il) {
   return [&il](token_type t) {
      return std::find( il.begin( ), il.end( ), t ) != il.end( );
   };
}

template<typename P>
token* optional_match(token*& t, P pred) {
    return (pred(*t) ? t++ : nullptr);
}

token* optional_match(token*& t, token_type type) {
    return optional_match( t, match_any({ type }) );
}

// Template de match con predicado
template<typename P>
token* match(token*& t, P pred, const std::string& mes = "") {
    if(!pred(*t)) {
        throw std::make_pair(*t, "Syntax Error: " + mes);
    }
    return t++;
}

token* match(token*& t, token_type type, const std::string& mes = "") {
    return match( t, match_any({ type }), mes );
}

struct operator_map {
    std::unordered_set<std::string_view> suffix_operators, prefix_operators;
    std::unordered_map<std::string_view, std::pair<std::int64_t,bool>> infix_operators;

    template<typename InputContainer>
    explicit operator_map(const InputContainer& ops) {
        infix_operators.insert( { ":=", { -1, false } }); //No estoy seguro de si agregarlo pero me parece la solución más facil considerando que aceptemos cosas como (c := (a := b))
                                                          //Si no es más facil hace un statement especial, pero además si acepta como solución no se si sea mejor ponerle precedencia cero
                                                          //Y dejar que los demás solo pongan precedencias de 1 en adelante o ponerle -1 y aceptar de 0 en adelante.
        for(const auto& op : ops) {
            if(op.position == INFIX_K) {
                infix_operators.insert( { op.symbol.source,{ get_representation<std::uint32_t>(*op.precedence), *op.associativity == LEFT_K } });
            }
            else if(op.position == SUFFIX_K) {
                suffix_operators.insert(op.symbol.source);
            }
            else if(op.position == PREFIX_K) {
                prefix_operators.insert(op.symbol.source);
            }
        }
    }
};

bool is_prefix_operator(const operator_map& opm, const token& t) {
    return opm.prefix_operators.contains(t.str());
}

bool is_suffix_operator(const operator_map& opm, const token& t) {
    return opm.suffix_operators.contains(t.str());
}

bool is_binary_operator(const operator_map& opm, const token& t) {
    return opm.infix_operators.contains(t.str());
}

bool is_left_assoc(const operator_map& opm, const token& t) {
    return opm.infix_operators.at(t.str()).second;
}

std::int64_t precedence(const operator_map& opm, const token& t) {
    return opm.infix_operators.at(t.str()).first;
}

#endif
