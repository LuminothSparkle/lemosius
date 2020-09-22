#ifndef COMPILER_H
#define COMPILER_H

#include<vector>
#include<string>
#include<string_view>

#include"inout.h"
#include"token.h"
#include"lexer.h"
#include"parser.h"
#include"syntax_tree.h"
#include"semantic.h"
#include"semantic_utilities.h"

included_program compile( const char* path, std::vector<const char*>&& compiled = std::vector<const char*>() ) {
    program_resources pr;
    pr.source_file = read_file( path );
    try {
        auto ini = pr.source_file.c_str();
        /* Lexico 1 */
        lexer lex;
        pr.header_tokens = lex.analisis( ini, PROC_K );
        auto tok_p = pr.header_tokens.data( );
        /* Sintactico 1 */
        syntax_tree st = parse_header( tok_p );
        /* Semantico 1 */
        program actp;
        included_program publicp = semantic_analysis_header(actp,st,std::forward<decltype(compiled)>(compiled));
        /* Lexico 2 */
        pr.program_tokens = lex.analisis( ini, END_OF_INPUT );
        tok_p = pr.program_tokens.data( );
        /* Sintactico 2 */
        parse_program(tok_p,st);
        /* Mueve los recursos del programa actual a la importacion publica */
        publicp.resources.emplace_back(std::move(pr));
        return publicp;
    }
    catch(compiler_error& e) {
	    /* Si encuentra un error de compilacion en algun lado, normalmente no ha guardado el nombre del archivo y tiene que pasar el archivo en caso de que no se hayan definido */
        /* Esto me hace pensar que tal vez deba cambiar un poco el parser y el lexer porque creo que la manera que hice ahorita es muy revuelta */
        /* y depende mucho de si se dan cuenta los analizadores del error en ciertos momentos haciendo que la parte de errores y la logica no esten tan separados */
        if(e.file_name.empty()) {
            e.file_name = std::string(path);
            e.source_file = std::move(pr.source_file);
        }
        throw;
    }
}

#endif
