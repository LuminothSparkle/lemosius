#include<iostream>
#include<vector>
#include<fstream>
#include<filesystem>
#include<utility>
#include<string>

#include<algorithm>
#include<iterator>
#include<string_view>
#include<ostream>
#include<cstddef>

#include"lexer.h"
#include"parser.h"
#include"compiler.h"
#include"debugging.h"
#include"compiler_error.h"
#include"token.h"

int main(int argc, char *argv[])
try {
    if(argc != 2) {
        throw "Usage error.\n"
              "Usage:" + std::string( argv[0] ) + " <path_file_name>";
    }
    using debugging::operator<<;
    std::cout << "Compilacion exitosa\n";
    /* Imprime el programa, al final imprime los operadores que pueden ser vistos publicamente */
    std::cout << compile( argv[1] );
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
catch(const compiler_error& e) {
    /* Ahora que estandarice los errores solo falta imprimirlos */
    using debugging::operator<<;
    std::cout << e << "\n";
}
