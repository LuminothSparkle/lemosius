#ifndef DEBUGGING_H
#define DEBUGGING_H

#include "lexer.h"
#include "parser.h"
#include "semantic_global.h"

#include <string>
#include <ostream>
#include <iterator>

namespace debugging {

const std::map<token_type, const char*> token_str {
   {UNKNOWN,        "UNKNOWN"},
   {PUBLIC_K,       "PUBLIC_K"},
   {PRIVATE_K,      "PRIVATE_K"},
   {INCLUDE_K,      "INCLUDE_K"},
   {OPERATOR_K,     "OPERATOR_K"},
   {PREFIX_K,       "PREFIX_K"},
   {SUFFIX_K,       "SUFFIX_K"},
   {INFIX_K,        "INFIX_K"},
   {LEFT_K,         "LEFT_K"},
   {RIGHT_K,        "RIGHT_K"},
   {AS_K,           "AS_K"},
   {PROC_K,         "PROC_K"},
   {VAR_K,          "VAR_K"},
   {RETURN_K,       "RETURN_K"},
   {IF_K,           "IF_K"},
   {ELSE_K,         "ELSE_K"},
   {LPARENTHESIS_P, "LPARENTHESIS_P"},
   {RPARENTHESIS_P, "RPARENTHESIS_P"},
   {LBRACE_P,       "LBRACE_P"},
   {RBRACE_P,       "RBRACE_P"},
   {COMMA_P,        "COMMA_P"},
   {SEMICOLON_P,    "SEMICOLON_P"},
   {ASSIGNMENT_O,   "ASSIGNMENT_O"},
   {NUMBER_L,       "NUMBER_L"},
   {STRING_L,       "STRING_L"},
   {OPERATOR_L,     "OPERATOR_L"},
   {IDENTIFIER_L,   "IDENTIFIER_L"},
   {END_OF_INPUT,   "END_OF_INPUT"},
};

} //namespace debugging

// Son sobrecarga para imprimir los tipos de datos definidos
std::ostream& operator <<( std::ostream& os, const token& tok ) {
   return os << "Token.type:"    << debugging::token_str.at( tok.type ) << " " << "Token.source: " << std::string_view( tok );
}

std::ostream& operator <<( std::ostream& os, token* tok ) {
   return ( t == nullptr ? os << "[null_token]" : os << *tok );
}

template<typename T>
std::ostream& operator <<( std::ostream& os, const std::vector<T>& v ) {
   for( const auto& e : v ) {
      os << e << "\n";
   }
   return os;
}

std::ostream& operator <<( std::ostream& os, const std::vector<char>& v ) {
   for( const auto& e : v ) {
      os << e;
   }
   return os;
}

std::ostream& operator <<( std::ostream& os, const include_declaration& inc ) {
   os << "include_declaration:\n";
   os << "access: "     << inc.access    << "\n";
   os << "file_name: "  << inc.file_name << "\n";
   os << "ends include_declaration";
   return os;
}

std::ostream& operator <<( std::ostream& os, const operator_declaration& op ) {
   os << "operator_declaration:\n";
   os << "access: "        << op.access        << "\n";
   os << "symbol: "        << op.symbol        << "\n";
   os << "position: "      << op.position      << "\n";
   os << "associativity: " << op.associativity << "\n";
   os << "precedence: "    << op.precedence    << "\n";
   os << "function: "      << op.function      << "\n";
   os << "ends operator_declaration";
   return os;
}

std::ostream& print( std::ostream&, const statement*, std::size_t );
std::ostream& print( std::ostream&, const sequence_statement*, std::size_t identation );
std::ostream& print( std::ostream&, const if_statement*, std::size_t identation );

std::ostream& print( std::ostream& os, const sequence_statement* seq, std::size_t identation = 0 ) {
   std::string ident( identation * 3, ' ' );
   os << ident << "{\n";
   if( seq ) {
      for( const auto& stmt : seq->body ) {
         print( os, stmt.get( ), identation + 1 );
      }
   }
   os << ident << "}\n";
   return os;
}

std::ostream& print( std::ostream& os, const if_statement* if_stmt, std::size_t identation = 0 ) {
   std::string ident( identation * 3, ' ' );
   if( if_stmt ) {
      for( std::size_t idx = 0; idx < if_stmt->conditions.size( ); ++idx ) {
         os << ident << ( idx > 0 ? "else " : "" ) << "if(" << to_string( if_stmt->conditions[idx] ) << ")\n";
         print( os, if_stmt->bodys[idx].get( ), identation );
      }
      if( if_stmt->else_body ) {
         os << ident << "else\n";
         print( os, if_stmt->else_body.get( ), identation );
      }
   }
   return os;
}

std::ostream& print( std::ostream& os, const statement* stmt, std::size_t identation = 0 ) {
   auto if_ptr  = dynamic_cast<const if_statement*>( stmt );
   auto seq_ptr = dynamic_cast<const sequence_statement*>( stmt );
   if( if_ptr != nullptr ) {
      return print( os, if_ptr, identation );
   } else if( seq_ptr != nullptr ) {
      return print( os, seq_ptr, identation );
   } else {
      std::string ident( identation * 3, ' ' );
      return os << ident << to_string( stmt );
   }
}

std::ostream& operator <<( std::ostream& os, const function_declaration& fd ) {
   os << "function_declaration:\n";
   os << to_string( fd.name ) << "(";
   for( auto it = fd.parameters.begin( ); it != fd.parameters.end( ); std::advance( it, 1 ) ) {
      os << to_string( *it )  << ( std::next( it ) != fd.parameters.end( ) ? "," : "" );
   }
   os << ")\n";
   print( os, fd.body.get( ) ) << "\n";
   os << "ends function_declaration";
   return os;
}

std::ostream& operator <<( std::ostream& os, const syntax_tree& st ) {
   os << "syntax_tree:\n";
   os << "function_tree:\n";
   os << st.functions << "\n";
   os << "ends function_tree\n";
   os << "ends syntax_tree";
   return os;
}

std::ostream& operator <<( std::ostream& os, const visible_operator& vo ) {
   os << "visible_operator:\n";
   os << "public: " << ( vo.access ? "public" : "private" ) << "\n";
   os << "declaration:" << vo.declaration->str( ) << "\n";
   os << "ends visible_operator";
   return os;
}

std::ostream& operator <<( std::ostream& os, const decltype( program_resources::operator_overloads )& oo ) {
   os << "operator_overloads:\n";
   for( const auto& [symbol, overloads] : oo ) {
      os << "symbol:" << symbol << "\n";
      os << "overloads:\n";
      for( const auto& [position, vis_op] : overloads ) {
         os << "position:" << debugging::token_str.at( position ) << "\n"
            << vis_op << "\n";
      }
   }
   os << "ends operator_overloads";
   return os;
}

std::ostream& operator <<( std::ostream& os, const visible_function& vf ) {
   os << "visible_operator:\n";
   os << "public: " << ( vf.access ? "public" : "private" ) << "\n";
   os << "declaration:" << vf.declaration->str( ) << "\n";
   os << "ends visible_operator";
   return os;
}


std::ostream& operator <<( std::ostream& os, const decltype( program_resources::function_overloads )& fo ) {
   os << "function_overloads:\n";
   for( const auto& [name, overloads] : fo ) {
      os << "name:" << name << "\n";
      os << "overloads:\n";
      for( const auto& [arguments, vis_func] : overloads ) {
         os << "arguments:" << arguments << "\n"
            << vis_func << "\n";
      }
   }
   return os << "ends function_overloads";
}

std::ostream& operator <<( std::ostream& os, const program_resources& pr ) {
   os        << "program_resources:\n";
   os        << "source_path: "        << pr.source_path        << "\n";
   os        << "source_file: "        << pr.source_file        << "\n";
   os        << "header_tokens: "      << pr.header_tokens      << "\n";
   os        << "program_tokens: "     << pr.program_tokens     << "\n";
   os        << "tree:"                << pr.tree               << "\n";
   os        << "operator_overloads: " << pr.operator_overloads << "\n";
   os        << "function_overloads: " << pr.function_overloads << "\n";
   return os << "ends program_resources\n";
}

#endif
