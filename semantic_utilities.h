#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include "parser.h"

#include <cstdlib>

#include <string>
#include <string_view>
#include <utility>
#include <charconv>

template<typename HasAccessType>
bool is_public( const HasAccessType& t ) {
   return t.access == nullptr || *t.access == PUBLIC_K;
};

bool isoctal( char c ) {
   return isdigit( c ) && c < '8';
}

template<typename NumType>
std::pair<NumType, bool> get_representation( const std::string_view& s ) {
   //      :( GCC aún no tiene from_chars para floating point
   //long double res;
   //auto [end, error] = std::from_chars(s.data( ), s.data( ) + s.size( ), res);
   //return { T(res), (!error && p == s.end( ) && T(res) == res) }; */
   char* cptr;
   std::string buffer( s );
   long double res = std::strtold( buffer.c_str( ), &cptr );
   if( cptr != buffer.end( ).base( ) ) {
      return { NumType( ), false };
   }
   return { NumType( res ), NumType( res ) == res };
   //try {      // horrible, pero más fácil de escribir que el que usa std::strold, en particular porque strold requiere una cadena con terminador
   //   long double res = std::stold(std::string(s));      // nosotros sabemos que nuestros string_views vienen de tokens válidos y que acaban en algo que ya no es número,
   //   return { T(res), T(res) == res };                  // pero prefiero no andarle jugando al vivo
   //} catch (...) {
   //   return { T( ), false };
   //}
}

template<typename NumType>
NumType get_representation( const token& tok, const std::string& err_mes = "" ) {
   auto [val, success] = get_representation<NumType>( std::string_view( tok ) );
   if( !success ) {
      throw std::pair<token, std::string>( tok, err_mes );
   }
   return val;
}

std::pair<std::string, const char*> unquoted_str( const std::string_view& sv ) {
   auto it = sv.begin( );
   if( sv.empty( ) || sv.front( ) != '"' ) { //Mejor para no tener problemas, ya que igual se checa en el lexer pero tambien aqui por si las dudas.
      return { "", sv.begin( ) };
   }
   std::string res;
   for( std::advance( it, 1 ); it != sv.end( ) && *it != '"'; std::advance( it, 1 ) ) {
      if( *it == '\\' ) {
         std::advance( it, 1 );
         constexpr std::string_view escaped = R"(abtnvfre0"'\)";
         constexpr std::string_view replacement = "\a\b\t\n\v\f\r\x1B\0\"\'\\";
         if( std::size_t idx = escaped.find( *it ); idx < escaped.size( ) ) {
            res.push_back( replacement[idx] );
            std::advance( it, 1 );
         } else if( bool is_hex = *it == 'x'; is_hex || isoctal( *it ) ) {
            std::size_t base = ( is_hex ? 16 : 8 );
            std::advance( it, is_hex );
            auto obtain_char = [&base, &sv]( auto & it, unsigned char& c ) {
               unsigned char val = 0;
               auto first = it;
               it = std::from_chars( first, sv.end( ), val, base ).ptr;
               return first != it;
            };
            unsigned char c;
            if( !obtain_char( it, c ) ) {
               return { res, it };
            }
            res.push_back( c );
            while( is_hex && obtain_char( it, c ) ) {
               res.push_back( c );
            }
         } else {
            return { res, it };
         }
      } else {
         res.push_back( *it );
      }
   }
   return { res, std::next( it, *it == '"' ) };
}

std::string unquoted_str( const token& tok, const std::string& err_mes = "" ) {
   auto [str, cptr] = unquoted_str( std::string_view( tok ) );
   if( cptr != tok.end( ) ) {
      throw std::pair<token, std::string>( { token_type{tok}, { cptr, tok.size( ) } }, err_mes );
   }
   return str;
}

#endif
