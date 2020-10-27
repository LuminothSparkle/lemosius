#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include "lexer.h"
#include "parser_utilities.h"
#include"semantic_utilities.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct operator_map {
    std::unordered_set<std::string_view> suffix_operators, prefix_operators;
    std::unordered_map<std::string_view,std::pair<std::uint64_t,bool>> infix_operators;

    template<typename Container>
    operator_map(const Container& ops) {
        infix_operators.insert( { ":=", { -1, true } }); //No estoy seguro de si agregarlo pero me parece la solución más facil considerando que aceptemos cosas como (c := (a := b))
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

    bool is_binary(const std::string_view& sv) const {
        return infix_operators.contains(sv);
    }
    bool is_suffix(const std::string_view& sv) const {
        return suffix_operators.contains(sv);
    }
    bool is_prefix(const std::string_view& sv) const {
        return prefix_operators.contains(sv);
    }
    bool is_left(const std::string_view& sv) const {
        return infix_operators.at(sv).second;
    }
    std::uint64_t precedence(const std::string_view& sv) const {
        return infix_operators.at(sv).first;
    }
};

auto is_prefix_operator(const auto& opm) {
    return [&opm](const token& t) {
        return opm.is_prefix(t.str());
    };
}

auto is_suffix_operator(const auto& opm) {
    return [&opm](const token& t) {
        return opm.is_suffix(t.str());
    };
}

auto is_binary_operator(const auto& opm) {
    return [&opm](const token& t) {
        return opm.is_binary(t.str());
    };
}

// Terminan funciones de prueba para la gramatica


struct expression {
    virtual std::string str() const = 0;
};

struct terminal_expression : expression {
    token t;  // literal o identificador
    std::string str() const {
        return t.str();
    }
};

struct prefix_expression : expression {
    token op;
    std::unique_ptr<expression> ex;
    std::string str() const {
        return op.str() + ex->str();
    }
};

struct suffix_expression : expression {
    std::unique_ptr<expression> ex;
    token op;
    std::string str() const {
        return ex->str() + op.str();
    }
};

struct binary_expression : expression {
    std::unique_ptr<expression> ex1;
    token op;
    std::unique_ptr<expression> ex2;
    std::string str() const {
        return ex1->str() + " " + op.str() + " " + ex2->str();
    }
};

struct call_expression : expression {
    token function_name;       // como sólo tenemos números como tipos, la llamada a función debe ser f(params) con f identificador
    std::vector<std::unique_ptr<expression>> params;                 // para simplificar la vida, no hay que permitir (f)(params)
    std::string str() const {
        return function_name.str() + transform_join(params,
        [](const auto& p){
            return to_string(p);
        },",","(",")");
    }
};

std::unique_ptr<expression> parse_expression(token*& t);

std::unique_ptr<expression> parse_primary_expression(token*& t, const auto& opm) {      // un terminal_expression, una expresión envuelta en ( ) o una llamada a función
    if (*t == LPARENTHESIS_P) {
        match(t, LPARENTHESIS_P);
        auto res = parse_expression(t,opm);
        match(t, RPARENTHESIS_P);
        return res;
    } else if (*t == IDENTIFIER_L && *(t + 1) == LPARENTHESIS_P) {
        auto res = std::make_unique<call_expression>( );
        res->function_name = *match(t, IDENTIFIER_L);
        match(t, LPARENTHESIS_P);
        while(*t != RPARENTHESIS_P) {
            res->params.push_back(parse_expression(t,opm));
            if(*t != RPARENTHESIS_P) {
                match(t, COMMA_P);
            }
        }
        match(t, RPARENTHESIS_P);
        return res;
    } else {
        auto res = std::make_unique<terminal_expression>( );
        res->t = *match(t, match_any({ NUMBER_L, IDENTIFIER_L }));
        return res;
    }
}

std::unique_ptr<expression> parse_unary_expression(token*& t, const auto& opm) {
   if(opm.is_prefix(*t)) {          // ¿cómo?
      auto res = std::make_unique<prefix_expression>( );
      res->op = *match(t, is_prefix_operator(opm));
      res->ex = parse_unary_expression(t,opm);
      return res;
   }
   auto res = parse_primary_expression(t,opm);
   while(opm.is_suffix(*t)) {       // ¿cómo?
      auto temp = std::make_unique<suffix_expression>( );
      temp->ex = std::move(res);
      temp->op = *match(t, is_suffix_operator(opm));
      res = std::move(temp);
   }
   return res;
}

std::unique_ptr<expression> parse_binary_expression(token*& t, std::size_t min_precedence, const auto& opm) {
   auto res = parse_unary_expression(t,opm);
   while(opm.is_binary(*t) && opm.precedence(*t) >= min_precedence) {    // ¿cómo?
        auto next_prec = opm.precedence(*t) + opm.is_left(*t);
        auto temp = std::make_unique<binary_expression>( );
        temp->ex1 = std::move(res);
        temp->op = *match(t, is_binary_operator(opm));
        temp->ex2 = parse_binary_expression(t, next_prec, opm);
        res = std::move(temp);
   }
   return res;
}

std::unique_ptr<expression> parse_expression(token*& t, const operator_map& opm) {
    return parse_binary_expression(t, 0, opm);        // suponiendo que 0 fuera la precedencia más pequeña que se acepte; ¿qué valores aceptaremos como precedencias?
}

#endif
