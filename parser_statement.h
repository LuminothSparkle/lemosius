#ifndef PARSER_STATEMENT_H
#define PARSER_STATEMENT_H

#include "lexer.h"
#include "parser_expression.h"
#include "parser_utilities.h"
#include <memory>
#include <vector>

struct statement {
    virtual ~statement( ) = 0;
};
statement::~statement( ) = default;

struct sequence_statement : statement {
    std::vector<std::unique_ptr<statement>> body;
};

struct expression_statement : statement {
    std::unique_ptr<expression> body;
};

struct if_statement : statement {
    std::vector<std::unique_ptr<statement>>  bodys;
    std::vector<std::unique_ptr<expression>> conditions;
    std::unique_ptr<statement>               else_body      = nullptr;
};

struct var_statement : statement {
    token                       name;
    std::unique_ptr<expression> value = nullptr;
};

struct return_statement : statement {
    std::unique_ptr<expression> return_value = nullptr;
};


std::unique_ptr<statement> parse_statement(token*& t);

auto parse_expression_statement(token*& t) {
    auto exp_stmt = std::make_unique<expression_statement>();

    if(*t != SEMICOLON_P) {
        exp_stmt->body = parse_expression(t);
    }
    match(t, SEMICOLON_P);

    return exp_stmt;
}

auto parse_sequence_statement(token*& t) {
    auto seq_stmt = std::make_unique<sequence_statement>();

    match(t, LBRACE_P);
    while(*t != RBRACE_P)
        seq_stmt->body.push_back(parse_statement(t));
    match(t, RBRACE_P);

    return seq_stmt;
}

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

auto parse_var_statement(token*& t) {
    auto var_decl = std::make_unique<var_statement>();

    match(t, VAR_K);
    var_decl->name = *match(t, IDENTIFIER_L);
    if(optional_match(t,ASSIGNMENT_O) != nullptr) {
        var_decl->value = parse_expression(t);
    }
    match(t, SEMICOLON_P);

    return var_decl;
} //end function parse_var_statement

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
            return parse_var_statement(t);
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

#endif
