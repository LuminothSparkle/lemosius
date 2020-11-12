#ifndef SEMANTIC_LOCAL_H
#define SEMANTIC_LOCAL_H

#include "semantic_types.h"
#include "semantic_utilities.h"

#include <string>

VALUE_TYPE analyze_expression( const expression&, scope_stack&, resolution_table& );

VALUE_TYPE analyze_expression( const terminal_expression& e, scope_stack& ss, resolution_table& tbl ) {
   if( *e.tok_ptr == IDENTIFIER_L ) {
      if( auto var_decl = ss.find_symbol( *e.tok_ptr ).get_variable( ); var_decl != nullptr ) {
         tbl.variable_lookup.emplace( &e, var_decl );
      } else {
         throw std::pair<token, std::string>( *e.tok_ptr, "Variable not declared." );
      }
      return LVALUE;
   }
   return RVALUE;
}

VALUE_TYPE analyze_expression( const prefix_expression& e, scope_stack& ss, resolution_table& tbl ) {
   analyze_expression( *e.exp, ss, tbl );
   return RVALUE;
}

VALUE_TYPE analyze_expression( const suffix_expression& e, scope_stack& ss, resolution_table& tbl ) {
   analyze_expression( *e.exp, ss, tbl );
   return RVALUE;
}

VALUE_TYPE analyze_expression( const binary_expression& e, scope_stack& ss, resolution_table& tbl ) {
   auto ltype = analyze_expression( *e.exp1, ss, tbl );
   if(ltype == RVALUE && *e.op == ASSIGNMENT_O) {
      throw std::pair<token, std::string>( *e.op, "Trying to assign a rvalue.");
   }
   analyze_expression( *e.exp2, ss, tbl );
   return RVALUE;
}

VALUE_TYPE analyze_expression( const call_expression& e, scope_stack& ss, resolution_table& tbl ) {
   auto sym = ss.find_symbol( *e.function_name );
   if( auto var = sym.get_variable( ); var != nullptr ) {
      throw std::vector<std::pair<token, std::string>>( {
         { *e.function_name, "Variable used as function."  },
         { *var->name,             "Variable declaration." }
      } );
   }
   if( auto [func_decl, exists] = sym.get_overload( e.params.size( ) ); exists ) {
      tbl.function_lookup.emplace( &e, func_decl );
   } else {
      const auto& [user_ptr, builtin_ptr] = *sym.get_overload_sets( );
      if( user_ptr == nullptr && builtin_ptr == nullptr ) {
         throw std::pair<token, std::string>( *e.function_name, "Builtin function doesn't exist and an overload is not declared." );
      } else if( ( user_ptr == nullptr || !user_ptr->contains( e.params.size( ) ) ) && ( builtin_ptr == nullptr || !builtin_ptr->contains( e.params.size( ) ) ) ) {
         throw std::pair<token, std::string>( *e.function_name, "The builtin function or overload function doesn't fit with the number of arguments." );
      }
   }
   for( const auto& param : e.params ) {
      analyze_expression( *param, ss, tbl );
   }
   return RVALUE;
}

VALUE_TYPE analyze_expression( const expression& e, scope_stack& ss, resolution_table& tbl ) {       // sí, if else if !
   if( typeid( e ) == typeid( terminal_expression ) ) {
      return analyze_expression( dynamic_cast<const terminal_expression&>( e ), ss, tbl );
   } else if( typeid( e ) == typeid( prefix_expression ) ) {
      return analyze_expression( dynamic_cast<const prefix_expression&>( e ),   ss, tbl );
   } else if( typeid( e ) == typeid( suffix_expression ) ) {
      return analyze_expression( dynamic_cast<const suffix_expression&>( e ),   ss, tbl );
   } else if( typeid( e ) == typeid( binary_expression ) ) {
      return analyze_expression( dynamic_cast<const binary_expression&>( e ),   ss, tbl );
   } else if( typeid( e ) == typeid( call_expression ) ) {
      return analyze_expression( dynamic_cast<const call_expression&>( e ),     ss, tbl );
   }
   return RVALUE;
}

void analyze_statement( const statement&, scope_stack&, resolution_table& );

void analyze_statement( const sequence_statement& s, scope_stack& ss, resolution_table& tbl ) {
   ss.push( );
   for( const auto& stmt : s.body ) {
      analyze_statement( *stmt, ss, tbl );
   }
   ss.pop( );
}

void analyze_statement( const expression_statement& s, scope_stack& ss, resolution_table& tbl ) {
   if( s.body != nullptr ) {
      analyze_expression( *s.body, ss, tbl );
   }
}

void analyze_statement( const if_statement& s, scope_stack& ss, resolution_table& tbl ) {
   for( const auto& condition : s.conditions ) {
      analyze_expression( *condition, ss, tbl );
   }
   for( const auto& body : s.bodys ) {
      analyze_statement( *body, ss, tbl );
   }
   if( s.else_body != nullptr ) {
      analyze_statement( *s.else_body, ss, tbl );
   }
}

void analyze_statement( const var_statement& s, scope_stack& ss, resolution_table& tbl ) {
   if( s.value != nullptr ) {
      analyze_expression( *s.value, ss, tbl );
   }
   if( !ss.top( ).try_declare( s ) ) {
      throw std::vector<std::pair<token, std::string>>( {
         { *s.name,                                   "Variable redeclaration." },
         { *ss.top( ).find_variable( *s.name )->name, "Previously declared." }
      } );
   }
}

void analyze_statement( const return_statement& s, scope_stack& ss, resolution_table& tbl ) {
   if( s.return_value != nullptr ) {
      analyze_expression( *s.return_value, ss, tbl );
   }
}

void analyze_statement( const statement& s, scope_stack& ss, resolution_table& tbl ) {      // sí, if else if !
   if( typeid( s ) == typeid( sequence_statement ) ) {
      return analyze_statement( dynamic_cast<const sequence_statement&>( s ),   ss, tbl );
   } else if( typeid( s ) == typeid( expression_statement ) ) {
      return analyze_statement( dynamic_cast<const expression_statement&>( s ), ss, tbl );
   } else if( typeid( s ) == typeid( if_statement ) ) {
      return analyze_statement( dynamic_cast<const if_statement&>( s ),         ss, tbl );
   } else if( typeid( s ) == typeid( var_statement ) ) {
      return analyze_statement( dynamic_cast<const var_statement&>( s ),        ss, tbl );
   } else if( typeid( s ) == typeid( return_statement ) ) {
      return analyze_statement( dynamic_cast<const return_statement&>( s ),     ss, tbl );
   }
}

void analyze_function( const function_declaration& f, scope_stack& ss, resolution_table& tbl ) {
   if( typeid( *f.body->body.back( ) ) != typeid( return_statement ) ) {
      f.body->body.emplace_back( std::make_unique<return_statement>( ) );
   }
   ss.push( );
   for( const auto& var : f.parameters ) {
      ss.top( ).try_declare( *var );
   }
   analyze_statement( *f.body, ss, tbl );
   ss.pop( );
}

#endif
