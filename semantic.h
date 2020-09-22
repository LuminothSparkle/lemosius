#ifndef SEMANTIC_H
#define SEMANTIC_H

#include<vector>
#include<string>
#include<string_view>
#include<map>
#include<filesystem>

#include"token.h"
#include"syntax_tree.h"
#include"semantic_utilities.h"
#include"compiler_error.h"  

/* Utilizo el sistema de archivos para saber si esta incluyendo un archivo ya incluido */
bool is_recursive_include( const char* path1, const std::vector<const char*>&& paths ) {
    return std::any_of(paths.begin(),paths.end(),[&path1](const char* path2){
        return std::filesystem::equivalent(path1,path2);
    });
}

//No se si sea buena idea separar la parte del compilador con esta pero de momento asi esta
included_program compile(const char* path, std::vector<const char*>&& compiled);

/* No es obvio desde la cabecera pero regresa un included_program publicp */
/* Que son los recursos y operadores publicos que pueden exportar los demas include */
/* Un vector de compiled para checar compilacion recursiva, tal vez no lo marque como error despues pero de momento si */
auto semantic_analysis_header(program& actp, const syntax_tree& st, std::vector<const char*>&& compiled) {
    included_program publicp;
    for(const auto& incl : st.includes) {
        std::string file_name = extract_string_literal( incl.file_name );
        
        if( is_recursive_include( file_name.c_str(), std::forward<decltype(compiled)>(compiled) ) ) {
            throw compiler_error{RECURSIVE_INCLUDE_ERROR, incl.source, "Semantic Error: Recursive compilation."};
        }
        
        included_program inclp = compile( file_name.c_str( ), std::forward<decltype(compiled)>(compiled) );
        
        /* Incluye los operadores que pueden ser exportados por los demas archivos */
        for(const auto& op : inclp.operators) {
            bool redeclared = !actp.operators.emplace( op.first, op.second ).second; //Si la insercion no se pudo hacer, entonces esta redefiniendolo con la misma posicion.
            if( !redeclared && is_public(incl) ) {
                publicp.operators[op.first] = op.second;
            }
            else if( redeclared ) {
                throw compiler_error{OPERATOR_REDEFINED_ERROR, op.second.source, "Semantic Error: Redefinition Operator.", file_name, std::move(inclp.resources.back().source_file) };
            }
        }
        
        //Aqui se checan las funciones
        
        /* Pasa los recursos que utilizan los programas incluidos, si no puede exportar ningun operador o funcion, ignoralo */
        if( !inclp.operators.empty( ) ) {
            auto first = std::make_move_iterator( inclp.resources.begin( ) );
            auto last  = std::make_move_iterator( inclp.resources.end( )   );
            /* No importa realmente quien los tenga mientras se mantengan vivos */
            /* Si el include es publico entonces el programa publico se queda con los recursos */
            /* Si el include es privado entonces el programa actual (privadamente) se queda con los recursos y los recursos mueren con el */
            if( is_public(incl) ) {
                publicp.resources.insert( publicp.resources.end( ), first, last );
            }
            else {    
                actp.resources.insert(    actp.resources.end( ),    first, last );
            }
        }
    }
    /* Checa si los operadores del archivo actual colisiona con alguno ya declarado en los include */
    for(const auto& op_decl : st.operators) {
        auto key = std::make_pair(op_decl.symbol.source,obtain_position(op_decl.position));
        bool redeclared = !actp.operators.emplace( key, op_decl ).second;
        if( !redeclared && is_public(op_decl) ) {
                publicp.operators[key] = op_decl;
        }
        else if( redeclared ) {
            throw compiler_error{OPERATOR_REDEFINED_ERROR, op_decl.source, "Semantic Error: Redefinition Operator."};
        }
    }
    return publicp;
}

#endif
