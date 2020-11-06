#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H

#include "lexer.h"
#include "parser.h"

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <set>
#include <variant>

struct program_resources {
   struct inclusion;
   struct visible_operator;
   struct visible_function;
   using function_overload_set = std::map<std::size_t, visible_function>;
   using operator_overload_set = std::map<token_type, visible_operator>;

   std::filesystem::path source_path;
   std::vector<char>     source_file;
   std::vector<token>    header_tokens;
   std::vector<token>    program_tokens;
   syntax_tree           tree;

   std::vector<inclusion> inclusions;
   std::unordered_map<std::string_view, operator_overload_set> operator_overloads;
   std::unordered_map<std::string_view, function_overload_set> function_overloads;
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

class symbol {
   std::variant<const token*, const program_resources::function_overload_set*> data;
 public:
   symbol() = default;
   template<typename... Args>
   symbol( const Args&... args ) : data( args... ) {}
   template<typename... Args>
   symbol( Args&&... args ) : data( std::forward<Args>( args )... ) {}
   auto get_function_overload_set() {
      auto ptr = std::get_if<const program_resources::function_overload_set*>( &data );
      return ptr != nullptr ? *ptr : nullptr;
   }
   auto get_variable() {
      auto ptr = std::get_if<const token*>( &data );
      return ptr != nullptr ? *ptr : nullptr;
   }
};

class scope {
   std::unordered_map<std::string_view, const token*> variables;                //Tabla de variables
 public:
   bool try_declare( const token& t ) {
      return variables.emplace( t.source, &t ).second;
   }

   const token* find_variable( const token& t ) const {
      auto it = variables.find( t.source );
      return it != variables.end() ? it->second : nullptr;
   }

};

class scope_stack {
   std::list<scope> scope_list;
   const program_resources& resources;
 public:
   scope_stack( const program_resources& pr )
      : resources( pr ) {
   }
   void push() {
      scope_list.emplace_back();
   }
   void pop() {
      scope_list.pop_back();
   }
   scope& top() {
      return scope_list.back();
   }
   const scope& top() const {
      return scope_list.back();
   }

   symbol find_symbol( const token& t ) const {     //Devuelve un tipo diferente segun sea el caso
      // generalmente vamos a usar esta función para buscar una variable siempre, pero hay que identificar tres casos (para el reporte de errores):
      // 1) no existe el símbolo (regresamos nullptr)
      // 2) sí existe el símbolo y sí es variable
      // 3) sí existe el símbolo pero es una función
      for( auto it = scope_list.rbegin(); it != scope_list.rend(); ++it ) {
         if( auto pt = it->find_variable( t ); pt != nullptr ) {
            return pt;
         }
      }
      if( auto it = resources.function_overloads.find( t.source ); it != resources.function_overloads.end() ) {
         return &it->second;
      }
      return {};
   }
};

#endif //SEMANTIC_TYPES_H
