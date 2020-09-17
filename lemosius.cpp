#include<iostream>
#include<vector>
#include<fstream>
#include<filesystem>
#include<utility>
#include<string>
#include"lexer.h"
#include"parser_.h"
#include"inout.h"
#include"error_handling.h"


int main(int argc, char *argv[])
try {
    std::string text = read_file( argv[1] );
    try {
       const char* ini = text.c_str();
       lexer L;
       std::vector<token> tokens = L.analisis(ini,PROC_K);
       for(auto& token : tokens) {
           std::cout << token.type << "\n";
           std::cout << token.original << "\n";
       }
       auto it_tok = tokens.data();
       auto arbol = parser1(it_tok);
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
