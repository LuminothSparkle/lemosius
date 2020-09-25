#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include"lexer.h"

#include<string>

// Template para checar si una declaracion es publica, estoy pensando en cambiar un poco el arbol sintactico debido a estas cosas
template<typename T>
bool is_public(const T& t) {
    return t.access == nullptr || *t.access == PUBLIC_K;
};

// Funcion que ayuda a extraer de una literal string su cadena interna, como actualmente son del estilo "algo" basta con quitar las comillas
std::string extract_string_literal(const token& t) {            // ahorita el valor de retorno podría ser un std::string_view
    return std::string(t.source.substr(1,t.source.size()-2));   // eventualmente sí tendrá que ser un std::string porque es necesario el unquote
};

#endif
