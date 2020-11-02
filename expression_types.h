#ifndef EXPRESSION_TYPES_H
#define EXPRESSION_TYPES_H

#include "lexer_types.h"

#include "string_utilities.h"

#include <vector>
#include <memory>
#include <string>

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

#endif //EXPRESSION_TYPES_H
