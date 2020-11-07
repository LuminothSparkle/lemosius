#ifndef PARSER_TYPES_H
#define PARSER_TYPES_H

#include "lexer.h"
#include "string_utilities.h"
#include "semantic_utilities.h"

#include <vector>
#include <memory>
#include <string>

#include <unordered_set>
#include <unordered_map>

// Expression Types

struct expression {
   virtual std::string str() const = 0;
};

struct terminal_expression : expression {
   const token* t;  // literal o identificador
   std::string str() const {
      return to_string( t );
   }
};

struct prefix_expression : expression {
   const token* op;
   std::unique_ptr<expression> ex;
   std::string str() const {
      return to_string( op ) + to_string( ex );
   }
};

struct suffix_expression : expression {
   std::unique_ptr<expression> ex;
   const token* op;
   std::string str() const {
      return to_string( ex ) + to_string( op );
   }
};

struct binary_expression : expression {
   std::unique_ptr<expression> ex1;
   const token* op;
   std::unique_ptr<expression> ex2;
   std::string str() const {
      return to_string( ex1, "", " " ) + to_string( op, "", " " ) + to_string( ex2 );
   }
};

struct call_expression : expression {
   const token* function_name;       // como sólo tenemos números como tipos, la llamada a función debe ser f(params) con f identificador
   std::vector<std::unique_ptr<expression>> params;                 // para simplificar la vida, no hay que permitir (f)(params)
   std::string str() const {
      return to_string( function_name ) + transform_join( params,
      []( const auto & p ) {
         return to_string( p );
      }, ",", "(", ")" );
   }
};

// Statement types

struct statement {
   virtual std::string str() const = 0;
};

struct sequence_statement : statement {
   std::vector<std::unique_ptr<statement>> body;

   std::string str() const {
      return "{...}";
   }
};

struct expression_statement : statement {
   std::unique_ptr<expression> body;

   std::string str() const {
      return to_string( body, "" ) + ";";
   }
};

struct if_statement : statement {
   std::vector<std::unique_ptr<sequence_statement>>  bodys;
   std::vector<std::unique_ptr<expression>>          conditions;
   std::unique_ptr<sequence_statement>               else_body      = nullptr;

   std::string str() const {
      std::string res;
      for( std::size_t idx = 0; idx < conditions.size(); ++idx ) {
         res.append( ( idx > 0 ? "else " : "" ) + to_string( conditions[idx], "if(", ")" ) + to_string( bodys[idx], " ", "\n" ) );
      }
      res.append( to_string( else_body, "else ", "\n" ) );
      return res;
   }
};

struct var_statement : statement {
   const token*                      name;
   std::unique_ptr<expression> value = nullptr;

   std::string str() const {
      return to_string( name, "var " ) + to_string( value, " := " ) + ";";
   }
};

struct return_statement : statement {
   std::unique_ptr<expression> return_value = nullptr;

   std::string str() const {
      return "return" + to_string( return_value, " " ) + ";";
   }
};

// Parser types

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
