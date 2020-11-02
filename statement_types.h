#ifndef STATEMENT_TYPES_H
#define STATEMENT_TYPES_H

#include "lexer_types.h"
#include "expression_types.h"

#include "string_utilities.h"

#include <memory>
#include <string>
#include <vector>

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

#endif //STATEMENT_TYPES_H
