#ifndef PARSER_H
#define PARSER_H

#include "parser_types.h"
#include "parser_utilities.h"
#include "parser_statement.h"

#include <vector>
#include <utility>

auto parse_include_decl( const token*& tok_ptr ) {
   include_declaration inc;
   inc.access = optional_match( tok_ptr, is_access );
   match( tok_ptr, INCLUDE_K, "Expecting include" );
   inc.file_name = match( tok_ptr, STRING_L, "Expecting a string literal" );
   match( tok_ptr, SEMICOLON_P, "Expecting ;" );
   return inc;
}

auto parse_operator_decl( const token*& tok_ptr ) {
   operator_declaration op;
   op.access = optional_match( tok_ptr, is_access );
   match( tok_ptr, OPERATOR_K, "Expecting operator" );
   op.symbol   = match( tok_ptr, OPERATOR_L, "Expecting an operator" );
   op.position = match( tok_ptr, is_position, "Expecting infix, prefix or suffix" );
   if( *op.position == INFIX_K ) {
      match( tok_ptr, LPARENTHESIS_P, "Expecting (" );
      op.associativity = match( tok_ptr, is_associativity, "Expecting left or right" );
      op.precedence   = match( tok_ptr, NUMBER_L, "Expecting a number literal" );
      match( tok_ptr, RPARENTHESIS_P, "Expecting )" );
   }
   match( tok_ptr, AS_K, "Expecting as" );
   op.function = match( tok_ptr, IDENTIFIER_L, "Expecting an identifier" );
   match( tok_ptr, SEMICOLON_P, "Expecting ;" );
   return op;
}

auto parse_header( const token*& tok_ptr ) {
   header_declarations hd;
   while( *tok_ptr == INCLUDE_K || ( is_access( *tok_ptr ) && *( tok_ptr + 1 ) == INCLUDE_K ) ) {
      hd.includes.push_back( parse_include_decl( tok_ptr ) );
   }
   while( *tok_ptr == OPERATOR_K || ( is_access( *tok_ptr ) && *( tok_ptr + 1 ) == OPERATOR_K ) ) {
      hd.operators.push_back( parse_operator_decl( tok_ptr ) );
   }
   match( tok_ptr, END_OF_INPUT, "Expecting a header or operator declaration" );
   return hd;
}

auto parse_function_decl( const token*& tok_ptr, const operator_map& opm ) {
   function_declaration func;
   func.access = optional_match( tok_ptr, is_access );
   match( tok_ptr, PROC_K, "Expecting proc" );
   func.name = match( tok_ptr, IDENTIFIER_L, "Expecting an identifier" );
   match( tok_ptr, LPARENTHESIS_P, "Expecting (" );
   while( *tok_ptr == IDENTIFIER_L ) {
      func.parameters.push_back( match( tok_ptr, IDENTIFIER_L, "Expecting an identifier" ) );
      if( *tok_ptr != RPARENTHESIS_P ) {
         match( tok_ptr, COMMA_P, "Expecting ," );
      }
   }
   match( tok_ptr, RPARENTHESIS_P, "Expecting )" );
   // Parsea las demas sentencias de la funcion
   func.body = parse_sequence_statement( tok_ptr, opm );
   return func;
}

auto parse_program( const token*& tok_ptr, const std::vector<operator_declaration>& op_decls ) {
   std::vector<function_declaration> funcs;
   operator_map opm( op_decls );
   while( *tok_ptr == PROC_K || ( is_access( *tok_ptr ) && *( tok_ptr + 1 ) == PROC_K ) ) {
      funcs.push_back( parse_function_decl( tok_ptr, opm ) );
   }
   match( tok_ptr, END_OF_INPUT, "Expecting a function declaration" );
   return funcs;
}

#endif
