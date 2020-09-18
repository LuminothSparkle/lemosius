#ifndef PARSER_H
#define PARSER_H

#include<utility>
#include<memory>

#include"parser_utilities.h"
#include"syntax_tree.h"
#include"error_handling.h"


token* parse_visibility(token*& t) {
   return (is_visibility(*t) ? t++ : nullptr);
}

syntax_tree parse_header(token*& t) {
    syntax_tree s_tree;    
    while(*t == INCLUDE_K || is_visibility(*t) && *(t + 1) == INCLUDE_K) {
        include_declaration inc_decl;
        inc_decl.visibility = parse_visibility(t);
        match(t, INCLUDE_K);
        inc_decl.file_name = match(t, STRING_L, "Syntax Error: Expecting a file name");
        match(t, SEMICOLON_P);
        s_tree.include_decls.push_back(std::move(inc_decl));
    }
    while(*t == OPERATOR_K || is_visibility(*t) && *(t + 1) == OPERATOR_K) {
        operator_declaration op_decl;
        op_decl.visibility = parse_visibility(t);
        match(t, OPERATOR_K);
        op_decl.symbol = match(t, OPERATOR_L);
        op_decl.position = match(t, is_position, "Syntax Error: Expecting a position");
        if (*op_decl.position == INFIX_K) {
            match(t, LPARENTHESIS_P);
            op_decl.asociativity = match(t, is_asociativity, "Syntax Error: Expecting asociativity");
            op_decl.precedence = match(t, NUMBER_L);
            match(t, RPARENTHESIS_P);
        }
        match(t, AS_K);
        op_decl.function = match(t, IDENTIFIER_L);
        match(t, SEMICOLON_P);
        s_tree.operator_decls.push_back(std::move(op_decl));
    }
    return s_tree;
}

std::unique_ptr<statement> parse_statement(token*& t);
std::unique_ptr<expression> parse_expression(token*& t);

//Hecho para probar la gramatica.
void parse_operand(token*& t) {
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
}

//Hecho para probar la gramatica.
std::unique_ptr<expression> parse_expression(token*& t) {
    parse_operand(t);
    while(is_operator(*t)) {    
        match(t, is_operator);
        parse_operand(t);
    }
    return std::unique_ptr<expression>(new expression);
}

std::unique_ptr<selection_statement> parse_selection(token*& t) {
    std::unique_ptr<selection_statement> sel(new selection_statement);
    match(t, IF_K);
    match(t, LPARENTHESIS_P);
    sel->if_conditions.push_back(parse_expression(t));
    match(t, RPARENTHESIS_P);
    sel->if_bodys.push_back(parse_statement(t));
    while(*t == ELSE_K && *(t + 1) == IF_K) {
        match(t, ELSE_K);
        match(t, IF_K);
        match(t, LPARENTHESIS_P);
        sel->if_conditions.push_back(parse_expression(t));
        match(t, RPARENTHESIS_P);
        sel->if_bodys.push_back(parse_statement(t));
    }
    if(*t == ELSE_K) {
        match(t, ELSE_K);
        sel->else_body = parse_statement(t);
    }
    return sel;
}

std::unique_ptr<variable_declaration> parse_variable(token*& t) {
    std::unique_ptr<variable_declaration> var_decl{new variable_declaration};
    match(t, VAR_K);
    var_decl->variable_name = match(t, IDENTIFIER_L);
    if(*t == ASSIGNMENT_O) {
        match(t, ASSIGNMENT_O);
        var_decl->variable_value = parse_expression(t);
    }
    match(t, SEMICOLON_P);
    return var_decl;
}

std::unique_ptr<return_statement> parse_return(token*& t) {
    std::unique_ptr<return_statement> ret{new return_statement};
    match(t, RETURN_K);
    ret->return_value = parse_expression(t);
    match(t, SEMICOLON_P);
    return ret;
}


std::unique_ptr<group_statement> parse_group(token*& t) {
    std::unique_ptr<group_statement> group{new group_statement};
    match(t, LBRACE_P);
    while(*t != RBRACE_P)
        group->body.push_back(parse_statement(t));
    match(t, RBRACE_P);
    return group;
}

std::unique_ptr<statement> parse_statement(token*& t) {
    std::unique_ptr<expression> exp;
    switch(*t) {
        case LBRACE_P:
            return parse_group(t);
        case IF_K:
            return parse_selection(t);
        case VAR_K:
            return parse_variable(t);
        case RETURN_K:
            return parse_return(t);
        case NUMBER_L:
        case IDENTIFIER_L:
        case OPERATOR_L:
            exp = parse_expression(t);
            match(t, SEMICOLON_P);
            return exp;
        case SEMICOLON_P:
            match(t, SEMICOLON_P);
    }
    return nullptr;
}

syntax_tree& parse_program(token*& t, syntax_tree& s_tree) {
    while(*t == PROC_K || is_visibility(*t) && *(t + 1) == PROC_K) {
        function_definition func_def;    
        func_def.visibility = parse_visibility(t);
        match(t, PROC_K);
        func_def.name = match(t, IDENTIFIER_L);
        match(t, LPARENTHESIS_P);
        while(*t == IDENTIFIER_L) {
            func_def.parameters.push_back(match(t, IDENTIFIER_L));
            if(*t != RPARENTHESIS_P)
                match(t, COMMA_P);
        }
        match(t, RPARENTHESIS_P);
        func_def.body = parse_statement(t);
        s_tree.function_defs.push_back(std::move(func_def));
    }
    return s_tree;
}

#endif
