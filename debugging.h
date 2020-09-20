#ifndef DEBUGGING_H
#define DEBUGGING_H

#include<string>
#include<string_view>
#include<ostream>

#include"token.h"

namespace debugging {

using declaration::include_declaration, declaration::operator_declaration, definition::function_definition;

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

std::ostream& operator <<(std::ostream& os, const token& t) {
    return os << "Token.type:" << token_str[t.type] << "\t"
              << "Token.original: "<< t.original;
}

std::ostream& operator <<(std::ostream& os, token* t) {
    return os << (t == nullptr ? os << "nullptr" : os << *t);
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
              << "file_name: "  << inc.file_name;
}

std::ostream& operator <<(std::ostream& os, const operator_declaration& op) {
    return os << "operator_declaration:\n"
              << "visibility: "   << op.visibility   << "\n"
              << "symbol: "       << op.symbol       << "\n"
              << "position: "     << op.position     << "\n"
              << "asociativity: " << op.asociativity << "\n"
              << "precedence: "   << op.precedence   << "\n"
              << "function: "     << op.function;
}

} //namespace debugging

#endif
