#include "lexer.h"
#include "parser.h"
#include "semantic_global.h"
#include "inout.h"
#include "codegen.h"
//#include "debugging.h"

#include <cstddef>

#include <iostream>
#include <map>
#include <stack>
#include <fstream>
#include <filesystem>
#include <utility>
#include <array>
#include <algorithm>
#include <span>
#include <tuple>

using map_path_source = std::map<std::filesystem::path, std::span<char>>;

struct compiler_error {     // cada vez que se eleva un error que es capturado en el driver, se construye el mensaje; en cuanto éste esté construido, nos podemos despreocupar de los recursos del programa
   static std::size_t line_of( const token& tok, const char* first ) {
      return 1 + std::count( first, tok.begin( ), '\n' );
   }

   static std::size_t col_of( const token& tok, const char* first ) {
      return 1 + ( tok.begin( ) - std::find( std::reverse_iterator( tok.begin( ) ), std::reverse_iterator( first ), '\n' ).base( ) );
   }

   static std::string_view view_line_of( const token& tok, const char* first, const char* last ) {
      auto        data = std::find( std::reverse_iterator( tok.begin( ) ), std::reverse_iterator( first ), '\n' ).base( );
      std::size_t len  = std::find( tok.begin( ),                          last,                           '\n' ) - data;
      return { data, len };
   }

   compiler_error( const map_path_source& compiled, const std::vector<std::pair<token, std::string>>& ee ) {
      auto obtain_file_data = [&compiled]( const token & tok ) {
         auto [file_path, file_data] = *std::find_if( compiled.begin( ), compiled.end( ),
         [&tok]( const auto & path_source ) {
            const auto& source = path_source.second;
            return &source.front( ) <= &tok.front( ) && &tok.back( ) <= &source.back( );
         } );
         return std::make_tuple( file_path, &file_data.front( ), &file_data.back( ) );
      };
      std::ostringstream oss;
      for( const auto& [tok, mes] : ee ) {
         auto [file_path, file_begin, file_end] = obtain_file_data( tok );
         oss << file_path.filename( ) << ":" << line_of( tok, file_begin ) << ":" << col_of( tok, file_begin ) << ": " << mes << "\n";
         oss << "\t" << view_line_of( tok, file_begin, file_end )          << "\n";
         oss << "\t" << std::string( col_of( tok, file_begin ) - 1, ' ' )  << "^\n";
      }
      what = std::move( oss ).str( );
   }

   compiler_error( const map_path_source& compiled, const std::pair<token, std::string>& err )
      : compiler_error( compiled, std::vector<std::pair<token, std::string>>( { err } ) ) { }

   compiler_error( const std::filesystem::path& path, const std::pair<std::exception, std::string>& err ) {
      std::ostringstream oss;
      oss << path.filename( ) << ": " << err.second << "\n";
      what = std::move( oss ).str( );
   }

   std::string what;
};

std::optional<program_resources> compile( std::filesystem::path path, const std::filesystem::path& include_path, map_path_source& compiled, std::ostream& os )
try {
   if( path = std::filesystem::absolute( path ); compiled.contains( path ) ) {
      return {};
   }
   program_resources pr;
   pr.source_path = path;
   pr.source_file = read_file( path );
   compiled.try_emplace( pr.source_path, pr.source_file );
   try {
      // Lexico 1
      lexer lex;
      const char* file_cptr = pr.source_file.data( );
      pr.header_tokens      = lex.tokenization( file_cptr, PROC_K, "String not recognized." );
      file_cptr             = unget_tokens_until( pr.header_tokens, std::not_fn( is_access ) );
      // Sintactico 1
      const token* tok_ptr  = pr.header_tokens.data( );
      pr.tree.header        = parse_header( tok_ptr );
      // Compilacion recursiva
      auto old_dir          = std::filesystem::current_path( );
      std::filesystem::current_path( path.parent_path( ) );
      for( const auto& inc : pr.tree.header.includes ) {
         auto unquoted_file_name = unquoted_str( *inc.file_name );
         try {
            auto res = compile( unquoted_file_name, include_path, compiled, os );
            if( res.has_value( ) ) {
               pr.inclusions.emplace_back( is_public( inc ), std::move( res ).value( ) );
            }
         } catch( std::stack<compiler_error>& s ) {
            s.push( compiler_error( compiled, { *inc.file_name, "In file included" } ) );
            throw;
         }
      }
      std::filesystem::current_path( old_dir );
      // Semantico 1
      pr.operator_overloads = generate_usables_operators( pr.inclusions, pr.tree.header.operators );
      // Lexico 2
      lex.overwrite_operators( pr.get_operator_views( ) );
      pr.program_tokens     = lex.tokenization( file_cptr, END_OF_INPUT, "String not recognized, if was an operator, maybe you not declared the operator." );
      // Sintactico 2
      tok_ptr               = pr.program_tokens.data( );
      pr.tree.functions     = parse_program( tok_ptr, pr.get_operator_decls( ) );
      pr.function_overloads = generate_usables_functions( pr.inclusions, pr.tree.functions );
      // Semantico 2
      resolution_table tbl  = analyze_program( pr );
      write_program( pr, tbl, include_path, os );
      return pr;
   } catch( const std::vector<std::pair<token, std::string>>& err ) {
      throw std::stack<compiler_error>( { compiler_error( compiled, err ) } );
   } catch( const std::pair<token, std::string>& err ) {
      throw std::stack<compiler_error>( { compiler_error( compiled, err ) } );
   }
} catch( const std::filesystem::filesystem_error& err ) {
   throw std::stack<compiler_error>( { compiler_error( path, { err, "Cannot stat path"         } ) } );
} catch( const std::ifstream::failure& err ) {
   throw std::stack<compiler_error>( { compiler_error( path, { err, "Cannot open or read file" } ) } );
}

int main( int argc, char *argv[] )
try {
   if( argc != 2 ) {
      std::cout << "Usage:" << argv[0] << " <path_file_name>\n";
      return 0;
   }
   map_path_source compiled;
   std::filesystem::path include_path( std::filesystem::absolute( argv[0] ) );
   include_path = include_path.parent_path( ) / "include";
   std::filesystem::path file_path( argv[1] );
   std::ostringstream oss;
   compile( file_path,  include_path, compiled, oss );
   std::ofstream ofs( file_path.replace_extension( ".cpp" ).c_str( ) );
   ofs << std::move( oss ).str( );
   //std::cout << compile( argv[1], compiled, std::cout ).value( );
} catch( std::stack<compiler_error>& sce ) {
   while( !sce.empty( ) ) {
      std::cout << sce.top( ).what << "\n";
      sce.pop( );
   }
} catch( const std::exception& err ) {
   std::cout << err.what( ) << "\n";
} catch( ... ) {
   std::cout << "Unknown error\n";
}
