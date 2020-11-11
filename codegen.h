#ifndef CODEGEN_H
#define CODEGEN_H

#include "semantic_types.h"

#include <ostream>
#include <sstream>
#include <string_view>
#include <string>

template<typename T>
std::string get_identifier_name( const T& t ) {
   std::ostringstream oss;
   oss << std::string_view( *t.name ) << "_" << &t;
   return std::move( oss ).str( );
}

std::string get_builtin_name( const std::string_view& name ) {
   return std::string( name );
}

template<typename T>
std::string get_identifier_declaration( const T& t ) {
   return "long double " + get_identifier_name( t );
}

std::string get_literal_constant( const std::string_view& sv ) {
   return std::string( sv ) + "L";
}

void write_expression( const expression&, const resolution_table&, std::ostream& );

void write_expression( const terminal_expression& e, const resolution_table& tbl, std::ostream& os ) {
   if( *e.tok_ptr == IDENTIFIER_L ) {
      os << get_identifier_name( *tbl.variable_lookup.at( &e ) );
   } else {
      os << get_literal_constant( *e.tok_ptr );
   }
}

void write_expression( const prefix_expression& e, const resolution_table& tbl, std::ostream& os ) {
   if( auto func_decl = tbl.operator_lookup.at( std::string_view( *e.op ) ).at( PREFIX_K ) ) {
      os << get_identifier_name( *func_decl );
   } else {
      os << get_builtin_name( *tbl.operator_overloads.at( std::string_view( *e.op ) ).at( PREFIX_K ).declaration->function );
   }
   os << "(";
   write_expression( *e.exp, tbl, os );
   os << ")";
}

void write_expression( const suffix_expression& e, const resolution_table& tbl, std::ostream& os ) {
   if( auto func_decl = tbl.operator_lookup.at( std::string_view( *e.op ) ).at( SUFFIX_K ) ) {
      os << get_identifier_name( *func_decl );
   } else {
      os << get_builtin_name( *tbl.operator_overloads.at( std::string_view( *e.op ) ).at( SUFFIX_K ).declaration->function );
   }
   os << "(";
   write_expression( *e.exp, tbl, os );
   os << ")";
}

void write_expression( const binary_expression& e, const resolution_table& tbl, std::ostream& os ) {
   if( auto func_decl = tbl.operator_lookup.at( std::string_view( *e.op ) ).at( INFIX_K ) ) {
      os << get_identifier_name( *func_decl );
   } else {
      os << get_builtin_name( *tbl.operator_overloads.at( std::string_view( *e.op ) ).at( INFIX_K ).declaration->function );
   }
   os << "(";
   write_expression( *e.exp1, tbl, os );
   os << ", ";
   write_expression( *e.exp2, tbl, os );
   os << ")";
}

void write_expression( const call_expression& e, const resolution_table& tbl, std::ostream& os ) {
   if( auto func_decl = tbl.function_lookup.at( &e ) ) {
      os << get_identifier_name( *func_decl );
   } else {
      os << get_builtin_name( *e.function_name );
   }
   os << "(";
   for( std::size_t i = 0; i < e.params.size( ); ++i ) {
      if( i > 0 ) {
         os << ",";
      }
      write_expression( *e.params[i], tbl, os );
   }
   os << ")";
}

void write_expression( const expression& e, const resolution_table& tbl, std::ostream& os ) {      // sí, if else if !
   if( typeid( e ) == typeid( terminal_expression ) ) {
      return write_expression( dynamic_cast<const terminal_expression&>( e ), tbl, os );
   } else if( typeid( e ) == typeid( prefix_expression ) ) {
      return write_expression( dynamic_cast<const prefix_expression&>( e ),   tbl, os );
   } else if( typeid( e ) == typeid( suffix_expression ) ) {
      return write_expression( dynamic_cast<const suffix_expression&>( e ),   tbl, os );
   } else if( typeid( e ) == typeid( binary_expression ) ) {
      return write_expression( dynamic_cast<const binary_expression&>( e ),   tbl, os );
   } else if( typeid( e ) == typeid( call_expression ) ) {
      return write_expression( dynamic_cast<const call_expression&>( e ),     tbl, os );
   }
}

void write_statement( const statement&, const resolution_table&, std::ostream& );

void write_statement( const sequence_statement& s, const resolution_table& tbl, std::ostream& os ) {
   os << "{\n";
   for( const auto& stmt : s.body ) {
      write_statement( *stmt, tbl, os );
   }
   os << "}\n";
}

void write_statement( const expression_statement& s, const resolution_table& tbl, std::ostream& os ) {
   if( s.body != nullptr ) {
      write_expression( *s.body, tbl, os );
   }
   os << ";\n";
}

void write_statement( const if_statement& s, const resolution_table& tbl, std::ostream& os ) {
   for( std::size_t i = 0; i < s.conditions.size( ); ++i ) {
      if( i > 0 ) {
         os << "else ";
      }
      os << "if (";
      write_expression( *s.conditions[i], tbl, os );
      os << ")";
      write_statement( *s.bodys[i], tbl, os );
   }
   if( s.else_body != nullptr ) {
      os << "else ";
      write_statement( *s.else_body, tbl, os );
   }
}

void write_statement( const var_statement& s, const resolution_table& tbl, std::ostream& os ) {
   os << get_identifier_declaration( s );
   if( s.value != nullptr ) {
      os << " = ";
      write_expression( *s.value, tbl, os );
   }
   os << ";\n";
}

void write_statement( const return_statement& s, const resolution_table& tbl, std::ostream& os ) {
   os << "return ";
   if( s.return_value != nullptr ) {
      write_expression( *s.return_value, tbl, os );
   } else {
      os << get_literal_constant( "0" );
   }
   os << ";\n";
}

void write_statement( const statement& s, const resolution_table& tbl, std::ostream& os ) {     // sí, if else if !
   if( typeid( s ) == typeid( sequence_statement ) ) {
      return write_statement( dynamic_cast<const sequence_statement&>( s ),   tbl, os );
   } else if( typeid( s ) == typeid( expression_statement ) ) {
      return write_statement( dynamic_cast<const expression_statement&>( s ), tbl, os );
   } else if( typeid( s ) == typeid( if_statement ) ) {
      return write_statement( dynamic_cast<const if_statement&>( s ),         tbl, os );
   } else if( typeid( s ) == typeid( var_statement ) ) {
      return write_statement( dynamic_cast<const var_statement&>( s ),        tbl, os );
   } else if( typeid( s ) == typeid( return_statement ) ) {
      return write_statement( dynamic_cast<const return_statement&>( s ),     tbl, os );
   }
}

void write_function_header( const function_declaration& f, const resolution_table& tbl, std::ostream& os ) {
   os << get_identifier_declaration( f ) << "(";
   for( std::size_t i = 0; i < f.parameters.size( ); ++i ) {
      if( i > 0 ) {
         os << ",";
      }
      os << get_identifier_declaration( *f.parameters[i] );
   }
   os << ")";
}

void write_function_definition( const function_declaration& f, const resolution_table& tbl, std::ostream& os ) {
   write_function_header( f, tbl, os );
   write_statement( *f.body, tbl, os );
}

void write_program( const program_resources& pr, const resolution_table& tbl, bool is_main, std::ostream& os ) {
   for( const auto& f : pr.tree.functions ) {
      write_function_header( f, tbl, os );
      os << ";\n";
   }
   os << "\n";
   for( const auto& f : pr.tree.functions ) {
      write_function_definition( f, tbl, os );
      os << "\n";
   }
   if( is_main ) {
      os << "int main( ) {\n";
      os << get_identifier_name( *pr.function_overloads.at( "main" ).at( 0 ).declaration ) << "( );\n";
      os << "}\n";
   }
}

#endif //CODE_GEN_H
