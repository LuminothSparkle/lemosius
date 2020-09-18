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

//Crei que seria mas conveniente tener un lexer como estructura o clase ya que se modificaran sus valores las formas de tokens para los analisis posterior.
struct lexer {
    //Considere que no era necesario tener una estructura con los dos datos, creo que solo basta tener las caras, al final nunca buscaremos al reves de caras hacia tipos de tokens.
    std::array<std::vector<std::string>, END_OF_INPUT> token_forms;

    RE2 generate_expresion( ) const {
        //Join que junta una cadena de strings agregandole un separado un prefijo y un sufijo
        auto join = [&](const std::vector<std::string>& v, const char* sep = "", const char* prefix = "", const char* suffix = "") {
            std::string res = prefix;
            for (std::size_t i = 0; i < v.size( ); ++i) {
                res += v[i] + (i + 1 < v.size( ) ? sep : suffix);
            }
            return res;
        };
        std::vector<std::string> parts(token_forms.size( ) + 1);
        parts[0] = R"(\s+|\#\#\#[^\#]*\#\#\#|\#\#[^\#\n]*)"; //Dado que # lo queremos considerar como operador al menos, utilizo ## y ### para los comentarios, dudo que se lleguen a usar como operadores así pero no se que opines.
        std::transform(token_forms.begin( ),token_forms.end( ),parts.begin( ) + 1,[&](const std::vector<std::string>& t) {
            return join(t,"|","(",")");
        });
        return RE2(join(parts,"|"));
    }

    std::vector<token> analisis(const char*& ini, token_type stop) const  {
        RE2 e = generate_expresion( ); //Genero la expresion cada que se llama el analisis, podria cambiar eso pero creo que la cadena va a cambiar dado que agregaremos caras o formas a los tokens.
        re2::StringPiece input( ini );
        std::array< re2::StringPiece, END_OF_INPUT > mt;
        std::array< RE2::Arg, END_OF_INPUT > args;
        std::array< const RE2::Arg*, END_OF_INPUT > m;
        std::transform(mt.begin( ), mt.end( ), args.begin( ), [&](re2::StringPiece& s){return &s;});
        std::transform(args.begin( ), args.end( ), m.begin( ), [&](RE2::Arg& a){return &a;});
        std::vector<token> tokens;
        while( !input.empty( ) && (stop == END_OF_INPUT || mt[stop].empty( )) ) {
            if( !RE2::FindAndConsumeN(&input, e, m.begin( ), m.size( )) ) {
                //Aqui solo entra si no llega coincidir ninguna de las cadenas y lanzo un UNKNOWN
                throw std::make_pair( 
                        token{ UNKNOWN, {input.data( ),input.length( )} },
                        "Lexic Error"
                      );
            }
            //El porque daba igual END_OF_INPUT es aqui, porque al se un caracter vacio simplemente se tomaba como vacia y no podia hacer distincion de ella y de los comentarios aun si los comentarios no se capturaban
            token_type type = token_type(std::find_if(mt.begin( ), mt.end( ),[&](const re2::StringPiece& s){
                return !s.empty( );
            }) - mt.begin( ));
            //Dado que no queria agregar los comentarios y los espacios como tokens, el regex si acepta las cadenas pero el todos los tokens estan vacios por lo que agregue estas condiciones además de que para que pare el ciclo el token de parada no debe estar vacio.
            if(type < END_OF_INPUT && type != stop) {
                tokens.push_back( { type, {mt[type].data( ), mt[type].length()} } );
            }
        }
        //Mejor agrego un END_OF_INPUT al final cuando llego al token de parada o se acaba la entrada.
        tokens.push_back( { END_OF_INPUT, {mt[stop].data( ), mt[stop].length()} } ); 
        ini = input.data( ) - mt[stop].length( );
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
        //Asi como estaba el codigo daba igual si existia END_OF_INPUT y de hecho no hacia diferencia incluyendolo, creo que es mejor si lo quito y es más un tipo de token especial para denotar el final sin importar su representacion.
    }

};

#endif

