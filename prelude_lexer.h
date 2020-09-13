enum token {
	NL,
	EOI,	
	INC_K,
	OP_K,
	PRI_K,
	PUB_K,
	INF_K,
	SUF_K,
	PRE_K,
	L_K,
	R_K,
	AS_K,		
	L_PAR,
	R_PAR,
	OP,
	STR,	
	NUM,
	ID,
	UK
};

struct token_anotada {
	token type;
	std::string_view original;
};

std::vector<token_anotada> prelude_lexer(const char*& ini) {
	re2::StringPiece input( ini );
	const RE2 e("[\\t\\f\\r ]*(?:(?:(?:(?://|#)[^\\n]*|/\\*.*?\\*/)?(\\n))+|(proc)|(include)|(operator)|(private)|(public)|(infix)|(suffix)|(prefix)|(left)|(right)|(as)|([(])|([)])|([!+\\-*/.^])|(\".*?\")|(\\d+\\.?\\d*)|(\\w+)|(\\S*))");

	re2::StringPiece mt[UK - NL + 1];
	RE2::Arg args[UK - NL + 1];
	const RE2::Arg* m[UK - NL + 1];
	for( unsigned i = 0; i <= UK - NL; ++i ) {
		args[i] = mt + i;
		m[i] = args + i;
	}

	std::vector<token_anotada> tokens;
	while( RE2::FindAndConsumeN(&input, e, m, UK - NL + 1) && mt[EOI - NL].empty() ) {
		for( unsigned i = 0; i < UK - NL; ++i ) {
			if( !mt[i].empty() ) {
				tokens.push_back( { (token)(NL + i), std::string_view( mt[i].data(), mt[i].length() ) } );
				mt[i].remove_suffix(mt[i].length());
			}
		}
		if(!mt[UK - NL].empty()) {
			throw std::make_pair( mt[UK - NL], "Lexic Error" );
		}
	}
	ini = input.data() - mt[EOI - NL].length();
	return tokens;
}

