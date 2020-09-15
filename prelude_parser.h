#ifndef PRELUDE_PARSER_H
#define PRELUDE_PARSER_H
#include<vector>
#include"prelude_lexer.h"

using it_tok = std::vector<token_anotada>::iterator;

token peek(const it_tok& t) {
	return t->type;
}

void match(it_tok& t, token type) {
	if(peek(t) != type) {
		throw std::make_pair(*t,"Syntax Error");
	}
	t++;
}

void vis_esp(it_tok& t) {
	if(peek(t) == PRIVATE_K) {
		match(t,PRIVATE_K);
	}
	else if(peek(t) == PUBLIC_K) {
		match(t,PUBLIC_K);
	}
}

void inc_dec(it_tok& t) {
	match(t,INCLUDE_K);
	match(t,STRING);
	match(t,NEW_LINE);
}

void op_aso_esp(it_tok& t) {
	if(peek(t) == LEFT_K) {
		match(t,LEFT_K);
	}
	else if(peek(t) == RIGHT_K) {
		match(t,RIGHT_K);
	}
	else {
		throw std::make_pair(*t,"Syntax error");
	}
}

void op_pre_esp(it_tok& t) {
	match(t,NUMBER);
}


void op_pos_esp(it_tok& t) {
	if(peek(t) == PREFIX_K) {
		match(t,PREFIX_K);
	}
	else if(peek(t) == SUFFIX_K) {
		match(t,SUFFIX_K);
	}
	else if(peek(t) == INFIX_K) {
		match(t,INFIX_K);
		match(t,LEFT_PARENTESIS);
		op_aso_esp(t);
		op_pre_esp(t);
		match(t,RIGHT_PARENTESIS);
	}
	else {
		throw std::make_pair(*t,"Syntax error");
	}
}

void op_dec(it_tok& t) {
	match(t,OPERATOR_K);
	match(t,OPERATOR);
	op_pos_esp(t);
	match(t,AS_K);
	match(t,IDENTIFIER);
	match(t,NEW_LINE);
}

void inc_head(it_tok& t) {
	if(peek(t) == PRIVATE_K || peek(t) == PUBLIC_K) {
		vis_esp(t);
		inc_dec(t);
		inc_head(t);
	}
	else if(peek(t) == INCLUDE_K) {
		inc_dec(t);
		inc_head(t);
	}
	else if(peek(t) == NEW_LINE) {
		match(t,NEW_LINE);
		inc_head(t);
	}
}

void op_head(it_tok& t) {
	if(peek(t) == PRIVATE_K || peek(t) == PUBLIC_K) {
		vis_esp(t);
		op_dec(t);
		op_head(t);
	}
	else if(peek(t) == OPERATOR_K) {
		op_dec(t);
		op_head(t);
	}
	else if(peek(t) == NEW_LINE) {
		match(t,NEW_LINE);
		op_head(t);
	}
}

void header(it_tok& t) {	
	inc_head(t);
	op_head(t);
}

#endif
