#ifndef PARSER_H
#define PARSER_H

#include"lexer.h"
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
}; // end struct include_declaration

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
}; // end struct operator_declaration

struct expression {
}; // end struct expression

struct statement {
    virtual ~statement( ) = 0;
}; // end struct statament
statement::~statement( ) = default;

struct sequence_statement : statement {
    std::vector<std::unique_ptr<statement>> body;
}; // end struct sequence_statement

struct expression_statement : statement {
    std::unique_ptr<expression> body;
}; // end struct expression_statement

struct if_statement : statement {
    std::vector<std::unique_ptr<statement>>  bodys;
    std::vector<std::unique_ptr<expression>> conditions;
    std::unique_ptr<statement>               else_body      = nullptr;
}; // end struct if_statement

struct variable_declaration : statement {
    token                       name;
    std::unique_ptr<expression> value = nullptr;
}; // end struct variable_declaration

struct return_statement : statement {
    std::unique_ptr<expression> return_value = nullptr;
}; // end struct return_statement

struct function_definition {
    token*                              access     = nullptr;
    token                               name;
    std::vector<token>                  parameters;
    std::unique_ptr<sequence_statement> body;
}; // end struct function_definition

struct syntax_tree {
    std::vector<include_declaration>  includes;
    std::vector<operator_declaration> operators;
    std::vector<function_definition>  functions;
}; // end struct syntax_tree

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
} // end function parse_header

std::unique_ptr<statement> parse_statement(token*& t);
std::unique_ptr<expression> parse_expression(token*& t);

//Hecho para probar la gramatica.
auto parse_operand(token*& t) {
    if(*t == IDENTIFIER_L && *(t + 1) == LPARENTHESIS_P) {
        match(t, IDENTIFIER_L);
        match(t, LPARENTHESIS_P);
        while(*t != RPARENTHESIS_P) {
            parse_expression(t);
            if(*t != RPARENTHESIS_P)
                match(t, COMMA_P);
        }
        match(t, RPARENTHESIS_P);
    }
    else {
        match(t, is_literal);
    }
} // end function parse_operand

//Hecho para probar la gramatica.
std::unique_ptr<expression> parse_expression(token*& t) {
    parse_operand(t);
    while(is_operator(*t)) {
        match(t, is_operator);
        parse_operand(t);
    }

    return std::unique_ptr<expression>(nullptr);
} //end function parse_expression

auto parse_expression_statement(token*& t) {
    auto exp_stmt = std::make_unique<expression_statement>();

    if(*t != SEMICOLON_P) {
        exp_stmt->body = parse_expression(t);
    }
    match(t, SEMICOLON_P);

    return exp_stmt;
} // end function parse_expression_statement

auto parse_sequence_statement(token*& t) {
    auto seq_stmt = std::make_unique<sequence_statement>();

    match(t, LBRACE_P);
    while(*t != RBRACE_P)
        seq_stmt->body.push_back(parse_statement(t));
    match(t, RBRACE_P);

    return seq_stmt;
} // end function parse_sequence_statement

auto parse_if_statement(token*& t) {
    auto if_stmt = std::make_unique<if_statement>();

    match(t, IF_K);
    match(t, LPARENTHESIS_P);
    if_stmt->conditions.push_back(parse_expression(t));
    match(t, RPARENTHESIS_P);
    if_stmt->bodys.push_back(parse_sequence_statement(t));
    while(*t == ELSE_K && *(t + 1) == IF_K) {
        match(t, ELSE_K);
        match(t, IF_K);
        match(t, LPARENTHESIS_P);
        if_stmt->conditions.push_back(parse_expression(t));
        match(t, RPARENTHESIS_P);
        if_stmt->bodys.push_back(parse_sequence_statement(t));
    }
    if(optional_match(t,ELSE_K) != nullptr) {
        if_stmt->else_body = parse_sequence_statement(t);
    }

    return if_stmt;
} // end function parse_if_statement

auto parse_variable_declaration(token*& t) {
    auto var_decl = std::make_unique<variable_declaration>();

    match(t, VAR_K);
    var_decl->name = *match(t, IDENTIFIER_L);
    if(optional_match(t,ASSIGNMENT_O) != nullptr) {
        var_decl->value = parse_expression(t);
    }
    match(t, SEMICOLON_P);

    return var_decl;
} //end function parse_variable_declaration

auto parse_return_statement(token*& t) {
    auto ret_stmt = std::make_unique<return_statement>();

    match(t, RETURN_K);
    ret_stmt->return_value = parse_expression(t);
    match(t, SEMICOLON_P);

    return ret_stmt;
} // end function parse_return_statement

std::unique_ptr<statement> parse_statement(token*& t) {
    switch(*t) {
        case LBRACE_P:
            return parse_sequence_statement(t);
        case IF_K:
            return parse_if_statement(t);
        case VAR_K:
            return parse_variable_declaration(t);
        case RETURN_K:
            return parse_return_statement(t);
        case NUMBER_L:
        case IDENTIFIER_L:
        case OPERATOR_L:
            return parse_expression_statement(t);
        default:
            match(t, SEMICOLON_P);
    }
    return std::unique_ptr<statement>(nullptr);
} // end function parse_statement

syntax_tree& parse_program(token*& t, syntax_tree& st) {
    while( *t == PROC_K || (is_access(*t) && *(t + 1) == PROC_K) ) {
        function_definition func_def;
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
} // end function parse_program

#endif
