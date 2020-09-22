#ifndef TOKEN_H
#define TOKEN_H

#include<string>
#include<string_view>

/* _K de keyword */
/* _P de puntuacion */
/* _L de literal */
/* _O de operador */
/* UNKNOWN y END_OF_INPUT son tokens especiales que deben ir en la primera y ultima posicion respectivamente */

enum token_type {
    UNKNOWN = -1,
    PUBLIC_K,
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
    LPARENTHESIS_P,
    RPARENTHESIS_P,
    LBRACE_P,
    RBRACE_P,
    COMMA_P,
    SEMICOLON_P,
    ASSIGNMENT_O,
    NUMBER_L,
    STRING_L,
    OPERATOR_L,
    IDENTIFIER_L,
    END_OF_INPUT
};

struct token {
    token_type type;          //Indica el tipo de token
    std::string_view source;  //Indica la cadena del token en la fuente original
    
    token_type peek() const {
        return type;
    }

    operator std::string() const {
        return std::string(source);
    }

    operator std::string_view() const {
        return source;
    }

    operator const char*() const {
        return source.data();
    }

    operator token_type() const {
        return type;
    }

};

#endif

