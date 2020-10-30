#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include"lexer.h"

#include<string>
#include<string_view>
#include<utility>
#include<charconv>
#include<cstdlib>


#include<filesystem>
#include<unordered_map>
#include<set>
#include<map>
#include<list>
#include<any>

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
   static std::list<scope*> scope_list;                                   //Nadie toca la lista externamente
   const program_resources& resources;
   std::list<scope*>::iterator parent;                                    //Tal vez me estoy complicando mucho la vida pero supuse el caso en el que me muero yo y mis hijos aun viven
   std::list<scope*>::iterator iter;                                      //Entonces debo hacer que mis hijos ya no apunten a mi cuando muera y que ademas no vayamos a ciegas en la lista
   std::set<scope*> childs;                       //Destruyendo nodos que no sabemos si son hijos solo porque se insertaron despues
   //Si ese fuera un problema en realidad creo que esta dificil no hacer alguna operación a ciegas
   std::unordered_map<std::string_view, token> variables;                 //Tabla de variables

   scope( const program_resources& pr, std::list<scope*>::iterator parent ) //Esta interfaz no se muestra al publico
      : resources( pr ), parent( parent ), iter( scope_list.emplace( parent, this ) ) {
      if( parent != scope_list.end() ) {
         childs.insert( this );
      }
   }

 public:
   enum return_type {                              //Es posible que no se tengan que usar debido al std::any pero no me quiero arriesgar demasiado aun
      NO_DECLARED = -1,
      FUNCTION_MAP,
      VARIABLE
   };

   scope() = delete;                               //Evitar scopes sin program_resources
   scope& operator =( const scope& ) = delete;     //Evitar copias de un mismo scope
   scope( const scope& ) = delete;                 //Evitar copias de un mismo scope

   explicit scope( const program_resources& pr )   //Si no tiene padre se genera asi y para evitar castings raros que sea explicit
      : scope( pr, scope_list.end() ) {
   }

   auto emplace_succesor() {                       //Genera un scope de uno existente asi
      return scope( resources, iter );
   }

   ~scope( ) {                                      //Si me muero yo que mis hijos apunten a mi padre y mi padre apunte a mis hijos y finalmente me borro
      if( parent != scope_list.end() ) {
         ( *parent )->childs.erase( this );
         for( auto child : childs ) {
            ( *parent )->childs.insert( child );
         }
      }
      for( auto child : childs ) {
         child->parent = parent;
      }
      scope_list.erase( iter );
   }

   bool try_declare( const token& t ) {
      return variables.emplace( t.source, t ).second;
   }

   std::pair<std::any, return_type> find_symbol( const token& t ) const {     //Devuelve un tipo diferente segun sea el caso
      // generalmente vamos a usar esta función para buscar una variable siempre, pero hay que identificar tres casos (para el reporte de errores):
      // 1) no existe el símbolo (regresamos nullptr)
      // 2) sí existe el símbolo y sí es variable
      // 3) sí existe el símbolo pero es una función
      for( auto it = iter; it != scope_list.end(); it = ( *it )->parent ) {
         if( auto v_it = ( *it )->variables.find( t.source ); v_it != ( *it )->variables.end() ) {
            return { v_it->second, VARIABLE };
         }
      }
      try {
         auto& func_map = resources.function_overloads.at( t.source );
         return { func_map, FUNCTION_MAP };
      } catch( const std::out_of_range& e ) {
         return { {}, NO_DECLARED };
      }
   }

};

std::list<scope*> scope::scope_list = std::list<scope*>();

template<typename T>
bool is_public( const T& t ) {
   return t.access == nullptr || *t.access == PUBLIC_K;
};

bool isoctal( char c ) {
   return isdigit( c ) && c < '8';
}

template<typename T>
std::pair<T, bool> get_representation( const std::string_view& s ) {
   //      :( GCC aún no tiene from_chars para floating point
   //long double res;
   //auto [end, error] = std::from_chars(s.data( ), s.data( ) + s.size( ), res);
   //return { T(res), (!error && p == s.end( ) && T(res) == res) }; */
   char* p;
   std::string buffer( s );
   long double res = std::strtold( buffer.c_str(), &p );
   if( p != buffer.c_str() + buffer.size() ) {
      return { T( ), false };
   }
   return { T( res ), T( res ) == res };
   //try {      // horrible, pero más fácil de escribir que el que usa std::strold, en particular porque strold requiere una cadena con terminador
   //   long double res = std::stold(std::string(s));      // nosotros sabemos que nuestros string_views vienen de tokens válidos y que acaban en algo que ya no es número,
   //   return { T(res), T(res) == res };                  // pero prefiero no andarle jugando al vivo
   //} catch (...) {
   //   return { T( ), false };
   //}
}

template<typename T>
T get_representation( const token& t, const std::string& error_mes = "" ) {
   auto [val, success] = get_representation<T>( t.source );
   if( !success ) {
      throw std::make_pair( t, "Semantic Error:" + error_mes );
   }
   return val;
}

std::pair<std::string, std::string_view::iterator> unquoted_str( const std::string_view& sv ) {
   auto it = sv.begin();
   if( sv.empty() || sv.front() != '"' ) { //Mejor para no tener problemas, ya que igual se checa en el lexer pero tambien aqui por si las dudas.
      return { "", sv.begin() };
   }
   std::string res;
   for( std::advance( it, 1 ); it != sv.end() && *it != '"'; std::advance( it, 1 ) ) {
      if( *it == '\\' ) {
         std::advance( it, 1 );
         constexpr std::string_view escaped = R"(abtnvfre0"'\)";
         constexpr std::string_view replacement = "\a\b\t\n\v\f\r\x1B\0\"\'\\";
         if( std::size_t idx = escaped.find( *it ); idx < escaped.size( ) ) {
            res.push_back( replacement[idx] );
            std::advance( it, 1 );
         } else if( bool is_hex = *it == 'x'; is_hex || isoctal( *it ) ) {
            std::size_t base = ( is_hex ? 16 : 8 );
            std::advance( it, is_hex );
            auto obtain_char = [&base, &sv]( auto & it, unsigned char& c ) {
               unsigned char val = 0;
               auto first = it;
               it = std::from_chars( first, sv.end(), val, base ).ptr;
               return first != it;
            };
            unsigned char c;
            if( !obtain_char( it, c ) ) {
               return { res, it };
            }
            res.push_back( c );
            while( is_hex && obtain_char( it, c ) ) {
               res.push_back( c );
            }
         } else {
            return { res, it };
         }
      } else {
         res.push_back( *it );
      }
   }
   return { res, std::next( it, *it == '"' ) };
}

std::string unquoted_str( const token& t, const std::string& error_mes = "" ) {
   using namespace std::string_literals;
   auto [str, it] = unquoted_str( t.source );
   if( it != t.end() ) {
      throw std::make_pair( token{ t.type, { it, t.end() } }, "Semantic Error:" + error_mes );
   }
   return str;
}



#endif
