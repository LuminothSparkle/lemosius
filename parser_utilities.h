#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include "lexer.h"

#include <initializer_list>
#include <algorithm>
#include <string>
#include <utility>

bool is_access( const token& tok ) {
   return tok == PUBLIC_K || tok == PRIVATE_K;
}

bool is_position( const token& tok ) {
   return tok == PREFIX_K || tok == SUFFIX_K || tok == INFIX_K;
}

bool is_associativity( const token& tok ) {
   return tok == LEFT_K || tok == RIGHT_K;
}

bool is_literal( const token& tok ) {
   return tok == IDENTIFIER_L || tok == NUMBER_L;
}

auto match_any( const std::initializer_list<token_type>& il ) {
   return [&il]( token_type tok ) {
      return std::find( il.begin( ), il.end( ), tok ) != il.end( );
   };
}

template<typename Pred>
const token* optional_match( const token*& tok_ptr, Pred P ) {
   return ( P( *tok_ptr ) ? tok_ptr++ : nullptr );
}

const token* optional_match( const token*& tok_ptr, token_type type ) {
   return optional_match( tok_ptr, match_any( { type } ) );
}

// Template de match con predicado
template<typename Pred>
const token* match( const token*& tok_ptr, Pred P, const std::string& err_mes = "" ) {
   if( !P( *tok_ptr ) ) {
      throw std::pair<token, std::string>( *tok_ptr, err_mes );
   }
   return tok_ptr++;
}

const token* match( const token*& tok_ptr, token_type type, const std::string& err_mes = "" ) {
   return match( tok_ptr, match_any( { type } ), err_mes );
}

bool is_prefix_operator( const operator_map& opm, const token& tok ) {
   return opm.prefix_operators.contains( std::string_view( tok ) );
}

bool is_suffix_operator( const operator_map& opm, const token& tok ) {
   return opm.suffix_operators.contains( std::string_view( tok ) );
}

bool is_binary_operator( const operator_map& opm, const token& tok ) {
   return opm.infix_operators.contains( std::string_view( tok ) );
}

bool is_left_assoc( const operator_map& opm, const token& tok ) {
   return opm.infix_operators.at( std::string_view( tok ) ).second;
}

std::int64_t precedence( const operator_map& opm, const token& tok ) {
   return opm.infix_operators.at( std::string_view( tok ) ).first;
}

#endif
