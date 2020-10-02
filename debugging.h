#ifndef DEBUGGING_H
#define DEBUGGING_H

#include"lexer.h"
#include"parser.h"

#include<string>
#include<ostream>

namespace debugging {

const std::map<token_type,const char*> token_str {
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
std::ostream& operator <<(std::ostream& os, const token& t) {
    return os << "Token.type:"    << debugging::token_str.at(t.type) << "\t"
              << "Token.source: " << t.source;
}

std::ostream& operator <<(std::ostream& os, token* t) {
    return (t == nullptr ? os << "[null_token]" : os << *t);
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
    for(const auto& e : v) {
        os << e << "\n";
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const std::vector<char>& v) {
    for(const auto& e : v) {
        os << e;
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const include_declaration& inc) {
    return os << "include_declaration:\n"
              << "access: "     << inc.access    << "\n"
              << "file_name: "  << inc.file_name << "\n";
}

std::ostream& operator <<(std::ostream& os, const operator_declaration& op) {
    return os << "operator_declaration:\n"
              << "access: "       << op.access       << "\n"
              << "symbol: "       << op.symbol       << "\n"
              << "position: "     << op.position     << "\n"
              << "asociativity: " << op.asociativity << "\n"
              << "precedence: "   << op.precedence   << "\n"
              << "function: "     << op.function     << "\n";
}

std::ostream& operator <<(std::ostream& os, const program_resources& pr) {
    return os << "program_resources:\n"
              << "source_path: "    << pr.source_path    << "\n"
              << "source_file: "    << pr.source_file    << "\n"
              << "header_tokens: "  << pr.header_tokens  << "\n"
              << "program_tokens: " << pr.program_tokens << "\n";
}


#endif
