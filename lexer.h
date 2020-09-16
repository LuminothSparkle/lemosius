#ifndef LEXER_H
#define LEXER_H
#include<re2/re2.h>
#include<re2/stringpiece.h>
#include<numeric>
#include<algorithm>
#include"token.h"

//Crei que seria mas conveniente tener un lexer como estructura o clase ya que se modificaran sus valores las formas de tokens para los analisis posterior.
struct lexer {
	//Considere que no era necesario tener una estructura con los dos datos, creo que solo basta tener las caras, al final nunca buscaremos al reves de caras hacia tipos de tokens.
	std::array<std::vector<std::string>, END_OF_INPUT + 1> token_forms;
	
	void init_default_token_forms( ) {
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
		token_forms[LPARENTHESIS_P] = { RE2::QuoteMeta("(") };
		token_forms[RPARENTHESIS_P] = { RE2::QuoteMeta(")") };
		token_forms[LBRACE_P] =       { RE2::QuoteMeta("{") };
		token_forms[RBRACE_P] =       { RE2::QuoteMeta("}") };
		token_forms[COMMA_P] =        { RE2::QuoteMeta(",") };
		token_forms[SEMICOLON_P] =    { RE2::QuoteMeta(";") };
		token_forms[ASSIGNMENT_O] =   { RE2::QuoteMeta(":=") };
		token_forms[NUMBER_L] =       { R"(\d+(?:\.\d*)?)", R"(\.\d+)" };
		token_forms[STRING_L] =       { R"(\".*?\")" };
		token_forms[OPERATOR_L] =     { R"([)" + RE2::QuoteMeta(R"(!#$%&'*+-./:<=>?@\^`|~)") + R"(]+)" };
		token_forms[IDENTIFIER_L] =   { R"(\w+)" };
		token_forms[END_OF_INPUT] =   { R"(\x00)" }; //Faltaba un 0, si compilaba pero no mandaba error al crear la cadena
	}

	RE2 generate_expresion( ) {
		//Join que junta una cadena de strings agregandole un separado un prefijo y un sufijo
		auto join = [&](const std::vector<std::string>& v, const char* sep = "", const char* prefix = "", const char* suffix = "") {
			std::string res = prefix;			
			for (unsigned i = 0; i < v.size( ); ++i) {
			 	res += v[i] + (i + 1 < v.size( ) ? sep : suffix);
		      	}
			return res;
		};
		std::vector<std::string> parts(token_forms.size( ) + 1);
		parts[0] = R"(\s+|\#\#[^\n]*|\#\#\#.*?\#\#\#)"; //Dado que # lo queremos considerar como operador al menos, utilizo ## y ### para los comentarios, dudo que se lleguen a usar como operadores así pero no se que opines.
		std::transform(token_forms.begin( ),token_forms.end( ),parts.begin( ) + 1,[&](const std::vector<std::string>& t) {
			return join(t,"|","(",")");
		});
		return RE2(join(parts,"|"));
	}

	std::vector<token> analisis(const char*& ini, token_type stop) {
		RE2 e = generate_expresion( ); //Genero la expresion cada que se llama el analisis, podria cambiar eso pero creo que la cadena va a cambiar dado que agregaremos caras o formas a los tokens.
		re2::StringPiece input( ini );
		std::array< re2::StringPiece, END_OF_INPUT + 1 > mt;
		std::array< RE2::Arg, END_OF_INPUT + 1 > args;
		std::array< const RE2::Arg*, END_OF_INPUT + 1 > m;
		std::transform(mt.begin( ), mt.end( ), args.begin( ), [&](re2::StringPiece& s){
			return &s;
		});
		std::transform(args.begin( ), args.end( ), m.begin( ), [&](RE2::Arg& a){
			return &a;
		});
		std::vector<token> tokens;
		do {
			if( !RE2::FindAndConsumeN(&input, e, m.begin( ), m.size( )) ) {
				//Aqui solo entra si no llega coincidir ninguna de las cadenas y lanzo un UNKNOWN
				throw std::make_pair( 
				          token( {UNKNOWN, std::string_view(input.data( ),input.length( ))} ),
				          "Lexic Error"
				      );
			}
			token_type type = token_type(std::find_if(mt.begin( ), mt.end( ),[&](const re2::StringPiece& s){
				return !s.empty( );
			}) - mt.begin( ));
			//Dado que no queria agregar los comentarios y los espacios como tokens, el regex si acepta las cadenas pero el todos los tokens estan vacios por lo que agregue estas condiciones además de que para que pare el ciclo el token de parada no debe estar vacio.
			if(type <= END_OF_INPUT && type != stop) {
				tokens.push_back( {type, std::string_view( mt[type].data( ), mt[type].length() )} );
				mt[type].remove_suffix(mt[type].length( ));
			}
		}while( mt[stop].empty( ) );
		tokens.push_back( {END_OF_INPUT, std::string_view( mt[stop].data( ), mt[stop].length() )} ); //Agrego un end of input sin importar cual sea el token de parada, supongo que pueden ayudar para el parser que siempre sea end of input el ultimo token
		ini = input.data( ) - mt[stop].length( );
		return tokens;
	}

	void add_literal_form(token_type t, const char* s) {
		token_forms[t].push_back(RE2::QuoteMeta(s));
	}

	lexer( ) {
		init_default_token_forms( );
	}

};

#endif

