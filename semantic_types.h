#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H

#include "lexer_types.h"
#include "parser_types.h"

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <set>
#include <any>

struct program_resources {
   struct inclusion;
   struct visible_operator;
   struct visible_function;

   std::filesystem::path source_path;
   std::vector<char>     source_file;
   std::vector<token>    header_tokens;
   std::vector<token>    program_tokens;
   syntax_tree           tree;

   std::vector<inclusion> inclusions;
   std::unordered_map<std::string_view, std::map<token_type,  visible_operator>> operator_overloads;
   std::unordered_map<std::string_view, std::map<std::size_t, visible_function>> function_overloads;
};

struct program_resources::inclusion {
   bool              access;
   program_resources resources;
};

struct program_resources::visible_operator {
   bool                        access;
   const operator_declaration* declaration;
};

struct program_resources::visible_function {
   bool                        access;
   const function_declaration* declaration;
};

class scope {
   static std::list<scope*> scope_stack;                                   //Nadie toca la lista externamente
   static const program_resources* resources;
   const std::list<scope*>::iterator iter;
   std::unordered_map<std::string_view, token> variables;                 //Tabla de variables

   scope() //Esta interfaz no se muestra al publico
   : iter( scope_stack.emplace( scope_stack.end(), this ) ) {
   }

 public:
    static scope get_global_scope(const program_resources& pr) {
      scope::resources = &pr;
      scope::scope_stack = std::list<scope*>();
      return scope();
    }

   static scope get_new_scope() {
      return scope();
   }

   scope& operator =( const scope& ) = delete;     //Evitar copias de un mismo scope
   scope( const scope& ) = delete;                 //Evitar copias de un mismo scope

   ~scope( ) {                                      //Si me muero yo que mis hijos apunten a mi padre y mi padre apunte a mis hijos y finalmente me borro
      scope_stack.erase( iter );
   }

   // La implementación del scope si acaso se cambia mantienendo la funcionalidad en el caso especifico del proyecto
   // pero me agrada la idea de que las siguientes funciones sean asi.

   enum return_type {                              //Es posible que no se tengan que usar debido al std::any pero no me quiero arriesgar demasiado aun
      NO_DECLARED = -1,
      FUNCTION_MAP,
      VARIABLE
   };

   bool try_declare( const token& t ) {
      return variables.emplace( t.source, t ).second;
   }

   std::pair<std::any, return_type> find_symbol( const token& t ) const {     //Devuelve un tipo diferente segun sea el caso
      // generalmente vamos a usar esta función para buscar una variable siempre, pero hay que identificar tres casos (para el reporte de errores):
      // 1) no existe el símbolo (regresamos nullptr)
      // 2) sí existe el símbolo y sí es variable
      // 3) sí existe el símbolo pero es una función
      auto it = std::find_if(std::make_reverse_iterator(std::next(iter)), scope_stack.rend(),
      [&t](const auto& p) {
         return p->variables.contains(t.source);
      });
      if( it != scope_stack.rend() ) {
         return { (*it)->variables.at(t.source), VARIABLE };
      }
      try {
         const auto& func_map = resources->function_overloads.at( t.source );
         return { func_map, FUNCTION_MAP };
         return { {}, NO_DECLARED };
      } catch( const std::out_of_range& e ) {
         return { {}, NO_DECLARED };
      }
   }

};

const program_resources* scope::resources = nullptr;
std::list<scope*> scope::scope_stack = std::list<scope*>();

#endif //SEMANTIC_TYPES_H
