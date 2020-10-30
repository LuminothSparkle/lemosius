#ifndef DEBUGGING_H
#define DEBUGGING_H

#include"lexer.h"
#include"parser.h"

#include<string>
#include<ostream>
#include<iterator>

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
std::ostream& operator <<( std::ostream& os, const token& t ) {
   return os << "Token.type:"    << debugging::token_str.at( t.type ) << " "
          << "Token.source: " << t.source;
}

std::ostream& operator <<( std::ostream& os, token* t ) {
   return ( t == nullptr ? os << "[null_token]" : os << *t );
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
   return os << "include_declaration:\n"
          << "access: "     << inc.access    << "\n"
          << "file_name: "  << inc.file_name << "\n"
          << "ends include_declaration\n";
}

std::ostream& operator <<( std::ostream& os, const operator_declaration& op ) {
   return os << "operator_declaration:\n"
          << "access: "        << op.access        << "\n"
          << "symbol: "        << op.symbol        << "\n"
          << "position: "      << op.position      << "\n"
          << "associativity: " << op.associativity << "\n"
          << "precedence: "    << op.precedence    << "\n"
          << "function: "      << op.function      << "\n"
          << "ends operator_declaration\n";
}

std::ostream& print( std::ostream& os, const statement* stmt, std::size_t identations );
std::ostream& print( std::ostream& os, const sequence_statement* seq, std::size_t identation );
std::ostream& print( std::ostream& os, const if_statement* seq, std::size_t identation );

std::ostream& print( std::ostream& os, const sequence_statement* seq, std::size_t identation = 0 ) {
   std::string ident( identation * 3, ' ' );
   os << ident << "{\n";
   if( seq ) {
      for( const auto& stmt : seq->body ) {
         print( os, stmt.get(), identation + 1 );
      }
   }
   return os << ident << "}\n";
}

std::ostream& print( std::ostream& os, const if_statement* if_stmt, std::size_t identation = 0 ) {
   std::string ident( identation * 3, ' ' );
   if( if_stmt ) {
      for( std::size_t idx = 0; idx < if_stmt->conditions.size(); ++idx ) {
         os << ident << ( idx > 0 ? "else " : "" ) << "if(" << to_string( if_stmt->conditions[idx] ) << ")\n";
         print( os, if_stmt->bodys[idx].get(), identation );
      }
      if( if_stmt->else_body ) {
         os << ident << "else\n";
         print( os, if_stmt->else_body.get(), identation );
      }
   }
   return os;
}

std::ostream& print( std::ostream& os, const statement* stmt, std::size_t identation = 0 ) {
   auto if_ptr = dynamic_cast<const if_statement*>( stmt );
   auto seq_ptr = dynamic_cast<const sequence_statement*>( stmt );
   if( if_ptr != nullptr ) {
      return print( os, if_ptr, identation );
   } else if( seq_ptr != nullptr ) {
      return print( os, seq_ptr, identation );
   } else {
      std::string ident( identation * 3, ' ' );
      return os << ident << to_string( stmt ) << "\n";
   }
}

std::ostream& operator <<( std::ostream& os, const function_declaration& fd ) {
   os << to_string( fd.name ) << "(";
   for( auto it = fd.parameters.begin(); it != fd.parameters.end(); std::advance( it, 1 ) ) {
      os << to_string( *it )  << ( std::next( it ) != fd.parameters.end() ? "," : "" );
   }
   os << ")\n";
   return print( os, fd.body.get() ) << "\nends function_declaration\n";
}

std::ostream& operator <<( std::ostream& os, const syntax_tree& st ) {
   return os << "syntax_tree:\n"
          << "function_tree:\n"
          << st.functions << "\n"
          << "ends function_tree\n";
}

std::ostream& operator <<( std::ostream& os, const program_resources::visible_operator& vo ) {
   os << "visible_operator:\n";
   os << "public: " << ( vo.access ? "public" : "private" ) << "\n";
   os << "declaration:" << vo.declaration.str() << "\n";
   return os << "ends visible_operator\n";
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
   return os << "ends overloads\n";
}

std::ostream& operator <<( std::ostream& os, const program_resources& pr ) {
   return os << "program_resources:\n"
          << "source_path: "        << pr.source_path        << "\n"
          << "source_file: "        << pr.source_file        << "\n"
          << "header_tokens:\n"
          << pr.header_tokens       << "\n"
          << "ends header_tokens\n"
          << "program_tokens: "
          << pr.program_tokens      << "\n"
          << "ends program_tokens\n"
          << "tree:"
          << pr.tree                << "\n"
          << "ends tree\n"
          << "operator_overloads: " << pr.operator_overloads << "\n"
          //<< "function_overloads: " << pr.function_overloads << "\n";
          << "ends program_resources\n";
}

#endif
