#ifndef PARSER_STATEMENT_H
#define PARSER_STATEMENT_H

#include"lexer.h"
#include"parser_expression.h"
#include"parser_utilities.h"

#include<memory>
#include<vector>

struct statement {
    virtual std::string str() const = 0;
};

struct sequence_statement : statement {
    std::vector<std::unique_ptr<statement>> body;

    std::string str() const {
        return "{...}";
    }
};

struct expression_statement : statement {
    std::unique_ptr<expression> body;

    std::string str() const {
        return to_string(body,"") + ";";
    }
};

struct if_statement : statement {
    std::vector<std::unique_ptr<sequence_statement>>  bodys;
    std::vector<std::unique_ptr<expression>>          conditions;
    std::unique_ptr<sequence_statement>               else_body      = nullptr;

    std::string str() const {
        std::string res;
        for(std::size_t idx = 0; idx < conditions.size(); ++idx) {
            res.append((idx > 0 ? "else " : "") + to_string(conditions[idx],"if(",")") + to_string(bodys[idx]," ","\n"));
        }
        res.append(to_string(else_body,"else ","\n"));
        return res;
    }
};

struct var_statement : statement {
    token                       name;
    std::unique_ptr<expression> value = nullptr;

    std::string str() const {
        return to_string(name,"var ") + to_string(value," := ") + ";";
    }
};

struct return_statement : statement {
    std::unique_ptr<expression> return_value = nullptr;

    std::string str() const {
        return "return" + to_string(return_value," ") + ";";
    }
};

std::unique_ptr<statement> parse_statement(token*& t, const operator_map& opm);

auto parse_sequence_statement(token*& t, const operator_map& opm) {
    auto seq_stmt = std::make_unique<sequence_statement>();

    match(t, LBRACE_P, "Expecting {");
    while(*t != RBRACE_P)
        seq_stmt->body.push_back( parse_statement(t,opm) );
    match(t, RBRACE_P, "Expecting }");

    return seq_stmt;
}

auto parse_if_statement(token*& t, const operator_map& opm) {
    auto if_stmt = std::make_unique<if_statement>();

    match(t, IF_K, "Expecting if");
    match(t, LPARENTHESIS_P, "Expecting (");
    if_stmt->conditions.push_back(parse_expression(t,opm));
    match(t, RPARENTHESIS_P, "Expecting )");
    if_stmt->bodys.push_back(parse_sequence_statement(t,opm));
    while(*t == ELSE_K && *(t + 1) == IF_K) {
        match(t, ELSE_K, "Expecting else");
        match(t, IF_K, "Expecting if");
        match(t, LPARENTHESIS_P, "Expecting (");
        if_stmt->conditions.push_back(parse_expression(t,opm));
        match(t, RPARENTHESIS_P, "Expecting )");
        if_stmt->bodys.push_back(parse_sequence_statement(t,opm));
    }
    if(optional_match(t,ELSE_K) != nullptr) {
        if_stmt->else_body = parse_sequence_statement(t,opm);
    }

    return if_stmt;
}

auto parse_var_statement(token*& t, const operator_map& opm) {
    auto var_decl = std::make_unique<var_statement>();

    match(t, VAR_K, "Expecting var");
    var_decl->name = *match(t, IDENTIFIER_L, "Expecting an identifier");
    if(optional_match(t,ASSIGNMENT_O) != nullptr) {
        var_decl->value = parse_expression(t,opm);
    }
    match(t, SEMICOLON_P, "Expecting ;");

    return var_decl;
}

auto parse_return_statement(token*& t, const operator_map& opm) {
    auto ret_stmt = std::make_unique<return_statement>();

    match(t, RETURN_K, "Expecting return");
    ret_stmt->return_value = parse_expression(t,opm);
    match(t, SEMICOLON_P, "Expecting ;");

    return ret_stmt;
}

auto parse_expression_statement(token*& t, const operator_map& opm) {
    auto exp_stmt = std::make_unique<expression_statement>();

    if(*t != SEMICOLON_P) {
        exp_stmt->body = parse_expression(t,opm);
    }
    match(t, SEMICOLON_P, "Expecting ;");

    return exp_stmt;
}

std::unique_ptr<statement> parse_statement(token*& t, const operator_map& opm) {
    switch(*t) {
        case LBRACE_P:
            return parse_sequence_statement(t,opm);
        case IF_K:
            return parse_if_statement(t,opm);
        case VAR_K:
            return parse_var_statement(t,opm);
        case RETURN_K:
            return parse_return_statement(t,opm);
        default:
            return parse_expression_statement(t,opm);
    }
}

#endif // PARSER_STATEMENT_H
