#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include<vector>
#include<memory>

#include"token.h"

namespace declaration {

    struct include_declaration {
        token* visibility = nullptr;
        token  file_name;
    };

    struct operator_declaration {
        token* visibility   = nullptr;
        token  symbol;
        token  position;
        token* asociativity = nullptr;
        token* precedence   = nullptr;
        token  function;
    };

}

namespace expression {
    struct expression {
    };
}

namespace statement {

    using expression::expression;

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

    struct selection_statement : statement {
        std::vector<std::unique_ptr<statement>>  if_bodys;
        std::vector<std::unique_ptr<expression>> if_conditions;
        std::unique_ptr<statement>               else_body      = nullptr;
    };

    struct variable_declaration : statement {
        token                       variable_name;
        std::unique_ptr<expression> variable_value = nullptr;
    };

    struct return_statement : statement {
        std::unique_ptr<expression> return_value = nullptr;
    };
    
}

namespace definition {
    
    using statement::sequence_statement;
    
    struct function_definition {
        token*                              visibility = nullptr;
        token                               name;
        std::vector<token>                  parameters;
        std::unique_ptr<sequence_statement> body;
    };
    
}

struct syntax_tree {
    std::vector<declaration::include_declaration>  include_declarations;
    std::vector<declaration::operator_declaration> operator_declarations;
    std::vector<definition::function_definition>   function_definitions;
};

#endif

