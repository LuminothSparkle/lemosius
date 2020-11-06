#ifndef LEXER_H
#define LEXER_H

#include "string_utilities.h"

#include <re2/re2.h>
#include <re2/stringpiece.h>

#include <algorithm>
#include <utility>
#include <string_view>
#include <string>
#include <vector>
#include <array>

// _K de keyword
// _P de puntuacion
// _L de literal
// _O de operador
// UNKNOWN y END_OF_INPUT son tokens especiales que deben ir en la primera y ultima posicion respectivamente

enum token_type {
   UNKNOWN = -1,
   PUBLIC_K,
   PRIVATE_K,
   INCLUDE_K,
   OPERATOR_K,
   PREFIX_K,
   SUFFIX_K,
   INFIX_K,
   LEFT_K,
   RIGHT_K,
   AS_K,
   PROC_K,
   VAR_K,
   RETURN_K,
   IF_K,
   ELSE_K,
   LPARENTHESIS_P,
   RPARENTHESIS_P,
   LBRACE_P,
   RBRACE_P,
   COMMA_P,
   SEMICOLON_P,
   ASSIGNMENT_O,
   NUMBER_L,
   STRING_L,
   OPERATOR_L,
   IDENTIFIER_L,
   END_OF_INPUT
};

struct token {
   token_type type;          // Indica el tipo de token
   std::string_view source;  // Indica la cadena del token en la fuente original

   std::string str() const {
      return std::string( source );
   }

   auto data( ) const {
      return source.data( );
   }

   auto begin( ) const  {
      return source.begin( );
   }

   auto end( ) const  {
      return source.end( );
   }

   const auto& back( ) const  {
      return source.back( );
   }

   const auto& front( ) const  {
      return source.front( );
   }

   operator token_type() const {
      return type;
   }

   operator std::string_view() const {
      return source;
   }

};

class lexer {
   std::array<std::vector<std::string>, END_OF_INPUT> token_forms; // Formas que pueden tomar los tokens reconocidos por el lexer

   RE2 generate_expresion( ) const {
      std::vector<std::string> other_tokens = {         // Tokens reconocidos como validos pero que no generan token para el parser
         R"(\s+)",                                      // Espacios y saltos de linea
                                                         // R"((?:[^\#](?s:.*)[^\#]|[^\#]?))"
         RE2::QuoteMeta( R"(###<)" ) + R"((?s:|.+))" + RE2::QuoteMeta( R"(>###)" ), // Comentarios multilinea
         RE2::QuoteMeta( R"(##<)" ) + R"([^\n]*(?:\n|$))",           // Comentarios de una linea
      };
      std::vector<std::string> parts( token_forms.size( ) + 1 );
      parts[0] = join( other_tokens, "|" );
      std::transform( token_forms.begin( ), token_forms.end( ), parts.begin( ) + 1,
      [&]( const std::vector<std::string>& t ) {
         return join( t, "|", "(", ")" );
      } );
      return RE2( join( parts, "|" ) );
   }

 public:
   std::vector<token> tokenization( const char*& ini, token_type stop, std::string error_mes = "" ) const  {
      // Genera expresion e inicializa la entrada del mismo
      RE2 e = generate_expresion( );
      re2::StringPiece input( ini );
      // Se generan los argumentos para la expresion regular, que son el arreglo de cadenas que son atrapadas, cada una identificada por el token_type
      std::array< re2::StringPiece, END_OF_INPUT > mt;
      std::array< RE2::Arg,         END_OF_INPUT > args;
      std::array< const RE2::Arg*,  END_OF_INPUT > m;
      std::transform( mt.begin( ),   mt.end( ),   args.begin( ), []( re2::StringPiece & s ) {
         return &s;
      } );
      std::transform( args.begin( ), args.end( ), m.begin( ),    []( RE2::Arg & a )        {
         return &a;
      } );
      // Inicia el analisis lexico
      std::vector<token> tokens;
      // Para hasta que termines la entrada y cuando el token de parada sea END_OF_INPUT o llegues al token de parada
      while( !input.empty( ) && ( stop == END_OF_INPUT || mt[stop].empty( ) ) ) {
         // Reconoce la cadena y si falla manda un error de compilador
         if( !RE2::ConsumeN( &input, e, m.begin( ), m.size( ) ) ) {
            throw std::pair<token, std::string>( {UNKNOWN, {input.data( ), 10}}, error_mes );
         }
         // Determina el tipo de token
         token_type type = token_type(
         std::find_if( mt.begin( ), mt.end( ), []( const re2::StringPiece & s ) {
            return !s.empty( );
         } ) - mt.begin( )
                           );
         // Insertalo al vector de tokens si no es un token de comentario o espacio o es el token de parada
         if( type != END_OF_INPUT && type != stop ) {
            tokens.push_back( { type, { mt[type].data( ), mt[type].length( ) } } );
         }
      }
      // Mueve la entrada hasta donde reconociste
      if( stop != END_OF_INPUT ) {
         ini = input.data( ) - mt[stop].size( );
      }
      // Inserta un token de END_OF_INPUT reemplazando siempre el token de parada
      tokens.push_back( { END_OF_INPUT, {ini, 0} } );
      return tokens;
   }

   void overwrite_operators( const std::vector<std::string_view>& ops ) {
      token_forms[OPERATOR_L].clear( );
      for( const auto& op : ops ) {
         token_forms[OPERATOR_L].push_back( RE2::QuoteMeta( op ) );
      }
      std::sort( token_forms[OPERATOR_L].begin(), token_forms[OPERATOR_L].end(), []( const auto & s1, const auto & s2 ) {
         return s1.size( ) > s2.size( );       // sí, hay que cuidar lo del maximum munch
      } );
   }

   lexer( ) {
      token_forms[PUBLIC_K] =       { RE2::QuoteMeta( "public" ) };
      token_forms[PRIVATE_K] =      { RE2::QuoteMeta( "private" ) };
      token_forms[INCLUDE_K] =      { RE2::QuoteMeta( "include" ) };
      token_forms[OPERATOR_K] =     { RE2::QuoteMeta( "operator" ) };
      token_forms[PREFIX_K] =       { RE2::QuoteMeta( "prefix" ) };
      token_forms[SUFFIX_K] =       { RE2::QuoteMeta( "suffix" ) };
      token_forms[INFIX_K] =        { RE2::QuoteMeta( "infix" ) };
      token_forms[LEFT_K] =         { RE2::QuoteMeta( "left" ) };
      token_forms[RIGHT_K] =        { RE2::QuoteMeta( "right" ) };
      token_forms[AS_K] =           { RE2::QuoteMeta( "as" ) };
      token_forms[PROC_K] =         { RE2::QuoteMeta( "proc" ) };
      token_forms[VAR_K] =          { RE2::QuoteMeta( "var" ) };
      token_forms[RETURN_K] =       { RE2::QuoteMeta( "return" ) };
      token_forms[IF_K] =           { RE2::QuoteMeta( "if" ) };
      token_forms[ELSE_K] =         { RE2::QuoteMeta( "else" ) };
      token_forms[LPARENTHESIS_P] = { RE2::QuoteMeta( "(" ) };
      token_forms[RPARENTHESIS_P] = { RE2::QuoteMeta( ")" ) };
      token_forms[LBRACE_P] =       { RE2::QuoteMeta( "{" ) };
      token_forms[RBRACE_P] =       { RE2::QuoteMeta( "}" ) };
      token_forms[COMMA_P] =        { RE2::QuoteMeta( "," ) };
      token_forms[SEMICOLON_P] =    { RE2::QuoteMeta( ";" ) };
      token_forms[ASSIGNMENT_O] =   { RE2::QuoteMeta( ":=" ) };
      token_forms[NUMBER_L] =       { R"(\d+(?:\.\d*)?)", R"(\.\d+)" };
      token_forms[STRING_L] =       { R"(\"(?:[^\"\n]|\")*\")" };
      token_forms[OPERATOR_L] =     { R"([)" + RE2::QuoteMeta( R"(!#$%&'*+-./:<=>?@\^`|~)" ) + R"(]+)" };
      token_forms[IDENTIFIER_L] =   { R"(\w+)" };
   }
};

const char* popback_tokens( std::vector<token>& tokens, token_type type ) {
   const char* last;
   while( tokens.back() != type ) {
      last = tokens.back().source.data();
      tokens.pop_back();
   }
   tokens.push_back( { END_OF_INPUT, { last, 0 } } );
   return last;
}

#endif //LEXER_H
