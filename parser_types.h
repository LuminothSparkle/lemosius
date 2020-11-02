#ifndef PARSER_TYPES_H
#define PARSER_TYPES_H

#include "lexer_types.h"
#include "statement_types.h"

#include "string_utilities.h"
#include "semantic_utilities.h"

#include <vector>
#include <memory>
#include <string>

#include <unordered_set>
#include <unordered_map>

struct include_declaration {
   const token* access     = nullptr;
   const token* file_name;

   std::string str() const {
      return to_string( access, "", " " ) + to_string( file_name, "include ", ";" );
   }
};

struct operator_declaration {
   const token* access        = nullptr;
   const token* symbol;
   const token* position;
   const token* associativity = nullptr;
   const token* precedence    = nullptr;
   const token* function;

   std::string str() const {
      std::string decl = to_string( access, "", " " );
      decl.append( to_string( symbol, "operator " ) );
      decl.append( to_string( position ) );
      decl.append( *position == INFIX_K ? to_string( associativity, "(" ) + to_string( precedence, " ", ")" ) : "" );
      decl.append( to_string( function, " as ", ";" ) );
      return decl;
   }
};

struct function_declaration {
   const token*                        access     = nullptr;
   const token*                        name;
   std::vector<const token*>           parameters;
   std::unique_ptr<sequence_statement> body;

   std::string str() const {
      return to_string( access, "", " " ) + to_string( name, "proc " ) + transform_join( parameters, []( const auto & s ) {
         return to_string( s );
      }, ",", "(", ");" );
   }
};

struct syntax_tree {
   struct header_declarations {
      std::vector<include_declaration>  includes;
      std::vector<operator_declaration> operators;
   };
   header_declarations               header;
   std::vector<function_declaration> functions;
};

struct operator_map {
   std::unordered_set<std::string_view> suffix_operators, prefix_operators;
   std::unordered_map<std::string_view, std::pair<std::int64_t, bool>> infix_operators;

   explicit operator_map( const std::vector<operator_declaration>& ops ) {
      infix_operators.insert( { ":=", { -1, false } } ); //No estoy seguro de si agregarlo pero me parece la solución más facil considerando que aceptemos cosas como (c := (a := b))
      //Si no es más facil hace un statement especial, pero además si acepta como solución no se si sea mejor ponerle precedencia cero
      //Y dejar que los demás solo pongan precedencias de 1 en adelante o ponerle -1 y aceptar de 0 en adelante.
      for( const auto& op : ops ) {
         if( *op.position == INFIX_K ) {
            infix_operators.insert( { op.symbol->source, { get_representation<std::uint32_t>( *op.precedence ), *op.associativity == LEFT_K } } );
         } else if( *op.position == SUFFIX_K ) {
            suffix_operators.insert( op.symbol->source );
         } else if( *op.position == PREFIX_K ) {
            prefix_operators.insert( op.symbol->source );
         }
      }
   }
};

#endif //PARSER_TYPES_H
