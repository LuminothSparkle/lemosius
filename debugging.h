#ifndef DEBUGGING_H
#define DEBUGGING_H

#include<string>
#include<string_view>
#include<ostream>

#include"token.h"
#include"syntax_tree.h"
#include"semantic_utilities.h"
#include"compiler_error.h"

namespace debugging {

std::map<token_type,const char*> token_str {
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

std::map<compiler_error_code,const char*> error_str {
    {SYNTAX_ERROR,             "SYNTAX_ERROR"},
    {LEXIC_ERROR,              "LEXIC_ERROR"},
    {SEMANTIC_ERROR,           "SEMANTIC_ERROR"},
    {FILE_ERROR,               "FILE_ERROR"},
    {RECURSIVE_INCLUDE_ERROR,  "RECURSIVE_INCLUDE_ERROR"},
    {OPERATOR_REDEFINED_ERROR, "OPERATOR_REDEFINED_ERROR"},
};

/* Son sobrecarga para imprimir los tipos de datos definidos */

std::ostream& operator <<(std::ostream& os, const token& t) {
    return os << "Token.type:" << token_str[t.type] << "\t"
              << "Token.source: "<< t.source;
}

std::ostream& operator <<(std::ostream& os, token* t) {
    return (t == nullptr ? os << "nullptr" : os << *t);
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
    for(const auto& e : v) {
        os << e << "\n";
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const include_declaration& inc) {
    return os << "include_declaration:\n"
              << "visibility: " << inc.visibility << "\n"
              << "file_name: "  << inc.file_name  << "\n";
}

std::ostream& operator <<(std::ostream& os, const operator_declaration& op) {
    return os << "operator_declaration:\n"
              << "visibility: "   << op.visibility   << "\n"
              << "symbol: "       << op.symbol       << "\n"
              << "position: "     << op.position     << "\n"
              << "asociativity: " << op.asociativity << "\n"
              << "precedence: "   << op.precedence   << "\n"
              << "function: "     << op.function     << "\n";
}

std::ostream& operator <<(std::ostream& os, const program_resources& rscrs) {
    return os << "resources: " << "\n"
              << rscrs.source_file << "\n"
              << rscrs.header_tokens << "\n"
              << rscrs.program_tokens << "\n";
}

std::ostream& operator <<(std::ostream& os, const included_program& inp) {
    os << "included_program:\n"
       << inp.resources << "\n";
    for(const auto& op : inp.operators) {
        os << op.second << "\n";
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const std::pair<operator_declaration,operator_declaration>& ops) {
    return os << "first: "  << ops.first
              << "second: " << ops.second;
}

/* Movi estas funciones para aca de momento en lo que encuentro un lugar apropiado para ellas */
unsigned line_off(const char* first, const char* last) {
    return 1 + std::count(first, last, '\n');
}

unsigned col_off(const char* first, const char* last) {
    return 1 + (last - std::find(std::reverse_iterator(last), std::reverse_iterator(first), '\n').base( ));
}

std::string_view view_off(const char* first, int len, const char* last) {
    return { first, std::size_t(std::find(first, (last - first <= len ? last : first + len), '\n') - first) };

}

std::ostream& error_report(std::ostream& os, const char* first, const char* mid, int len, const char* last) {
    return os << "Error en linea " << line_off(first, mid) << ", columna " << col_off(first, mid) << ":\n"
              << "\t" << view_off(mid, len, last) << "\n"
              << "\t^\n";
}

std::ostream& operator <<(std::ostream& os, const compiler_error& err) {
    os << "error_compiler:\n"
              << "code:"  << error_str[err.code] << "\n"
              << "where:" << err.where           << "\n"
              << "what:"  << err.what            << "\n"
              << "file:"  << err.file_name       << "\n";
    return error_report(os, err.source_file.c_str(), err.where.data( ), err.where.size(), err.source_file.c_str() + err.source_file.size());
}

} //namespace debugging

#endif
