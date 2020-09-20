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
    using declaration::include_declaration, declaration::operator_declaration;
    syntax_tree s_tree;
    
    while(*t == INCLUDE_K || is_visibility(*t) && *(t + 1) == INCLUDE_K) {
        include_declaration inc_decl;
        //Parsea la declaracion de include
        inc_decl.visibility = parse_visibility(t);
        match(t, INCLUDE_K);
        inc_decl.file_name = *match(t, STRING_L, "Syntax Error: Expecting a file name");
        match(t, SEMICOLON_P);
        //Inserta la declaracion de include
        s_tree.include_declarations.push_back(std::move(inc_decl));
    }
    while(*t == OPERATOR_K || is_visibility(*t) && *(t + 1) == OPERATOR_K) {
        operator_declaration op_decl;
        
        //Parsea la declaracion de operador
        op_decl.visibility = parse_visibility(t);
        match(t, OPERATOR_K);
        op_decl.symbol = *match(t, OPERATOR_L);
        op_decl.position = *match(t, is_position, "Syntax Error: Expecting a position");
	    if (op_decl.position == INFIX_K) {
            match(t, LPARENTHESIS_P);
            op_decl.asociativity = match(t, is_asociativity, "Syntax Error: Expecting asociativity");
            op_decl.precedence = match(t, NUMBER_L);
            match(t, RPARENTHESIS_P);
        }
        match(t, AS_K);
        op_decl.function = *match(t, IDENTIFIER_L);
        match(t, SEMICOLON_P);
        //Inserta la declaracion de operador
        s_tree.operator_declarations.push_back(op_decl);
    }
    match(t, END_OF_INPUT);
    
    return s_tree;
}

std::unique_ptr<statement::statement> parse_statement(token*& t);
std::unique_ptr<expression::expression> parse_expression(token*& t);

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
}

//Hecho para probar la gramatica.
std::unique_ptr<expression::expression> parse_expression(token*& t) {
    using expression::expression;
    
    parse_operand(t);
    while(is_operator(*t)) {    
        match(t, is_operator);
        parse_operand(t);
    }
    
    return std::unique_ptr<expression>(nullptr);
}

auto parse_expression_statement(token*& t) {
    using statement::expression_statement, expression::expression;
    auto exp_stmt = std::make_unique<expression_statement>();
    
    if(*t != SEMICOLON_P) {
        exp_stmt->body = parse_expression(t);
    }
    match(t, SEMICOLON_P);
    
    return exp_stmt;
}

auto parse_selection_statement(token*& t) {
    using statement::selection_statement, expression::expression, statement::statement;
    auto sel_stmt = std::make_unique<selection_statement>();
    
    match(t, IF_K);
    match(t, LPARENTHESIS_P);
    sel_stmt->if_conditions.push_back(parse_expression(t));
    match(t, RPARENTHESIS_P);
    sel_stmt->if_bodys.push_back(parse_statement(t));
    while(*t == ELSE_K && *(t + 1) == IF_K) {
        match(t, ELSE_K);
        match(t, IF_K);
        match(t, LPARENTHESIS_P);
        sel_stmt->if_conditions.push_back(parse_expression(t));
        match(t, RPARENTHESIS_P);
        sel_stmt->if_bodys.push_back(parse_statement(t));
    }
    if(*t == ELSE_K) {
        match(t, ELSE_K);
        sel_stmt->else_body = parse_statement(t);
    }
    
    return sel_stmt;
}

auto parse_variable_declaration(token*& t) {
    using statement::variable_declaration, expression::expression;
    auto var_decl = std::make_unique<variable_declaration>();

    match(t, VAR_K);
    var_decl->variable_name = *match(t, IDENTIFIER_L);
    if(*t == ASSIGNMENT_O) {
        match(t, ASSIGNMENT_O);
        var_decl->variable_value = parse_expression(t);
    }
    match(t, SEMICOLON_P);
    
    return var_decl;
}

auto parse_return_statement(token*& t) {
    using statement::return_statement, expression::expression;
    auto ret_stmt = std::make_unique<return_statement>();

    match(t, RETURN_K);
    ret_stmt->return_value = parse_expression(t);
    match(t, SEMICOLON_P);
    
    return ret_stmt;
}


auto parse_sequence_statement(token*& t) {
    using statement::sequence_statement;    
    auto seq_stmt = std::make_unique<sequence_statement>();

    match(t, LBRACE_P);
    while(*t != RBRACE_P)
        seq_stmt->body.push_back(parse_statement(t));
    match(t, RBRACE_P);
    
    return seq_stmt;
}

std::unique_ptr<statement::statement> parse_statement(token*& t) {
    using statement::statement;
    
    switch(*t) {
        case LBRACE_P:
            return parse_sequence_statement(t);
        case IF_K:
            return parse_selection_statement(t);
        case VAR_K:
            return parse_variable_declaration(t);
        case RETURN_K:
            return parse_return_statement(t);
        case NUMBER_L:
        case IDENTIFIER_L:
        case OPERATOR_L:
            return parse_expression_statement(t);
        case SEMICOLON_P:
            match(t, SEMICOLON_P);
    }
    return std::unique_ptr<statement>(nullptr);
}

syntax_tree& parse_program(token*& t, syntax_tree& s_tree) {
    using definition::function_definition;

    while(*t == PROC_K || is_visibility(*t) && *(t + 1) == PROC_K) {
        function_definition func_def;
        
        func_def.visibility = parse_visibility(t);
        match(t, PROC_K);
        func_def.name = *match(t, IDENTIFIER_L);
        match(t, LPARENTHESIS_P);
        while(*t == IDENTIFIER_L) {
            func_def.parameters.push_back(*match(t, IDENTIFIER_L));
            if(*t != RPARENTHESIS_P)
                match(t, COMMA_P);
        }
        match(t, RPARENTHESIS_P);
        func_def.body = parse_sequence_statement(t);
        s_tree.function_definitions.push_back(std::move(func_def));
    }
    match(t, END_OF_INPUT);
    
    return s_tree;
}

#endif
