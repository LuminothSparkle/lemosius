#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include<string>

/* Codigos de error */
enum compiler_error_code {
    LEXIC_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    FILE_ERROR,
    RECURSIVE_INCLUDE_ERROR,
    OPERATOR_REDEFINED_ERROR,
};

/* Estructura para localizar donde fue el error, en que archivo, donde del archivo, porque y el string al archivo dado que hay referencias a el*/
struct compiler_error {
    compiler_error_code code;
    std::string_view where;
    std::string what;
    std::string file_name;
    std::string source_file;
};

#endif
