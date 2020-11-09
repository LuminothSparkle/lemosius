#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include "parser_types.h"
#include "parser_utilities.h"

#include <memory>

std::unique_ptr<expression> parse_expression( const token*&, const operator_map& );

std::unique_ptr<expression> parse_primary_expression( const token*& tok_ptr, const operator_map& opm ) {    // un terminal_expression, una expresión envuelta en ( ) o una llamada a función
   if( *tok_ptr == LPARENTHESIS_P ) {
      match( tok_ptr, LPARENTHESIS_P, "Expecting (" );
      auto res = parse_expression( tok_ptr, opm );
      match( tok_ptr, RPARENTHESIS_P, "Expecting )" );
      return res;
   } else if( *tok_ptr == IDENTIFIER_L && *( tok_ptr + 1 ) == LPARENTHESIS_P ) {
      auto res = std::make_unique<call_expression>( );
      res->function_name = match( tok_ptr, IDENTIFIER_L, "Expecting an function identifier" );
      match( tok_ptr, LPARENTHESIS_P, "Expecting (" );
      while( *tok_ptr != RPARENTHESIS_P ) {
         res->params.push_back( parse_expression( tok_ptr, opm ) );
         if( *tok_ptr != RPARENTHESIS_P ) {
            match( tok_ptr, COMMA_P, "Expecting ," );
         }
      }
      match( tok_ptr, RPARENTHESIS_P, "Expecting )" );
      return res;
   } else {
      auto res = std::make_unique<terminal_expression>( );
      res->tok_ptr = match( tok_ptr, match_any( { NUMBER_L, IDENTIFIER_L } ), "Expecting an identifier or number literal" );
      return res;
   }
}

std::unique_ptr<expression> parse_unary_expression( const token*& tok_ptr, const operator_map& opm ) {
   if( is_prefix_operator( opm, *tok_ptr ) ) {     // ¿cómo?
      auto res = std::make_unique<prefix_expression>( );
      res->op  = match( tok_ptr, std::bind_front( is_prefix_operator, opm ), "Expecting an prefix operator" );
      res->exp = parse_unary_expression( tok_ptr, opm );
      return res;
   }
   auto res = parse_primary_expression( tok_ptr, opm );
   while( is_suffix_operator( opm, *tok_ptr ) ) {  // ¿cómo?
      auto temp = std::make_unique<suffix_expression>( );
      temp->exp = std::move( res );
      temp->op  = match( tok_ptr, std::bind_front( is_suffix_operator, opm ), "Expecting an suffix operator" );
      res = std::move( temp );
   }
   return res;
}

std::unique_ptr<expression> parse_binary_expression( const token*& tok_ptr, std::int64_t min_precedence, const operator_map& opm ) {
   auto res = parse_unary_expression( tok_ptr, opm );
   while( is_binary_operator( opm, *tok_ptr ) && precedence( opm, *tok_ptr ) >= min_precedence ) { // ¿cómo?
      auto temp      = std::make_unique<binary_expression>( );
      auto next_prec = precedence( opm, *tok_ptr ) + is_left_assoc( opm, *tok_ptr );
      temp->exp1     = std::move( res );
      temp->op       =  match( tok_ptr, std::bind_front( is_binary_operator, opm ), "Expecting an infix operator" );
      temp->exp2     = parse_binary_expression( tok_ptr, next_prec, opm );
      res = std::move( temp );
   }
   return res;
}

std::unique_ptr<expression> parse_expression( const token*& tok_ptr, const operator_map& opm ) {
   return parse_binary_expression( tok_ptr, -1, opm );      // suponiendo que 0 fuera la precedencia más pequeña que se acepte; ¿qué valores aceptaremos como precedencias?
}

#endif
