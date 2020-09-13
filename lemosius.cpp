#include<iostream>
#include<fstream>
#include<re2/re2.h>
#include<re2/stringpiece.h>

enum token {
	FIRST,
	COMMENT,
	END_PRELUDE,	
	INCLUDE_KEYWORD,
	VISIBILITY_KEYWORD,
	OPERATOR_KEYWORD,	
	POSITION_KEYWORD,
	ASOCIATIVITY_KEYWORD,
	AS_KEYWORD,		
	LEFT_PARENTESIS,
	RIGHT_PARENTESIS,
	OPERATOR,
	STRING_LITERAL,	
	NUMBER_LITERAL,
	IDENTIFIER,
	LAST
};

struct token_preludio {
	token type;
	std::string_view fuente;
	token_preludio(token type, std::string_view fuente) : type(type), fuente(fuente) {};
};

std::vector<token_preludio> lexer_preludio(const char*& ini) {
	re2::StringPiece input( ini );
	const RE2 expresion("\\s*(?:(#[^\\n\\r]*)|(proc)|(include)|(private)|(operator)|(suffix|infix|prefix)|(left|right)|(as)|([(])|([)])|([!+\\^])|(\".*?\")|(\\d+\\.?\\d*)|(\\w+))");

	re2::StringPiece type_tokens[LAST - FIRST - 1] = {re2::StringPiece(ini,0)};
	RE2::Arg args[LAST - FIRST - 1];
	const RE2::Arg* matches[LAST - FIRST - 1];
	for(int i = 0; i < LAST - FIRST - 1; ++i) {
		args[i] = type_tokens + i;
		matches[i] = args + i;
	}

	std::vector<token_preludio> tokens;
	while( RE2::FindAndConsumeN(&input, expresion, matches, LAST - FIRST - 1) && type_tokens[END_PRELUDE - FIRST - 1].empty() ) {
		for( int i = 0; i < LAST - FIRST - 1; ++i ) {
			if( !type_tokens[i].empty() ) {
				tokens.push_back( token_preludio( (token)(i + FIRST + 1), std::string_view( type_tokens[i].data(), type_tokens[i].length() ) ) );
				type_tokens[i].remove_suffix(type_tokens[i].length());
			}
		}
	}
	ini = input.data() - type_tokens[END_PRELUDE - FIRST - 1].length();
	return tokens;
}

std::string read_file(const char* path) {
	std::ifstream input(path);
	input.seekg(0,std::ios_base::end);
	std::streampos length(input.tellg());
	input.seekg(0,std::ios_base::beg);
	std::string buffer(length,'\0');
	input.read(&buffer[0],length);
	return buffer;
}

int main(int argc, char *argv[]) {
	std::string text = read_file( argv[1] );
	const char* ini = text.c_str();
	std::vector<token_preludio> preludio = lexer_preludio( ini );
	for(auto& token : preludio) {
		std::cout << token.type << "\n";
		std::cout << token.fuente << "\n";	
	}
}
