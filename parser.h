#ifndef PARSER_H
#define PARSER_H

#include"lexer.h"
#include"parser_expression.h"
#include"parser_statement.h"
#include"parser_utilities.h"

#include<utility>
#include<memory>
#include<vector>

struct include_declaration {
    token* access     = nullptr;
    token  file_name;

    std::string str() const {
        std::string decl = to_string(access,""," ");
        decl.append("include ");
        decl.append(file_name.str( ));
        decl.append(";");
        return decl;
    }
};

struct operator_declaration {
    token* access       = nullptr;
    token  symbol;
    token  position;
    token* asociativity = nullptr;
    token* precedence   = nullptr;
    token  function;

    std::string str() const {
        std::string decl = to_string(access,""," ");
        decl.append("operator ");
        decl.append(symbol.str( ));
        decl.append(position.str( ));
        decl.append(position == INFIX_K ? to_string(asociativity,"(") + to_string(precedence," ",")") : "");
        decl.append(" as ");
        decl.append(function.str( ));
        decl.append(";");
        return decl;
    }
};

struct function_declaration {
    token*                              access     = nullptr;
    token                               name;
    std::vector<token>                  parameters;
    std::unique_ptr<sequence_statement> body;

    std::string str() const {
        //...
        return "";
    }
};

struct syntax_tree {
    std::vector<include_declaration>  includes;
    std::vector<operator_declaration> operators;
    std::vector<function_declaration>  functions;
};

syntax_tree parse_header(token*& t) {
    syntax_tree st;
    while( *t == INCLUDE_K || (is_access(*t) && *(t + 1) == INCLUDE_K) ) {
        include_declaration inc_decl;
        // Parsea la declaracion de include
        inc_decl.access = optional_match(t,is_access);
        match(t, INCLUDE_K);
        inc_decl.file_name = *match(t, STRING_L, "Expecting a string literal");
        match(t, SEMICOLON_P, "Expecting ;");
        // Inserta la declaracion de include
        st.includes.push_back(std::move(inc_decl));
    }
    while( *t == OPERATOR_K || (is_access(*t) && *(t + 1) == OPERATOR_K) ) {
        operator_declaration op_decl;
        // Parsea la declaracion de operador
        op_decl.access   = optional_match(t, is_access);
        match(t, OPERATOR_K);
        op_decl.symbol   = *match(t, OPERATOR_L, "Expecting an operator");
        op_decl.position = *match(t, is_position, "Expecting infix, prefix or suffix");
        if(op_decl.position == INFIX_K) {
            match(t, LPARENTHESIS_P, "Expecting (");
            op_decl.asociativity = match(t, is_asociativity, "Expecting left or right");
            op_decl.precedence   = match(t, NUMBER_L, "Expecting a number literal");
            match(t, RPARENTHESIS_P, "Expecting )");
        }
        match(t, AS_K, "Expecting as");
        op_decl.function = *match(t, IDENTIFIER_L, "Expecting an identifier");
        match(t, SEMICOLON_P, "Expecting ;");
        // Inserta la declaracion de operador
        st.operators.push_back(std::move(op_decl));
    }
    match(t, END_OF_INPUT);
    return st;
}

syntax_tree& parse_program(token*& t, syntax_tree& st) {
    while( *t == PROC_K || (is_access(*t) && *(t + 1) == PROC_K) ) {
        function_declaration func_def;
        // Parsea la definicion de la funcion
        func_def.access = optional_match(t,is_access);
        match(t, PROC_K);
        func_def.name = *match(t, IDENTIFIER_L);
        match(t, LPARENTHESIS_P);
        while(*t == IDENTIFIER_L) {
            func_def.parameters.push_back(*match(t, IDENTIFIER_L));
            if(*t != RPARENTHESIS_P)
                match(t, COMMA_P);
        }
        match(t, RPARENTHESIS_P);
        // Parsea las demas sentencias de la funcion
        func_def.body = parse_sequence_statement(t);
        // Inserta la funcion al arbol.
        st.functions.push_back(std::move(func_def));
    }
    match(t, END_OF_INPUT);
    return st;
}

#endif
