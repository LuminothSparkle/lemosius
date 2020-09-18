#ifndef TOKEN_H
#define TOKEN_H

#include<string>
#include<string_view>

enum token_type {
    UNKNOWN = -1,
    PUBLIC_K,            // _K de keyword       (además, por la definición de arriba, PUBLIC_K comienza en 0)
    PRIVATE_K,
    INCLUDE_K,
    OPERATOR_K,
    PREFIX_K,
    SUFFIX_K,
    INFIX_K,
    LEFT_K,
    RIGHT_K,
    AS_K,
    PROC_K,
    VAR_K,
    RETURN_K,
    IF_K,
    ELSE_K,
    LPARENTHESIS_P,      // _P de puntuación
    RPARENTHESIS_P,
    LBRACE_P,
    RBRACE_P,
    COMMA_P,
    SEMICOLON_P,
    ASSIGNMENT_O,        // _O de operador predefinido
    NUMBER_L,            // _L de literal
    STRING_L,
    OPERATOR_L,
    IDENTIFIER_L,
    END_OF_INPUT         // sin sufijo (es especial)
};

//Considere crear la estructura para facilitar las comparaciones en el parser y cuando queramos imprimirlo como cadena y siento que se ven mejor pero no se que opines.
struct token {
    token_type type;
    std::string_view original;
    
    token_type peek() const {
        return type;
    }

    operator std::string() const {
        return std::string(original);
    }

    operator const char*() const {
        return original.data();
    }

    operator token_type() const {
        return type;
    }

};

#endif

