#include"lexer.h"
#include"parser.h"
#include"semantic.h"
#include"inout.h"
#include"debugging.h"

#include<cstddef>

#include<iostream>
#include<map>
#include<stack>
#include<fstream>
#include<filesystem>
#include<utility>
#include<array>
#include<algorithm>
#include<span>

using map_path_source = std::map<std::filesystem::path, std::span<char>>;

struct compiler_error {     // cada vez que se eleva un error que es capturado en el driver, se construye el mensaje; en cuanto éste esté construido, nos podemos despreocupar de los recursos del programa
   static unsigned line_of( const token& t, const char* ini ) {
      return 1 + std::count( ini, t.source.begin( ), '\n' );
   }

   static unsigned col_of( const token& t, const char* ini ) {
      return 1 + ( t.begin( ) - std::find( std::reverse_iterator( t.begin( ) ), std::reverse_iterator( ini ), '\n' ).base( ) );
   }

   static std::string_view view_line_of( const token& t, const char* first, const char* last ) {
      auto        data     = std::find( std::reverse_iterator( t.begin( ) ), std::reverse_iterator( first ), '\n' ).base( );
      std::size_t data_len = std::find( t.begin( ),                       last,                        '\n' ) - data;
      return { data, data_len };
   }

   static std::string_view view_of( const token& t, auto len, const char* fin ) {
      auto        data     = t.begin( );
      std::size_t data_len = std::find( t.begin( ), std::min( fin - t.begin( ), len ) + t.begin( ), '\n' ) - t.begin( );
      return { data, data_len };
   }

   compiler_error( const map_path_source& compiled, const std::vector<std::pair<token, std::string>>& ee ) {
      auto obtain_file_data = [&compiled]( const token & t ) {
         return *std::find_if( compiled.begin(), compiled.end(),
         [&t]( const auto & ps ) {
            return &ps.second.front() <= &t.front() && &t.back() <= &ps.second.back();
         } );
      };
      std::ostringstream oss;
      for( const auto& [t, mes] : ee ) {
         auto [file_path, file_data] = obtain_file_data( t );
         auto file_begin = &file_data.front(), file_end = &file_data.back();
         oss << file_path.filename() << ":" << line_of( t, file_begin ) << ":" << col_of( t, file_begin ) << ": " << mes << "\n";
         //oss << "\t" << view_of(t, 10, file_end) << "\n";
         oss << "\t" << view_line_of( t, file_begin, file_end ) << "\n";
         oss << "\t" << std::string( col_of( t, file_begin ) - 1, ' ' ) << "^\n";
      }
      what = std::move( oss ).str( );
   }

   compiler_error( const map_path_source& compiled, const std::pair<token, std::string>& e )
      : compiler_error( compiled, std::vector<std::pair<token, std::string>>( { e } ) ) { }

   compiler_error( const std::filesystem::path& path, const std::pair<std::exception, std::string>& e ) {
      std::ostringstream oss;
      oss << path.filename( ) << ": " << e.second << "\n";
      what = std::move( oss ).str( );
   }

   std::string what;
};

std::optional<program_resources> compile( std::filesystem::path path, map_path_source& compiled )
try {
   path = std::filesystem::absolute( path );
   if( compiled.contains( path ) ) {
      return {};
   }

   program_resources pr;
   pr.source_path = path;
   pr.source_file = read_file( path );

   compiled.try_emplace( pr.source_path, pr.source_file );

   try {
      // Lexico 1
      lexer lex;
      const char* ini  = pr.source_file.data( );
      pr.header_tokens = lex.analisis( ini, PROC_K );
      // Sintactico 1
      token* tok_p   = pr.header_tokens.data( );
      pr.tree.header = parse_header( tok_p );
      // Compilacion recursiva
      auto old_dir = std::filesystem::current_path( );
      std::filesystem::current_path( path.parent_path( ) );
      for( const auto& inc : pr.tree.header.includes ) {
         try {
            auto res = compile( unquoted_str( inc.file_name ), compiled );
            if( res.has_value( ) ) {
               pr.inclusions.emplace_back( is_public( inc ), std::move( res ).value( ) );
            }
         } catch( std::stack<compiler_error>& s ) {
            s.push( compiler_error( compiled, std::make_pair( inc.file_name, "In file included" ) ) );
            throw;
         }
      }
      std::filesystem::current_path( old_dir );
      // Semantico 1
      pr.operator_overloads = generate_usables_operators( pr.inclusions, pr.tree.header.operators );
      // Lexico 2
      lex.overwrite_operators( std::move( get_operator_views( pr.operator_overloads ) ) );
      pr.program_tokens = lex.analisis( ini, END_OF_INPUT );
      // Sintactico 2
      tok_p             = pr.program_tokens.data( );
      pr.tree.functions = parse_program( tok_p, get_operator_decls( pr.operator_overloads ) );

      return pr;
   } catch( const std::vector<std::pair<token, std::string>>& e ) {
      throw std::stack<compiler_error>( { compiler_error( compiled, e ) } );
   } catch( const std::pair<token, std::string>& e ) {
      throw std::stack<compiler_error>( { compiler_error( compiled, e ) } );
   }
} catch( const std::filesystem::filesystem_error& e ) {
   throw std::stack<compiler_error>( { compiler_error( path, std::make_pair( e, "Cannot stat path" ) ) } );
} catch( const std::ifstream::failure& e ) {
   throw std::stack<compiler_error>( { compiler_error( path, std::make_pair( e, "Cannot open or read file" ) ) } );
}

int main( int argc, char *argv[] )
try {
   if( argc != 2 ) {
      std::cout << "Usage:" << argv[0] << " <path_file_name>\n";
      return 0;
   }
   map_path_source compiled;
   std::cout << compile( argv[1], compiled ).value( );
} catch( std::stack<compiler_error>& s ) {
   while( !s.empty( ) ) {
      std::cout << s.top( ).what << "\n";
      s.pop( );
   }
} catch( const std::exception& e ) {
   std::cout << e.what( ) << "\n";
} catch( ... ) {
   std::cout << "Unknown error\n";
}
