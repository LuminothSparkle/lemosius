#ifndef PARSER_H
#define PARSER_H
#include <utility>
#include "parser_utilities_.h"

struct include {
   token* visibilidad;     // si tiene * es que son opcionales
   token archivo;          // los que no son * tienen una copia completa del token (consumirá un poco más d ememoria, pero te evitas una desreferencia; salvo que tengas millones de includes, no notarás la diferencia=
};

struct operador {
   token* visibilidad;
   token simbolo;
   token posicion;
   token* asociatividad;
   token* precedencia;
   token funcion;
};

struct sentencia {
   virtual ~sentencia( ) = 0;
};
// struct sentencia_var : sentencia { ... }
// struct sentencia_asignacion : sentencia { ... };
// etc...

struct funcion {
   token* publico;
   token nombre;
   std::vector<token> parametros;
   std::vector<std::unique_ptr<sentencia>> sentencias;      // éstas se crearán con new eltiporealdelasentencia(las cosas) y se guardarán aquí
};

struct arbol_sintactico {
   std::vector<include> includes;
   std::vector<operador> operadores;
   std::vector<funcion> funciones;
};

token* parse_visibility(token*& t) {
   return (is_visibility(*t) ? t : nullptr);
}

arbol_sintactico parser1(token*& t) {
   arbol_sintactico a;
   while (*t == INCLUDE_K || is_visibility(*t) && *(t + 1) == INCLUDE_K) {
      auto vis = parse_visibility(t);
      match(t, INCLUDE_K);
      auto archivo = match(t, STRING_L);
      match(t, SEMICOLON_P);
      a.includes.push_back(include{vis, *archivo});
   }

   while (*t == OPERATOR_K || is_visibility(*t) && *(t + 1) == OPERATOR_K) {
      auto vis = parse_visibility(t);
      match(t, OPERATOR_K);
      auto simbolo = match(t, OPERATOR_L);
      auto posicion = match(t, is_position);
      token *asociatividad = nullptr, *precedencia = nullptr;
      if (*posicion == INFIX_K) {
         match(t, LPARENTHESIS_P);
         asociatividad = match(t, is_precedence);
         precedencia = match(t, NUMBER_L);
         match(t, RPARENTHESIS_P);
      }
      match(t, AS_K);
      auto funcion = match(t, IDENTIFIER_L);
      match(t, SEMICOLON_P);
      a.operadores.push_back(operador{vis, *simbolo, *posicion, asociatividad, precedencia, *funcion});
   }

   return a;
}

arbol_sintactico& parser2(token*& t, arbol_sintactico& a/*, algo más */) {
   // llenar a.funciones                                  ^^ una tabla que incluya la información de precedencia y asociatividad de los operadores
   return a;
}

#endif
