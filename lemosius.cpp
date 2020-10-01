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
#include<span>

using path_source = std::pair<std::filesystem::path,std::span<char>>;
using map_path_source = std::map<std::filesystem::path,std::span<char>>;

struct compiler_error {     // cada vez que se eleva un error que es capturado en el driver, se construye el mensaje; en cuanto éste esté construido, nos podemos despreocupar de los recursos del programa
    static unsigned line_of(const token& t, const char* ini) {
        return 1 + std::count(ini, t.source.begin( ), '\n');
    }

    static unsigned col_of(const token& t, const char* ini) {
        return 1 + (t.source.begin( ) - std::find(std::reverse_iterator(t.source.begin( )), std::reverse_iterator(ini), '\n').base( ));
    }

    static std::string_view view_line_of(const token& t, const char* first, const char* last) {
        auto          data     = std::find(std::reverse_iterator(t.source.begin( )),std::reverse_iterator(first),'\n').base( );
        long unsigned data_len = std::find(t.source.begin( ),                       last,                        '\n') - data;
        return { data, data_len };
    }

    static std::string_view view_of(const token& t, long int len, const char* fin) {
    	auto data = t.source.begin( );
    	auto data_len = std::size_t(std::find(t.source.begin( ), std::min(fin - t.source.begin( ),len) + t.source.begin( ), '\n') - t.source.begin( ));
        return { data , data_len };
    }
    
    compiler_error(const map_path_source& compiled, const std::vector<std::pair<token, std::string>>& ee) {
        auto obtain_file_data = [&compiled](const token& t) {
            return std::find_if(compiled.begin(),compiled.end(),
            [&t](const path_source& ps){
                return &ps.second.front() <= t.source.data() && t.source.data() <= &ps.second.back();
            });
        };
        std::ostringstream oss;
        for(const auto& e : ee) {
            token t           = e.first;
            std::string mes   = e.second;
            auto file_data    = *obtain_file_data(t);
            auto file_name    = file_data.first.filename();
            auto file_begin   = &file_data.second.front();
            auto file_end     = &file_data.second.back();
            oss << file_name << ":" << line_of(t, file_begin) << ":" << col_of(t, file_begin) << ": " << mes << "\n";
            //oss << "\t" << view_of(t, 10, file_end) << "\n";
            oss << "\t" << view_line_of(t, file_begin, file_end) << "\n";
        }
        what = std::move(oss).str( );
    }

    compiler_error(const map_path_source& compiled, const std::pair<token, std::string>& e)
    : compiler_error(compiled, std::vector<std::pair<token, std::string>>({ e })) { }

    compiler_error(const std::filesystem::path& path, const std::pair<std::exception,std::string>& e) {
        std::ostringstream oss;
        oss << path.filename( ) << ": " << e.second << "\n";
        what = std::move(oss).str( );
    }

    std::string what;
}; // end struct compiler_error

std::optional<program_resources> compile( std::filesystem::path path, map_path_source& compiled )
try {
    path = std::filesystem::absolute(path);
    if( compiled.find( path ) != compiled.end( ) ) {
        return {};
    }
    program_resources pr;
    pr.source_path = path;
    pr.source_file = read_file( path );
    
    compiled.emplace(pr.source_path,pr.source_file);
    
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
        for(const auto& inc : pr.tree.includes) {
            try {
                auto res = compile(unquote(inc.file_name), compiled);
                if(res.has_value( )) {
                    pr.inclusions.push_back(program_resources::inclusion{is_public(inc), std::move(res).value( )});
                }
            }
            catch(std::stack<compiler_error>& s) {
                s.push(compiler_error(compiled, std::make_pair(inc.file_name, "In file included")));
                throw;
            }
        }
        std::filesystem::current_path(cur_dir);
        // Semantico 1
        analyze_operators(pr);
        // Lexico 2
        pr.program_tokens = lex.analisis( ini, END_OF_INPUT );
        tok_p = pr.program_tokens.data( );
        // Sintactico 2
        parse_program(tok_p,pr.tree);
        
        return pr;
    }
    catch(const std::vector<std::pair<token, std::string>>& e){
       throw std::stack<compiler_error>({ compiler_error( compiled, e ) });
    }
    catch(const std::pair<token, std::string>& e){
       throw std::stack<compiler_error>({ compiler_error( compiled, e ) });
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
    map_path_source compiled;
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
