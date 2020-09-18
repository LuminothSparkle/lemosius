#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include<vector>
#include<memory>

#include"token.h"

struct include_declaration {
   token* visibility = nullptr;
   token* file_name;
};

struct operator_declaration {
    token* visibility = nullptr;
    token* symbol;
    token* position;
    token* asociativity = nullptr;
    token* precedence = nullptr;
    token* function;
};

struct statement {
    virtual ~statement( ) = 0;
};
statement::~statement( ) = default;

struct function_definition {
    token* visibility = nullptr;
    token* name;
    std::vector<token*> parameters;
    std::unique_ptr<statement> body;
};

struct group_statement : statement {
    std::vector<std::unique_ptr<statement>> body;
    ~group_statement( ) = default;
};

struct expression : statement {
    ~expression( ) = default;
};

struct selection_statement : statement {
    std::vector<std::unique_ptr<statement>> if_bodys;
    std::vector<std::unique_ptr<expression>> if_conditions;
    std::unique_ptr<statement> else_body = nullptr;
    ~selection_statement( ) = default;
};

struct variable_declaration : statement {
    token* variable_name;
    std::unique_ptr<expression> variable_value = nullptr;
    ~variable_declaration( ) = default;
};

struct return_statement : statement {
    std::unique_ptr<expression> return_value;
    ~return_statement( ) = default;
};

struct syntax_tree {
    std::vector<include_declaration> include_decls;
    std::vector<operator_declaration> operator_decls;
    std::vector<function_definition> function_defs;
};

#endif

