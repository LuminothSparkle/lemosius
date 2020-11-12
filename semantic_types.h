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

//Dijiste que realmente no habria colision de nombres para el proyecto asi que creo que vale la pena sacarlos, se estan usando en varios lados y es mejor evitar los decltype con program_resources::
//Ademas de que las demas estructuras eran así como syntax tree y operator_declaration, mejor ser consistentes y no creo que valga la pena andar juntando todo

struct visible_operator {
   bool                        access;
   const operator_declaration* declaration;
};

struct visible_function {
   bool                        access;
   const function_declaration* declaration;
};

using operator_overload_set = std::map<token_type,  visible_operator>;
using function_overload_set = std::map<std::size_t, visible_function>;
using builtin_overload_set  = std::set<std::size_t>;
//Yo creo que si hay necesidad de saber de esta interface para el programador
//ya que realmente usamos su tipo en el analisis del programa, y hace claro el codigo sin tanto decltype y program_resources::
//Además tu mismo dijiste que realmente no habrá colisiones con nombres, así que no veo necesidad de hacerlo

struct inclusion;

struct program_resources {
   std::filesystem::path source_path;
   std::vector<char>     source_file;
   std::vector<token>    header_tokens;
   std::vector<token>    program_tokens;
   syntax_tree           tree;

   std::vector<inclusion> inclusions;
   std::unordered_map<std::string_view, operator_overload_set> operator_overloads;
   std::unordered_map<std::string_view, function_overload_set> function_overloads;
   std::unordered_map<std::string_view, builtin_overload_set> builtin_overloads;

   program_resources(const auto& builtin_decls) {
      for(const auto&[name,overloads] : builtin_decls) {
         for(const auto& arity : overloads) {
            builtin_overloads[name].insert(arity);
         }
      }
   }

   auto get_operator_views( ) const {
      std::vector<std::string_view> res;
      for( const auto& [str_view, overload] : operator_overloads ) {
         res.push_back( str_view );
      }
      return res;
   }

   auto get_operator_decls( ) const {
      std::vector<operator_declaration> res;
      for( const auto& [str_view, overload] : operator_overloads ) {
         for( const auto& [pos, vop] : overload ) {
            res.push_back( *vop.declaration );
         }
      }
      return res;
   }

};

struct inclusion {
   bool              access;
   program_resources resources;
};

struct symbol { //Solo contiene datos con funciones auxiliares
   std::variant<const var_statement*, std::pair<const function_overload_set*, const builtin_overload_set*>> data;
   auto get_variable( ) const {
      auto ptr = std::get_if<0>( &data );
      return ptr != nullptr ? *ptr : nullptr;
   }
   auto get_user_overload_set( ) const {
      auto ptr = std::get_if<1>( &data );
      return ptr != nullptr ? ptr->first : nullptr;
   }
   auto get_builtin_overload_set( ) const {
      auto ptr = std::get_if<1>( &data );
      return ptr != nullptr ? ptr->second : nullptr;
   }
   auto get_overload_sets( ) const {
      return std::get_if<1>( &data );
   }
   std::pair<const function_declaration*, bool> get_overload( std::size_t arity ) const {
      if( auto user_ptr = get_user_overload_set( ); user_ptr != nullptr ) {
         if( auto iter = user_ptr->find( arity ); iter != user_ptr->end( ) ) {
            return { iter->second.declaration, true };
         }
      }
      if( auto builtin_ptr = get_builtin_overload_set( ); builtin_ptr != nullptr ) {
         if( auto iter = builtin_ptr->find( arity ); iter != builtin_ptr->end( ) ) {
            return { nullptr, true };
         }
      }
      return { nullptr, false };
   }
};

class scope { //Solo contiene datos con funciones auxiliares
   std::unordered_map<std::string_view, const var_statement*> variables; //Tabla de variables
 public:
   bool try_declare( const var_statement& stmt ) {
      return variables.emplace( std::string_view( *stmt.name ), &stmt ).second;
   }
   auto find_variable( const token& tok ) const {
      auto iter = variables.find( std::string_view( tok ) );
      return iter != variables.end( ) ? iter->second : nullptr;
   }

};

class scope_stack {
   std::list<scope> scope_list;
   const program_resources& resources;
 public:
   scope_stack( const program_resources& pr )
      : resources( pr ) {
   }
   void push( ) {
      scope_list.emplace_back( );
   }
   void pop( ) {
      scope_list.pop_back( );
   }
   scope& top( ) {
      return scope_list.back( );
   }
   const scope& top( ) const {
      return scope_list.back( );
   }

   symbol find_symbol( const token& tok ) const {     //Devuelve un tipo diferente segun sea el caso
      // generalmente vamos a usar esta función para buscar una variable siempre, pero hay que identificar tres casos (para el reporte de errores):
      // 1) no existe el símbolo (regresamos nullptr)
      // 2) sí existe el símbolo y sí es variable
      // 3) sí existe el símbolo pero es una función
      for( auto iter = scope_list.rbegin( ); iter != scope_list.rend( ); ++iter ) {
         if( auto var_ptr = iter->find_variable( tok ); var_ptr != nullptr ) {
            return { var_ptr };
         }
      }
      const function_overload_set* user_overloads    = nullptr;
      const builtin_overload_set*  builtin_overloads = nullptr;
      std::string_view symbol( tok );
      if( auto iter = resources.function_overloads.find( symbol ); iter != resources.function_overloads.end( ) ) {
         user_overloads = &iter->second;
      }
      if( auto iter = resources.builtin_overloads.find( symbol ); iter != resources.builtin_overloads.end( ) ) {
         builtin_overloads = &iter->second;
      }
      return { std::make_pair( user_overloads, builtin_overloads ) };
   }
};

struct resolution_table {
   const std::unordered_map<std::string_view, operator_overload_set>&                      operator_overloads;
   std::unordered_map<std::string_view, std::map<token_type, const function_declaration*>> operator_lookup;
   std::unordered_map<const terminal_expression*, const var_statement*>                    variable_lookup;
   std::unordered_map<const call_expression*, const function_declaration*>                 function_lookup;
};

enum VALUE_TYPE {
   RVALUE = 0,
   LVALUE,
};

#endif //SEMANTIC_TYPES_H
