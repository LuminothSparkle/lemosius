#include"lexer.h"
#include"parser.h"
#include"semantic.h"
#include"inout.h"
#include"debugging.h"

#include<cstddef>

#include<iostream>
#include<set>
#include<stack>
#include<fstream>
#include<filesystem>
#include<utility>
#include<array>
#include<algorithm>

struct compiler_error {     // cada vez que se eleva un error que es capturado en el driver, se construye el mensaje; en cuanto éste esté construido, nos podemos despreocupar de los recursos del programa
    static unsigned line_of(const token& t, const char* ini) {
        return 1 + std::count(ini, t.source.begin( ), '\n');
    }

    static unsigned col_of(const token& t, const char* ini) {
        return 1 + (t.source.begin( ) - std::find(std::reverse_iterator(t.source.begin( )), std::reverse_iterator(ini), '\n').base( ));
    }

    static std::string_view view_line_of(const token& t, const char* first, const char* last) {
        auto          data = std::find(std::reverse_iterator(t.source.begin( )),std::reverse_iterator(first),'\n').base( );
        long unsigned len  = std::find(t.source.begin( ),                       last,                        '\n') - data;
        return { data, len };
    }

    static std::string_view view_of(const token& t, int len, const char* fin) {
        return { t.source.begin( ), std::size_t(std::find(t.source.begin( ), (fin - t.source.begin( ) <= len ? fin : t.source.begin( ) + len), '\n') - t.source.begin( )) };
    }
    
    template<std::size_t N>  // si un error tiene dos puntos a reportar (ej. la redeclaración de un operador, el original y el incorrecto) podemos elevar un array<pair>
    compiler_error(std::filesystem::path path, const std::vector<char>& source, const std::array<std::pair<token, std::string>, N>& ee) {
        std::ostringstream oss;
        for(const auto& e : ee) {
            oss << path.filename( ) << ":" << line_of(e.first, source.data( )) << ":" << col_of(e.first, source.data( )) << ": " << e.second << "\n";
            //oss << "\t" << view_of(e.first, 10, source.data( ) + source.size( )) << "\n";
            oss << "\t" << view_line_of(e.first, source.data( ), source.data( ) + source.size( )) << "\n";
        }
        what = std::move(oss).str( );
    }

    compiler_error(const std::filesystem::path& path, const std::vector<char>& source, const std::pair<token, std::string>& e)
    : compiler_error(path, source, std::array<std::pair<token, std::string>,1>({ e })) { }

    compiler_error(const std::filesystem::path& path, const std::pair<std::exception,std::string>& e) {
        std::ostringstream oss;
        oss << path.filename( ) << ": " << e.second << "\n";
        what = std::move(oss).str( );
    }

    std::string what;
}; // end struct compiler_error

std::optional<program_resources> compile( std::filesystem::path path, std::set<std::filesystem::path>& compiled )
try {
    path = std::filesystem::absolute(path);
    if( !compiled.insert( path ).second ) {
        return {};
    }
    program_resources pr;
    pr.source_path = path;
    pr.source_file = read_file( path );
    try {
        const char* ini = pr.source_file.data( );
        // Lexico 1
        lexer lex;
        pr.header_tokens = lex.analisis( ini, PROC_K );
        token* tok_p = pr.header_tokens.data( );
        // Sintactico 1
        pr.tree = parse_header( tok_p );
        // Compilacion recursiva
        auto cur_dir = std::filesystem::current_path( );
        std::filesystem::current_path(path.parent_path( ));
	string_literal_parser str_par;        
	for(const auto& inc : pr.tree.includes) {
            try {
                if(auto res = compile(str_par.parse(inc.file_name), compiled); res.has_value( )) {
                    pr.inclusions.push_back(program_resources::inclusion{is_public(inc), std::move(res.value( ))});
                }
            }
            catch(std::stack<compiler_error>& s) { // sí, la idea es que si ocurre un error en un include del include del include, empiece a juntar los mensajes de error en una pila
                s.push(compiler_error(path, pr.source_file, std::make_pair(inc.file_name, "In file included")));
                throw;
            }
        }
        std::filesystem::current_path(cur_dir);
        // Lexico 2
        pr.program_tokens = lex.analisis( ini, END_OF_INPUT );
        tok_p = pr.program_tokens.data( );
        // Sintactico 2
        parse_program(tok_p,pr.tree);
        return pr;
    }
    catch(const std::pair<token, std::string>& e){ // un error en el archivo actual, lo envolvemos en una pila y lo reelevamos
       throw std::stack<compiler_error>({ compiler_error( path, pr.source_file, e ) });
    }
}
catch(const std::filesystem::filesystem_error& e) {
   throw std::stack<compiler_error>({ compiler_error( path, std::make_pair(e, "Cannot stat path")         ) });
}
catch(const std::ifstream::failure& e) {
   throw std::stack<compiler_error>({ compiler_error( path, std::make_pair(e, "Cannot open or read file") ) });
} // end function compile

int main(int argc, char *argv[])
try {
    if(argc != 2) {
        std::cout << "Usage:" << argv[0] << " <path_file_name>";
        return 0;
    }
    std::set<std::filesystem::path> compiled;
    std::cout << compile( argv[1], compiled ).value( );
}
catch(std::stack<compiler_error>& s) {
    while(!s.empty( )) {
        std::cout << s.top( ).what << "\n";
        s.pop( );
    }
}
catch(const std::exception& e) {
   std::cout << e.what( ) << "\n";
}
catch(...) {
   std::cout << "Unknown error\n";
} // end function main
