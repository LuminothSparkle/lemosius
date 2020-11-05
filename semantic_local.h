#ifndef SEMANTIC_LOCAL_H
#define SEMANTIC_LOCAL_H

<<<<<<< HEAD
#include"lexer.h"
#include"parser.h"
#include"semantic_global.h"
#include"semantic_utilities.h"
#include<list>
#include<unordered_set>
#include<typeinfo>

class scope {
   scope* parent;
   std::unordered_map<std::string_view, token*> variables;
   const std::unordered_map<std::string_view, std::map<std::size_t, program_resources::visible_function>>& function_overloads;

public:
   scope(const decltype(program_resources::function_overloads)& f)
   : parent(nullptr), function_overloads(f) {
   }

   scope(scope* p)     // no puede ser nulo (debería ser referencia, pero no quiero que haya ambigüedad con un constructor por copia)
   : parent(p), function_overloads(p->function_overloads) {
   }

   bool try_declare(std::string_view s) {
      // intentar declarar en .variables
   }

   std::pair<token*, token_type> find_symbol(std::string_view s) {
      // generalmente vamos a usar esta función para buscar una variable siempre, pero hay que identificar tres casos (para el reporte de errores):
      // 1) no existe el símbolo (regresamos nullptr)
      // 2) sí existe el símbolo y sí es variable
      // 3) sí existe el símbolo pero es una función
      // el token apunta al identificador de la variable; el token_type lo construimos nosotros (NUMBER o FUNCTION) dependiendo si es o no variable

      // buscar en las variables del nivel actual
      // si no está entonces hacemos recursión con el padre
      // si no tenemos padre entonces buscamos en las funciones
   }

   // nulo si no la encontramos
   operator_declaration* find_function(std::string_view s, std::size_t arity) {
      // buscar que no esté oculta por una variable
   }
};

void analyze_expression(const expression& e, const program_resources& pr, scope& current);

void analyze_expression(const terminal_expression& e, const program_resources& pr, scope& current) {

}

void analyze_expression(const prefix_expression& e, const program_resources& pr, scope& current) {

}

void analyze_expression(const suffix_expression& e, const program_resources& pr, scope& current) {

}

void analyze_expression(const binary_expression& e, const program_resources& pr, scope& current) {

}

void analyze_expression(const call_expression& e, const program_resources& pr, scope& current) {

}

void analyze_expression(const statement& e, const program_resources& pr, scope& current) {         // sí, if else if !
   if (typeid(e) == typeid(terminal_expression)) {
      return analyze_expression(dynamic_cast<const terminal_expression&>(e), pr, current);
   } else if (typeid(e) == typeid(prefix_expression)) {
      return analyze_expression(dynamic_cast<const prefix_expression&>(e), pr, current);
   } else if (typeid(e) == typeid(suffix_expression)) {
      return analyze_expression(dynamic_cast<const suffix_expression&>(e), pr, current);
   } else if (typeid(e) == typeid(binary_expression)) {
      return analyze_expression(dynamic_cast<const binary_expression&>(e), pr, current);
   } else if (typeid(e) == typeid(call_expression)) {
      return analyze_expression(dynamic_cast<const call_expression&>(e), pr, current);
   }
}

void analyze_statement(const statement& s, const program_resources& pr, scope& current);

void analyze_statement(const sequence_statement& s, const program_resources& pr, scope& current) {

}

void analyze_statement(const expression_statement& s, const program_resources& pr, scope& current) {

}

void analyze_statement(const if_statement& s, const program_resources& pr, scope& current) {
   // analizar la condición
   parse_si: {
      scope inner(&current);
      // enviar inner a la recursión
   }

   parte_no: {
      scope inner(&current);
      // enviar inner a la recursión
   }
}

void analyze_statement(const var_statement& s, const program_resources& pr, scope& current) {

}

void analyze_statement(const statement& s, const program_resources& pr, scope& current) {        // sí, if else if !
   if (typeid(s) == typeid(sequence_statement)) {
      return analyze_statement(dynamic_cast<const sequence_statement&>(s), pr, current);
   } else if (typeid(s) == typeid(expression_statement)) {
      return analyze_statement(dynamic_cast<const expression_statement&>(s), pr, current);
   } else if (typeid(s) == typeid(if_statement)) {
      return analyze_statement(dynamic_cast<const if_statement&>(s), pr, current);
   } else if (typeid(s) == typeid(var_statement)) {
      return analyze_statement(dynamic_cast<const var_statement&>(s), pr, current);
   }
}

void analyze_function(const function_declaration& f, const program_resources& pr, scope& current) {
   //...
   analyze_statement(*f.body, pr, current);
}

void analyze_program(const syntax_tree& tree, const program_resources& pr) {
   scope current(pr.function_overloads);
   for (const auto& f : tree.functions) {
      analyze_function(f, pr, current);
   }
=======
#include "lexer_types.h"
#include "parser_types.h"
#include "semantic_types.h"

#include "semantic_utilities.h"

#include <string>
#include <typeinfo>
#include <utility>
#include <any>

void analyze_expression( const expression& e, scope& current );

void analyze_expression( const terminal_expression& e, scope& current ) {
   if( *e.t == IDENTIFIER_L ) {
      auto [res, type] = current.find_symbol( *e.t );
      if( type != scope::VARIABLE ) {
         using namespace std::string_literals;
         throw std::make_pair( *e.t, "Variable not declared"s );
      }
   }
}

void analyze_expression( const prefix_expression& e, scope& current ) {
   analyze_expression( *e.ex, current );
}

void analyze_expression( const suffix_expression& e, scope& current ) {
   analyze_expression( *e.ex, current );
}

void analyze_expression( const binary_expression& e, scope& current ) {
   analyze_expression( *e.ex1, current );
   analyze_expression( *e.ex2, current );
}

void analyze_expression( const call_expression& e, scope& current ) {
   auto [res, type] = current.find_symbol( *e.function_name );
   if( type != scope::FUNCTION_MAP ) {
      using namespace std::string_literals;
      if( type == scope::VARIABLE ) {
         auto decl_mes = std::make_pair( std::any_cast<token>( res ), "Variable here declared"s );
         auto use_mes = std::make_pair( *e.function_name, "Used as function here"s );
         throw std::vector<std::pair<token, std::string>>( { decl_mes, use_mes } );
      }
      throw std::make_pair( *e.function_name, "Overload not declared"s );
   }
   try {
      auto overloads = std::any_cast<const std::map<std::size_t, program_resources::visible_function>&>( res );
      overloads.at( e.params.size() );
   } catch( const std::out_of_range& err ) {
      using namespace std::string_literals;
      throw std::make_pair( *e.function_name, "Overload declared doesn't fit with number of arguments"s );
   }
}

void analyze_expression( const expression& e, scope& current ) {       // sí, if else if !
   if( typeid( e ) == typeid( terminal_expression ) ) {
      return analyze_expression( dynamic_cast<const terminal_expression&>( e ), current );
   } else if( typeid( e ) == typeid( prefix_expression ) ) {
      return analyze_expression( dynamic_cast<const prefix_expression&>( e ),   current );
   } else if( typeid( e ) == typeid( suffix_expression ) ) {
      return analyze_expression( dynamic_cast<const suffix_expression&>( e ),   current );
   } else if( typeid( e ) == typeid( binary_expression ) ) {
      return analyze_expression( dynamic_cast<const binary_expression&>( e ),   current );
   } else if( typeid( e ) == typeid( call_expression ) ) {
      return analyze_expression( dynamic_cast<const call_expression&>( e ),     current );
   }
}

void analyze_statement( const statement& s, scope& current );

void analyze_statement( const sequence_statement& s, scope& current ) {
   scope seq_scope = scope::get_new_scope();
   for( const auto& stmt : s.body ) {
      analyze_statement( *stmt, seq_scope );
   }
}

void analyze_statement( const expression_statement& s, scope& current ) {
   if( s.body != nullptr ) {
      analyze_expression( *s.body, current );
   }
}

void analyze_statement( const if_statement& s, scope& current ) {
   for( const auto& condition : s.conditions ) {
      analyze_expression( *condition, current );
   }
   for( const auto& body : s.bodys ) {
      analyze_statement( *body, current );
   }
   if( s.else_body != nullptr ) {
      analyze_statement( *s.else_body, current );
   }
}

void analyze_statement( const var_statement& s, scope& current ) {
   if( !current.try_declare( *s.name ) ) {
      using namespace std::string_literals;
      auto [var, type] = current.find_symbol( *s.name );
      auto decl1_mes = std::make_pair( std::any_cast<token>( var ), "First declaration"s );
      auto decl2_mes = std::make_pair( *s.name, "Second declaration"s );
      throw std::vector<std::pair<token, std::string>>( { decl1_mes, decl2_mes } );
   }
   if( s.value != nullptr ) {
      analyze_expression( *s.value, current );
   }

}

void analyze_statement( const return_statement& s, scope& current ) {
   using namespace std::string_literals;
   if( s.return_value != nullptr ) {
      analyze_expression( *s.return_value, current  );
   }
}

void analyze_statement( const statement& s, scope& current ) {      // sí, if else if !
   if( typeid( s ) == typeid( sequence_statement ) ) {
      return analyze_statement( dynamic_cast<const sequence_statement&>( s ),   current );
   } else if( typeid( s ) == typeid( expression_statement ) ) {
      return analyze_statement( dynamic_cast<const expression_statement&>( s ), current );
   } else if( typeid( s ) == typeid( if_statement ) ) {
      return analyze_statement( dynamic_cast<const if_statement&>( s ),         current );
   } else if( typeid( s ) == typeid( var_statement ) ) {
      return analyze_statement( dynamic_cast<const var_statement&>( s ),        current );
   } else if( typeid( s ) == typeid( return_statement ) ) {
      return analyze_statement( dynamic_cast<const return_statement&>( s ),     current );
   }
}

void analyze_function( const function_declaration& f, scope& current ) {
   scope func_scope = scope::get_new_scope();
   for( const auto& var : f.parameters ) {
      func_scope.try_declare( *var );
   }
   analyze_statement( *f.body, func_scope );
>>>>>>> master
}

#endif
