#ifndef PARSER_H
#define PARSER_H
#include<utility>
#include"parser_utilities.h"

void visibility_especifier(it_tok& t) {
    if(*t == PRIVATE_K) {
        match(t,PRIVATE_K);
    }
    else if(*t == PUBLIC_K) {
        match(t,PUBLIC_K);
    }
}

void include_declaration(it_tok& t) {
    match(t,INCLUDE_K);
    match(t,STRING_L);
    match(t,SEMICOLON_P);
}

void asociativity_especifier(it_tok& t) {
    if(*t == LEFT_K) {
        match(t,LEFT_K);
    }
    else if(*t == RIGHT_K) {
        match(t,RIGHT_K);
    }
    else {
        throw std::make_pair(*t,"Syntax error");
    }
}

void precedence_especifier(it_tok& t) {
    match(t,NUMBER_L);
}

void position_especifier(it_tok& t) {
    if(*t == PREFIX_K) {
        match(t,PREFIX_K);
    }
    else if(*t == SUFFIX_K) {
        match(t,SUFFIX_K);
    }
    else if(*t == INFIX_K) {
        match(t,INFIX_K);
        match(t,LPARENTHESIS_P);
        asociativity_especifier(t);
        precedence_especifier(t);
        match(t,RPARENTHESIS_P);
    }
    else {
        throw std::make_pair(*t,"Syntax error");
    }
}

void operator_declaration(it_tok& t) {
    match(t,OPERATOR_K);
    match(t,OPERATOR_L);
    position_especifier(t);
    match(t,AS_K);
    match(t,IDENTIFIER_L);
    match(t,SEMICOLON_P);
}

void include_header(it_tok& t) {
    if(*t == PRIVATE_K || *t == PUBLIC_K) {
        visibility_especifier(t);
        include_declaration(t);
        include_header(t);
    }
    else if(*t == INCLUDE_K) {
        include_declaration(t);
        include_header(t);
    }
}

void operator_header(it_tok& t) {
    if(*t == PRIVATE_K || *t == PUBLIC_K) {
        visibility_especifier(t);
        operator_declaration(t);
        operator_header(t);
    }
    else if(*t == OPERATOR_K) {
        operator_declaration(t);
        operator_header(t);
    }
}

void header(it_tok& t) {    
    include_header(t);
    operator_header(t);
}

void variable_definition(it_tok& t) {
    //Falta por implementar
}

void statements(it_tok& t) {
    //Falta por implementar
}

void procedure_definition(it_tok& t) {    
    if(*t == VAR_K) {
        variable_definition(t);
        procedure_definition(t);
    }
    else if(*t != RBRACE_P) {
        statements(t);
        procedure_definition(t);
    }
}

void program(it_tok& t) {
    if(*t == PRIVATE_K || *t == PUBLIC_K) {
        visibility_especifier(t);
        match(t,LBRACE_P);
        procedure_definition(t);
        match(t,RBRACE_P);
    }
    else if(*t == PROC_K) {
        match(t,LBRACE_P);
        procedure_definition(t);
        match(t,RBRACE_P);
    }

}

#endif
