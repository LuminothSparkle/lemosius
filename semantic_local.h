#ifndef SEMANTIC_LOCAL_H
#define SEMANTIC_LOCAL_H

#include "semantic_types.h"

#include "semantic_utilities.h"

#include <string>

void analyze_expression( const expression& e, scope_stack& ss );

void analyze_expression( const terminal_expression& e, scope_stack& ss ) {
   if( *e.t == IDENTIFIER_L ) {
      auto var = ss.find_symbol( *e.t ).get_variable();
      if( var == nullptr ) {
         throw std::pair<token, std::string>( *e.t, "Variable not declared" );
      }
      //...
   }
}

void analyze_expression( const prefix_expression& e, scope_stack& ss ) {
   analyze_expression( *e.ex, ss );
}

void analyze_expression( const suffix_expression& e, scope_stack& ss ) {
   analyze_expression( *e.ex, ss );
}

void analyze_expression( const binary_expression& e, scope_stack& ss ) {
   analyze_expression( *e.ex1, ss );
   analyze_expression( *e.ex2, ss );
}

void analyze_expression( const call_expression& e, scope_stack& ss ) {
   auto sym = ss.find_symbol( *e.function_name );
   if(!sym.get_overload(e.params.size( )).second) {
      if( auto var = sym.get_variable(); var != nullptr ) {
         throw std::vector<std::pair<token, std::string>>( {
            { *var,             "Variable declared here." },
            { *e.function_name, "Used as function here."  }
         } );
      } else if( sym.get_overload_set() != nullptr) {
         throw std::pair<token, std::string>( *e.function_name, "Overload declared doesn't fit with number of arguments." );
      } else {
         throw std::pair<token, std::string>( *e.function_name, "Overload not declared." );
      }
   }
   //...
}

void analyze_expression( const expression& e, scope_stack& ss ) {       // sí, if else if !
   if( typeid( e ) == typeid( terminal_expression ) ) {
      return analyze_expression( dynamic_cast<const terminal_expression&>( e ), ss );
   } else if( typeid( e ) == typeid( prefix_expression ) ) {
      return analyze_expression( dynamic_cast<const prefix_expression&>( e ),   ss );
   } else if( typeid( e ) == typeid( suffix_expression ) ) {
      return analyze_expression( dynamic_cast<const suffix_expression&>( e ),   ss );
   } else if( typeid( e ) == typeid( binary_expression ) ) {
      return analyze_expression( dynamic_cast<const binary_expression&>( e ),   ss );
   } else if( typeid( e ) == typeid( call_expression ) ) {
      return analyze_expression( dynamic_cast<const call_expression&>( e ),     ss );
   }
}

void analyze_statement( const statement& s, scope_stack& ss );

void analyze_statement( const sequence_statement& s, scope_stack& ss ) {
   ss.push();
   for( const auto& stmt : s.body ) {
      analyze_statement( *stmt, ss );
   }
   ss.pop();
}

void analyze_statement( const expression_statement& s, scope_stack& ss ) {
   if( s.body != nullptr ) {
      analyze_expression( *s.body, ss );
   }
}

void analyze_statement( const if_statement& s, scope_stack& ss ) {
   for( const auto& condition : s.conditions ) {
      analyze_expression( *condition, ss );
   }
   for( const auto& body : s.bodys ) {
      analyze_statement( *body, ss );
   }
   if( s.else_body != nullptr ) {
      analyze_statement( *s.else_body, ss );
   }
}

void analyze_statement( const var_statement& s, scope_stack& ss ) {
   if( !ss.top().try_declare( *s.name ) ) {
      throw std::vector<std::pair<token, std::string>>( {
         { *ss.top().find_variable( *s.name ), "Variable redeclaration, first declaration." },
         { *s.name,                            "Second declaration." }
      } );
   }
   if( s.value != nullptr ) {
      analyze_expression( *s.value, ss );
   }

}

void analyze_statement( const return_statement& s, scope_stack& ss ) {
   if( s.return_value != nullptr ) {
      analyze_expression( *s.return_value, ss );
   }
}

void analyze_statement( const statement& s, scope_stack& ss ) {      // sí, if else if !
   if( typeid( s ) == typeid( sequence_statement ) ) {
      return analyze_statement( dynamic_cast<const sequence_statement&>( s ),   ss );
   } else if( typeid( s ) == typeid( expression_statement ) ) {
      return analyze_statement( dynamic_cast<const expression_statement&>( s ), ss );
   } else if( typeid( s ) == typeid( if_statement ) ) {
      return analyze_statement( dynamic_cast<const if_statement&>( s ),         ss );
   } else if( typeid( s ) == typeid( var_statement ) ) {
      return analyze_statement( dynamic_cast<const var_statement&>( s ),        ss );
   } else if( typeid( s ) == typeid( return_statement ) ) {
      return analyze_statement( dynamic_cast<const return_statement&>( s ),     ss );
   }
}

void analyze_function( const function_declaration& f, scope_stack& ss ) {
   ss.push();
   for( const auto& var : f.parameters ) {
      ss.top().try_declare( *var );
   }
   analyze_statement( *f.body, ss );
   ss.pop();
}

#endif
