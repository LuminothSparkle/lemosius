#ifndef LEXER_H
#define LEXER_H

#include<cstddef>

#include<algorithm>
#include<utility>
#include<string_view>
#include<string>
#include<vector>
#include<array>

#include<re2/re2.h>
#include<re2/stringpiece.h>

#include"token.h"
#include"compiler_error.h"

struct lexer {
    std::array<std::vector<std::string>, END_OF_INPUT> token_forms; //Formas que pueden tomar los tokens reconocidos por el lexer

    RE2 generate_expresion( ) const {
        /* Lambda para juntar cadenas con un prefijo "prefix", un sufijo "suffix" y un separador "sep" entre cadenas */
        auto join = [&](const std::vector<std::string>& v, const char* sep = "", const char* prefix = "", const char* suffix = "") {
            std::string res = prefix;
            for (std::size_t i = 0; i < v.size( ); ++i) {
                res += v[i] + (i + 1 < v.size( ) ? sep : suffix);
            }
            return res;
        };
        std::vector<std::string> other_tokens = {          //Tokens reconocidos como validos pero que no generan token para el parser
            R"(\s+)",                                      //Espacios y saltos de linea
            R"(\#\#\#(?:[^\#](?s:.*)[^\#]|[^\#]?)\#\#\#)", //Comentarios multilinea
            R"(\#\#(?:[^\#\n][^\n]*\n|\n|$))",             //Comentarios de una linea
        };
        std::vector<std::string> parts(token_forms.size( ) + 1);
        parts[0] = join(other_tokens,"|");
        std::transform(token_forms.begin( ),token_forms.end( ),parts.begin( ) + 1,[&](const std::vector<std::string>& t) {
            return join(t,"|","(",")");
        });
        return RE2(join(parts,"|"));
    }

    std::vector<token> analisis(const char*& ini, token_type stop) const  {
        /* Genera expresion e inicializa la entrada del mismo */
        RE2 e = generate_expresion( );
        re2::StringPiece input( ini );
        /* Se generan los argumentos para la expresion regular, que son el arreglo de cadenas que son atrapadas, cada una identificada por el token_type */
        std::array< re2::StringPiece, END_OF_INPUT > mt;
        std::array< RE2::Arg,         END_OF_INPUT > args;
        std::array< const RE2::Arg*,  END_OF_INPUT > m;
        std::transform(mt.begin( ),   mt.end( ),   args.begin( ), [](re2::StringPiece& s){return &s;});
        std::transform(args.begin( ), args.end( ), m.begin( ),    [](RE2::Arg& a)        {return &a;});
        /* Inicia el analisis lexico */
        std::vector<token> tokens;
        /* Para hasta que termines la entrada y cuando el token de parada sea END_OF_INPUT o llegues al token de parada */
        while( !input.empty( ) && (stop == END_OF_INPUT || mt[stop].empty( )) ) {
            /* Reconoce la cadena y si falla manda un error de compilador */
            if( !RE2::FindAndConsumeN(&input, e, m.begin( ), m.size( )) ) {
                throw compiler_error{LEXIC_ERROR, {input.data( ), input.size( )}, "Lexic Error: Unknown token."};
            }
            /* Determina el tipo de token */
            token_type type = token_type(std::find_if(mt.begin( ), mt.end( ),[](const re2::StringPiece& s){
                return !s.empty( );
            }) - mt.begin( ));
            /* Insertalo al vector de tokens si no es un token de comentario o espacio o es el token de parada */
            if(type < END_OF_INPUT && type != stop) {
                tokens.push_back({ type, {mt[type].data( ), mt[type].length()} });
            }
        }
        /* Mueve la entrada hasta donde reconociste */
        auto len = (stop != END_OF_INPUT ? mt[stop].length( ) : 0);
        ini = input.data( ) - len;
        /* Inserta un token de END_OF_INPUT reemplazando siempre el token de parada */
        tokens.push_back({ END_OF_INPUT, {ini, len} }); 
        return tokens;
    }

    void add_literal_form(token_type t, const char* s) {
        token_forms[t].push_back(RE2::QuoteMeta(s));
    }

    lexer( ) {
        token_forms[PUBLIC_K] =       { RE2::QuoteMeta("public") };
        token_forms[PRIVATE_K] =      { RE2::QuoteMeta("private") };
        token_forms[INCLUDE_K] =      { RE2::QuoteMeta("include") };
        token_forms[OPERATOR_K] =     { RE2::QuoteMeta("operator") };
        token_forms[PREFIX_K] =       { RE2::QuoteMeta("prefix") };
        token_forms[SUFFIX_K] =       { RE2::QuoteMeta("suffix") };
        token_forms[INFIX_K] =        { RE2::QuoteMeta("infix") };
        token_forms[LEFT_K] =         { RE2::QuoteMeta("left") };
        token_forms[RIGHT_K] =        { RE2::QuoteMeta("right") };
        token_forms[AS_K] =           { RE2::QuoteMeta("as") };
        token_forms[PROC_K] =         { RE2::QuoteMeta("proc") };
        token_forms[VAR_K] =          { RE2::QuoteMeta("var") };
        token_forms[RETURN_K] =       { RE2::QuoteMeta("return") };
        token_forms[IF_K] =           { RE2::QuoteMeta("if") };
        token_forms[ELSE_K] =         { RE2::QuoteMeta("else") };
        token_forms[LPARENTHESIS_P] = { RE2::QuoteMeta("(") };
        token_forms[RPARENTHESIS_P] = { RE2::QuoteMeta(")") };
        token_forms[LBRACE_P] =       { RE2::QuoteMeta("{") };
        token_forms[RBRACE_P] =       { RE2::QuoteMeta("}") };
        token_forms[COMMA_P] =        { RE2::QuoteMeta(",") };
        token_forms[SEMICOLON_P] =    { RE2::QuoteMeta(";") };
        token_forms[ASSIGNMENT_O] =   { RE2::QuoteMeta(":=") };
        token_forms[NUMBER_L] =       { R"(\d+(?:\.\d*)?)", R"(\.\d+)" };
        token_forms[STRING_L] =       { R"(\"[^\"]*\")" };
        token_forms[OPERATOR_L] =     { R"([)" + RE2::QuoteMeta(R"(!#$%&'*+-./:<=>?@\^`|~)") + R"(]+)" };
        token_forms[IDENTIFIER_L] =   { R"(\w+)" };
    }
};

#endif

