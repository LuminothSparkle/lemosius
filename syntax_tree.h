#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include<vector>
#include<memory>

#include"token.h"

struct include_declaration {
    std::string_view source;               //Indica la cadena que representa la declaracion en la fuente original
    token*           visibility = nullptr;
    token            file_name;
};

struct operator_declaration {
    std::string_view source;                 //Indica la cadena que representa la declaracion en la fuente original
    token*           visibility   = nullptr;
    token            symbol;
    token            position;
    token*           asociativity = nullptr;
    token*           precedence   = nullptr;
    token            function;
};

struct expression {
    std::string_view original_string; //Indica la cadena que representa la expresion en la fuente original
};

struct statement {
    std::string_view source; //Indica la cadena que representa la sentencia en la fuente original
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

struct variable_declaration : statement {
    token                       name;
    std::unique_ptr<expression> value = nullptr;
};

struct return_statement : statement {
    std::unique_ptr<expression> return_value = nullptr;
};

struct function_definition {
    std::string_view                    source;               //Indica la cadena que representa la definicion hasta el inicio de las sentencias en la fuente original
    token*                              visibility = nullptr;
    token                               name;
    std::vector<token>                  parameters;
    std::unique_ptr<sequence_statement> body;
};

struct syntax_tree {
    std::vector<include_declaration>  includes;
    std::vector<operator_declaration> operators;
    std::vector<function_definition>  functions;
};

#endif

