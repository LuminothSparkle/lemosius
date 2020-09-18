#include<iostream>
#include<vector>
#include<fstream>
#include<filesystem>
#include<utility>
#include<string>

#include"lexer.h"
#include"parser.h"
#include"inout.h"
#include"error_handling.h"


int main(int argc, char *argv[])
try {
    std::string text = read_file( argv[1] );
    try {
        const char* ini = text.c_str();
        lexer lex;
        std::vector<token> tokens = lex.analisis(ini,PROC_K);
        for(auto& token : tokens) {
            std::cout << token.original << " : " << token.type << "\n";   
        }
        auto it_tok = tokens.data();
        auto arbol = parse_header(it_tok);
        std::cout << "\nincludes\n\n";
        for(auto& i_d : arbol.include_decls) {
            std::cout << "visibility: " << (i_d.visibility == nullptr ? "nullptr" : std::string(*i_d.visibility)) << "\n" 
                      << "file_name: " << std::string(*i_d.file_name) << "\n\n";   
        }
        std::cout << "operators\n\n";
        for(auto& o_d : arbol.operator_decls) {
            std::cout << "visibility: " << (o_d.visibility == nullptr ? "nullptr" : std::string(*o_d.visibility)) << "\n"
                      << "symbol: " << std::string(*o_d.symbol) << "\n"
                      << "position: " << std::string(*o_d.position) << "\n"
                      << "asociativity: " << (o_d.asociativity == nullptr ? "nullptr" : std::string(*o_d.asociativity)) << "\n"
                      << "precedence: " << (o_d.precedence == nullptr ? "nullptr" : std::string(*o_d.precedence)) << "\n"
                      << "function: " << std::string(*o_d.function) << "\n\n";   
        }
        tokens = lex.analisis(ini,END_OF_INPUT);
        for(auto& token : tokens) {
            std::cout << token.original << " : " << token.type << "\n";   
        }
        it_tok = tokens.data();
        parse_program(it_tok,arbol);
    }
    catch(const std::pair<token, const char*>& e) {
        error_report(std::cout, text.c_str(), text.c_str() + text.length(), e);
    }
}
catch(const std::ifstream::failure& e) {
    std::cout << "Error al leer el archivo: " << e.what() << "\n";
}
catch(const std::filesystem::filesystem_error& e) {
    std::cout << "Error al abrir el archivo " << e.path1() << ": " << e.what() << "\n";
}
