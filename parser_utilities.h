#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include "lexer_types.h"
#include "parser_types.h"

#include <initializer_list>
#include <algorithm>
#include <string>
#include <utility>

bool is_access( const token& t ) {
   return t == PUBLIC_K || t == PRIVATE_K;
}

bool is_position( const token& t ) {
   return t == PREFIX_K || t == SUFFIX_K || t == INFIX_K;
}

bool is_associativity( const token& t ) {
   return t == LEFT_K || t == RIGHT_K;
}

bool is_literal( const token& t ) {
   return t == IDENTIFIER_L || t == NUMBER_L;
}

auto match_any( const std::initializer_list<token_type>& il ) {
   return [&il]( token_type t ) {
      return std::find( il.begin( ), il.end( ), t ) != il.end( );
   };
}

template<typename P>
const token* optional_match( const token*& t, P pred ) {
   return ( pred( *t ) ? t++ : nullptr );
}

const token* optional_match( const token*& t, token_type type ) {
   return optional_match( t, match_any( { type } ) );
}

// Template de match con predicado
template<typename P>
const token* match( const token*& t, P pred, const std::string& mes = "" ) {
   if( !pred( *t ) ) {
      throw std::make_pair( *t, "Syntax Error: " + mes );
   }
   return t++;
}

const token* match( const token*& t, token_type type, const std::string& mes = "" ) {
   return match( t, match_any( { type } ), mes );
}

bool is_prefix_operator( const operator_map& opm, const token& t ) {
   return opm.prefix_operators.contains( t.str() );
}

bool is_suffix_operator( const operator_map& opm, const token& t ) {
   return opm.suffix_operators.contains( t.str() );
}

bool is_binary_operator( const operator_map& opm, const token& t ) {
   return opm.infix_operators.contains( t.str() );
}

bool is_left_assoc( const operator_map& opm, const token& t ) {
   return opm.infix_operators.at( t.str() ).second;
}

std::int64_t precedence( const operator_map& opm, const token& t ) {
   return opm.infix_operators.at( t.str() ).first;
}

#endif
