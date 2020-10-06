#ifndef PARSER_H
#define PARSER_H

#include"lexer.h"
#include"parser_utilities.h"
#include"string_utilities.h"
#include"parser_statement.h"

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
};

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
};

struct function_declaration {
    token*                              access     = nullptr;
    token                               name;
    std::vector<token>                  parameters;
    std::unique_ptr<sequence_statement> body;

    std::string str() {
        std::string decl = to_string(access,""," ");
        decl.append("proc ");
        decl.append(name.str( ));
        decl.append( transform_join(parameters,
        [](const token& t){
            return t.str();
        },",","(",");") );
        return decl;
    }
};

struct syntax_tree {
    struct header_declarations {
        std::vector<include_declaration>  includes;
        std::vector<operator_declaration> operators;
    };
    header_declarations               header;
    std::vector<function_declaration> functions;
};

struct operator_map {
   operator_map(const std::vector<operator_declaration>& ops) {
      // inicializar la cosa
   }

   /*bool is_cosa(cosa) const {

   }

   std::uint64_t precedence(cosa) const {       // 64 aqui para que el algoritmo de precedence climbing pueda sumar sin miedo

   }
   */
};

auto parse_header(token*& t) {
    syntax_tree::header_declarations hd;
    auto& [incs, ops] = hd;

    while( *t == INCLUDE_K || (is_access(*t) && *(t + 1) == INCLUDE_K) ) {
        include_declaration inc_decl;
        // Parsea la declaracion de include
        inc_decl.access = optional_match( t, is_access );
        match( t, INCLUDE_K );
        inc_decl.file_name = *match( t, STRING_L, "Expecting a string literal" );
        match( t, SEMICOLON_P, "Expecting ;" );
        // Inserta la declaracion de include
        incs.push_back( std::move(inc_decl) );
    }
    while( *t == OPERATOR_K || (is_access(*t) && *(t + 1) == OPERATOR_K) ) {
        operator_declaration op;
        // Parsea la declaracion de operador
        op.access = optional_match( t, is_access );
        match( t, OPERATOR_K );
        op.symbol   = *match( t, OPERATOR_L, "Expecting an operator" );
        op.position = *match( t, is_position, "Expecting infix, prefix or suffix" );
        if(op.position == INFIX_K) {
            match( t, LPARENTHESIS_P, "Expecting (" );
            op.asociativity = match( t, is_asociativity, "Expecting left or right" );
            op.precedence   = match( t, NUMBER_L, "Expecting a number literal" );
            match( t, RPARENTHESIS_P, "Expecting )" );
        }
        match( t, AS_K, "Expecting as" );
        op.function = *match( t, IDENTIFIER_L, "Expecting an identifier" );
        match( t, SEMICOLON_P, "Expecting ;" );
        // Inserta la declaracion de operador
        ops.push_back( std::move(op) );
    }
    match( t, END_OF_INPUT, "Expecting a function declaration");

    return hd;
}

auto parse_program(token*& t, const operator_map& opm) {       // el tipo real aquí, const auto& en expression.h
    std::vector<function_declaration> funcs;

    while( *t == PROC_K || (is_access(*t) && *(t + 1) == PROC_K) ) {
        function_declaration func_def;
        // Parsea la definicion de la funcion
        func_def.access = optional_match( t, is_access );
        match( t, PROC_K );
        func_def.name = *match( t, IDENTIFIER_L );
        match( t, LPARENTHESIS_P );
        while(*t == IDENTIFIER_L) {
            func_def.parameters.push_back( *match(t, IDENTIFIER_L) );
            if(*t != RPARENTHESIS_P) {
                match( t, COMMA_P );
            }
        }
        match( t, RPARENTHESIS_P );
        // Parsea las demas sentencias de la funcion
        func_def.body = parse_sequence_statement(t);
        // Inserta la funcion al arbol.
        funcs.push_back( std::move(func_def) );
    }
    match( t, END_OF_INPUT );

    return funcs;
}

#endif
