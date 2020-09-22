#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include<string>
#include<vector>
#include<string_view>
#include<map>
#include<utility>

#include"syntax_tree.h"
#include"token.h"

/* Template para checar si una declaracion es publica, estoy pensando en cambiar un poco el arbol sintactico debido a estas cosas */
template<typename T>
bool is_public(const T& t) {
	return t.visibility == nullptr || *t.visibility == PUBLIC_K;
};

/* Funcion que ayuda a extraer de una literal string su cadena interna, como actualmente son del estilo "algo" basta con quitar las comillas */
std::string extract_string_literal(const token& t) {
    return std::string(t.source.substr(1,t.source.size()-2));
};

/* Recursos compartidos por las definciones, tokens y demas generados en las primeras etapas del compilador */
/* Se utiliza para no invalidar los apuntadores y string_view a ellos */
struct program_resources {
    std::string source_file;
    std::vector<token> header_tokens;
    std::vector<token> program_tokens;
};

/* Lo declaro para que no haya confusion a la hora de buscar y no tomarse por un token type en el analisis semantico. */
enum operation_position {
    UNKNOWN_POS = -1,
    PREFIX_POS,
    INFIX_POS,
    SUFFIX_POS,
};

/* Funcion auxiliar para transformar de tipo de token a un enum de posicion */
operation_position obtain_position(const token_type& t) {
    switch(t) {
        case INFIX_K:
            return INFIX_POS;
        case PREFIX_K:
            return PREFIX_POS;
        case SUFFIX_K:
            return SUFFIX_POS;
        default:
            break;
    }
    return UNKNOWN_POS;
}

/* La estructura hace referencia a los recursos publicos que ofrece el programa para los demas */
/* De momento solo guarda un map de operadores con su posicion para identificar cuando se vuelven a sobrecargar en la misma posicion */
/* No se si sea eficiente pero es la manera mas facil de implementarlo */
struct included_program {
    std::vector<program_resources> resources;
    std::map<std::pair<std::string_view,operation_position>,operator_declaration> operators;
    //std::multimap<std::string_view,function_definition> functions;
};

/* Realmente los programacion de inclusion y los programas internos son iguales internamente pero quiero hacer la distincion */
using program = included_program;


#endif
