#ifndef SEMANTIC_LOCAL_H
#define SEMANTIC_LOCAL_H

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
}

#endif
