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
#include"debugging.h"


int main(int argc, char *argv[])
try {
    if(argc != 2) {
        throw "Usage error.\n"
              "Usage:" + std::string( argv[0] ) + " <path_file_name>";
    }
    std::string text = read_file( argv[1] );
    
    try {
        using debugging::operator<<;
    
        const char* ini = text.c_str();
        lexer lex;
        std::vector<token> t_header = lex.analisis(ini,PROC_K);
        auto it_tok = t_header.data();
        auto arbol = parse_header(it_tok);
        std::cout << t_header << "\n";
        std::cout << arbol.include_declarations << arbol.operator_declarations << "\n";
        auto t_program = lex.analisis(ini,END_OF_INPUT);
        it_tok = t_program.data();
        std::cout << t_program << "\n";
        parse_program(it_tok,arbol);
    }
    catch(const std::pair<token, const char*>& e) {
        error_report(std::cout, text.c_str(), text.c_str() + text.length(), e);
    }
    
}
catch(const std::string& mes) {
    std::cout << mes << "\n";
}
catch(const std::filesystem::filesystem_error& e) {
    std::cout << "Error al abrir el archivo " << e.path1() << ":\n"
              << e.what() << "\n";
}
catch(const std::ifstream::failure& e) {
    std::cout << "Error al leer el archivo:\n" 
              << e.what() << "\n";
}
