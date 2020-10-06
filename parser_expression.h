#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include "lexer.h"
#include "parser_utilities.h"

#include <memory>
#include <vector>

// Funciones de prueba para la gramatica

bool is_prefix_operator(const token& t) {
    return false;
}

bool is_suffix_operator(const token& t) {
    return false;
}

bool is_binary_operator(const token& t) {
    return t == OPERATOR_L || t == ASSIGNMENT_O;
}

std::size_t precedence(const token& t) {
    return 0;
}

token_type associativity(const token& t) {
    return t;
}

// Terminan funciones de prueba para la gramatica


struct expression {
    virtual ~expression( ) = 0;
};

expression::~expression( ) = default;

struct terminal_expression : expression {
   token t;  // literal o identificador
};

struct prefix_expression : expression {
   token op;
   std::unique_ptr<expression> ex;
};

struct suffix_expression : expression {
   std::unique_ptr<expression> ex;
   token op;
};

struct binary_expression : expression {
   std::unique_ptr<expression> ex1;
   token op;
   std::unique_ptr<expression> ex2;
};

struct call_expression : expression {
   token function_name;       // como sólo tenemos números como tipos, la llamada a función debe ser f(params) con f identificador
   std::vector<std::unique_ptr<expression>> params;                 // para simplificar la vida, no hay que permitir (f)(params)
};

std::unique_ptr<expression> parse_expression(token*& t);

std::unique_ptr<expression> parse_primary_expression(token*& t) {      // un terminal_expression, una expresión envuelta en ( ) o una llamada a función
   if (*t == LPARENTHESIS_P) {
      auto res = parse_expression(t);
      match(t, RPARENTHESIS_P);
      return res;
   } else if (*t == IDENTIFIER_L && *(t + 1) == LPARENTHESIS_P) {
      auto res = std::make_unique<call_expression>( );
      res->function_name = *match(t, IDENTIFIER_L);
      match(t, LPARENTHESIS_P);
      while(*t != RPARENTHESIS_P) {
        res->params.push_back(parse_expression(t));
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

std::unique_ptr<expression> parse_unary_expression(token*& t) {
   if (is_prefix_operator(*t)) {          // ¿cómo?
      auto res = std::make_unique<prefix_expression>( );
      res->op = *match(t, is_prefix_operator);
      res->ex = parse_expression(t);
      return res;
   }

   auto res = parse_primary_expression(t);
   while (is_suffix_operator(*t)) {       // ¿cómo?
      auto temp = std::make_unique<suffix_expression>( );
      temp->ex = std::move(res);
      temp->op = *match(t, is_suffix_operator);
      res = std::move(temp);
   }
   return res;
}

std::unique_ptr<expression> parse_binary_expression(token*& t, std::size_t min_precedence) {
   auto res = parse_unary_expression(t);
   while (is_binary_operator(*t) && precedence(*t) >= min_precedence) {    // ¿cómo?
      auto prec = precedence(*t); auto assoc = associativity(*t);    // ¿cómo?
      auto temp = std::make_unique<binary_expression>( );
      temp->ex1 = std::move(res);
      temp->op = *match(t, is_binary_operator);
      temp->ex2 = parse_binary_expression(t, prec + (assoc == LEFT_K));
      res = std::move(temp);
   }
   return res;
}

std::unique_ptr<expression> parse_expression(token*& t) {
   return parse_binary_expression(t, 0);        // suponiendo que 0 fuera la precedencia más pequeña que se acepte; ¿qué valores aceptaremos como precedencias?
}

#endif
