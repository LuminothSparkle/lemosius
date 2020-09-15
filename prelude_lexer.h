#ifndef PRELUDE_LEXER_H
#define PRELUDE_LEXER_H
#include<re2/re2.h>
#include<re2/stringpiece.h>

enum token {
	NEW_LINE,
	END_OF_INPUT,	
	INCLUDE_K,
	OPERATOR_K,
	PRIVATE_K,
	PUBLIC_K,
	INFIX_K,
	SUFFIX_K,
	PREFIX_K,
	LEFT_K,
	RIGHT_K,
	AS_K,		
	LEFT_PARENTESIS,
	RIGHT_PARENTESIS,
	OPERATOR,
	STRING,	
	NUMBER,
	IDENTIFIER,
	UNKNOWN
};

struct token_anotada {
	token type;
	std::string_view original;
};

std::vector<token_anotada> prelude_lexer(const char*& ini) {
	re2::StringPiece input( ini );
	const RE2 e("[\\t\\f\\r ]*(?:(?:(?:(?://|#)[^\\n]*|/\\*.*?\\*/)?(\\n))+|(proc)|(include)|(operator)|(private)|(public)|(infix)|(suffix)|(prefix)|(left)|(right)|(as)|([(])|([)])|([!+\\-*/.^])|(\".*?\")|(\\d+\\.?\\d*)|(\\w+)|(\\S*))");

	re2::StringPiece mt[UNKNOWN - NEW_LINE + 1];
	RE2::Arg args[UNKNOWN - NEW_LINE + 1];
	const RE2::Arg* m[UNKNOWN - NEW_LINE + 1];
	for( unsigned i = 0; i <= UNKNOWN - NEW_LINE; ++i ) {
		args[i] = mt + i;
		m[i] = args + i;
	}

	std::vector<token_anotada> tokens;
	while( RE2::FindAndConsumeN(&input, e, m, UNKNOWN - NEW_LINE + 1) && mt[END_OF_INPUT - NEW_LINE].empty() ) {
		for( unsigned i = 0; i < UNKNOWN - NEW_LINE; ++i ) {
			if( !mt[i].empty() ) {
				tokens.push_back( { (token)(NEW_LINE + i), std::string_view( mt[i].data(), mt[i].length() ) } );
				mt[i].remove_suffix(mt[i].length());
			}
		}
		if(!mt[UNKNOWN - NEW_LINE].empty()) {
			throw std::make_pair( mt[UNKNOWN - NEW_LINE], "Lexic Error" );
		}
	}
	ini = input.data() - mt[END_OF_INPUT - NEW_LINE].length();
	return tokens;
}

#endif

