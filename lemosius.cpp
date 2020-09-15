#include<iostream>
#include"prelude_parser.h"
#include"inout.h"
#include"error_handling.h"


int main(int argc, char *argv[])
try {
	std::string text = read_file( argv[1] );
   try {
      const char* ini = text.c_str();
      std::vector<token_anotada> preludio = prelude_lexer( ini );
      for(auto& token : preludio) {
         std::cout << token.type << "\n";
         std::cout << token.original << "\n";
      }
      auto tokens = preludio.begin();
      header(tokens);
   }
   catch(const std::pair<token_anotada, const char*>& e) {
      error_report(std::cout, text.c_str(), text.c_str() + text.length(), e);
   }
}
catch(const std::ifstream::failure& e) {
   std::cout << "Error al leer el archivo: " << e.what() << "\n";
}
catch(const std::filesystem::filesystem_error& e) {
   std::cout << "Error al abrir el archivo " << e.path1() << ": " << e.what() << "\n";
}
