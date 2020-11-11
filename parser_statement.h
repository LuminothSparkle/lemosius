#ifndef PARSER_STATEMENT_H
#define PARSER_STATEMENT_H

#include "parser_types.h"
#include "parser_utilities.h"
#include "parser_expression.h"

#include <memory>

std::unique_ptr<statement> parse_statement( const token*&, const operator_map& );

auto parse_sequence_statement( const token*& tok_ptr, const operator_map& opm ) {
   auto seq_stmt = std::make_unique<sequence_statement>( );
   match( tok_ptr, LBRACE_P, "Expecting {" );
   while( *tok_ptr != RBRACE_P ) {
      seq_stmt->body.push_back( parse_statement( tok_ptr, opm ) );
   }
   match( tok_ptr, RBRACE_P, "Expecting }" );
   return seq_stmt;
}

auto parse_if_statement( const token*& tok_ptr, const operator_map& opm ) {
   auto if_stmt = std::make_unique<if_statement>( );
   do {
      optional_match( tok_ptr, ELSE_K );
      match( tok_ptr, IF_K, "Expecting if" );
      match( tok_ptr, LPARENTHESIS_P, "Expecting (" );
      if_stmt->conditions.push_back( parse_expression( tok_ptr, opm ) );
      match( tok_ptr, RPARENTHESIS_P, "Expecting )" );
      if_stmt->bodys.push_back( parse_sequence_statement( tok_ptr, opm ) );
   } while( *tok_ptr == ELSE_K && *( tok_ptr + 1 ) == IF_K );
   if( optional_match( tok_ptr, ELSE_K ) != nullptr ) {
      if_stmt->else_body = parse_sequence_statement( tok_ptr, opm );
   }
   return if_stmt;
}

auto parse_var_statement( const token*& tok_ptr, const operator_map& opm ) {
   auto var_stmt = std::make_unique<var_statement>( );
   match( tok_ptr, VAR_K, "Expecting var" );
   var_stmt->name = match( tok_ptr, IDENTIFIER_L, "Expecting an identifier" );
   if( optional_match( tok_ptr, ASSIGNMENT_O ) != nullptr ) {
      var_stmt->value = parse_expression( tok_ptr, opm );
   }
   match( tok_ptr, SEMICOLON_P, "Expecting ;" );
   return var_stmt;
}

auto parse_return_statement( const token*& tok_ptr, const operator_map& opm ) {
   auto ret_stmt = std::make_unique<return_statement>( );
   match( tok_ptr, RETURN_K, "Expecting return" );
   ret_stmt->return_value = parse_expression( tok_ptr, opm );
   match( tok_ptr, SEMICOLON_P, "Expecting ;" );
   return ret_stmt;
}

auto parse_expression_statement( const token*& tok_ptr, const operator_map& opm ) {
   auto exp_stmt = std::make_unique<expression_statement>( );
   if( *tok_ptr != SEMICOLON_P ) {
      exp_stmt->body = parse_expression( tok_ptr, opm );
   }
   match( tok_ptr, SEMICOLON_P, "Expecting ;" );
   return exp_stmt;
}

std::unique_ptr<statement> parse_statement( const token*& tok_ptr, const operator_map& opm ) {
   switch( *tok_ptr ) {
   case LBRACE_P:
      return parse_sequence_statement( tok_ptr, opm );
   case IF_K:
      return parse_if_statement( tok_ptr, opm );
   case VAR_K:
      return parse_var_statement( tok_ptr, opm );
   case RETURN_K:
      return parse_return_statement( tok_ptr, opm );
   default:
      return parse_expression_statement( tok_ptr, opm );
   }
}

#endif // PARSER_STATEMENT_H
