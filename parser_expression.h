#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include "lexer_types.h"
#include "expression_types.h"

#include "parser_utilities.h"

#include <memory>

std::unique_ptr<expression> parse_expression( const token*& t, const operator_map& opm );

std::unique_ptr<expression> parse_primary_expression( const token*& t, const operator_map& opm ) {    // un terminal_expression, una expresión envuelta en ( ) o una llamada a función
   if( *t == LPARENTHESIS_P ) {
      match( t, LPARENTHESIS_P );
      auto res = parse_expression( t, opm );
      match( t, RPARENTHESIS_P );
      return res;
   } else if( *t == IDENTIFIER_L && *( t + 1 ) == LPARENTHESIS_P ) {
      auto res = std::make_unique<call_expression>( );
      res->function_name = match( t, IDENTIFIER_L );
      match( t, LPARENTHESIS_P );
      while( *t != RPARENTHESIS_P ) {
         res->params.push_back( parse_expression( t, opm ) );
         if( *t != RPARENTHESIS_P ) {
            match( t, COMMA_P );
         }
      }
      match( t, RPARENTHESIS_P );
      return res;
   } else {
      auto res = std::make_unique<terminal_expression>( );
      res->t = match( t, match_any( { NUMBER_L, IDENTIFIER_L } ) );
      return res;
   }
}

std::unique_ptr<expression> parse_unary_expression( const token*& t, const operator_map& opm ) {
   if( is_prefix_operator( opm, *t ) ) {     // ¿cómo?
      auto res = std::make_unique<prefix_expression>( );
      res->op = match( t, std::bind_front( is_prefix_operator, opm ) );
      res->ex = parse_unary_expression( t, opm );
      return res;
   }
   auto res = parse_primary_expression( t, opm );
   while( is_suffix_operator( opm, *t ) ) {  // ¿cómo?
      auto temp = std::make_unique<suffix_expression>( );
      temp->ex = std::move( res );
      temp->op = match( t, std::bind_front( is_suffix_operator, opm ) );
      res = std::move( temp );
   }
   return res;
}

std::unique_ptr<expression> parse_binary_expression( const token*& t, std::int64_t min_precedence, const operator_map& opm ) {
   auto res = parse_unary_expression( t, opm );
   while( is_binary_operator( opm, *t ) && precedence( opm, *t ) >= min_precedence ) { // ¿cómo?
      auto next_prec = precedence( opm, *t ) + is_left_assoc( opm, *t );
      auto temp = std::make_unique<binary_expression>( );
      temp->ex1 = std::move( res );
      temp->op =  match( t, std::bind_front( is_binary_operator, opm ) );
      temp->ex2 = parse_binary_expression( t, next_prec, opm );
      res = std::move( temp );
   }
   return res;
}

std::unique_ptr<expression> parse_expression( const token*& t, const operator_map& opm ) {
   return parse_binary_expression( t, -1, opm );      // suponiendo que 0 fuera la precedencia más pequeña que se acepte; ¿qué valores aceptaremos como precedencias?
}

#endif
