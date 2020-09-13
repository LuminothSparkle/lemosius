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
	if(peek(t) == PRI_K) {
		match(t,PRI_K);
	}
	else if(peek(t) == PUB_K) {
		match(t,PUB_K);
	}
}

void inc_dec(it_tok& t) {
	match(t,INC_K);
	match(t,STR);
	match(t,NL);
}

void op_aso_esp(it_tok& t) {
	if(peek(t) == L_K) {
		match(t,L_K);
	}
	else if(peek(t) == R_K) {
		match(t,R_K);
	}
	else {
		throw std::make_pair(*t,"Syntax error");
	}
}

void op_pre_esp(it_tok& t) {
	match(t,NUM);
}


void op_pos_esp(it_tok& t) {
	if(peek(t) == PRE_K) {
		match(t,PRE_K);
	}
	else if(peek(t) == SUF_K) {
		match(t,SUF_K);
	}
	else if(peek(t) == INF_K) {
		match(t,INF_K);
		match(t,L_PAR);
		op_aso_esp(t);
		op_pre_esp(t);
		match(t,R_PAR);
	}
	else {
		throw std::make_pair(*t,"Syntax error");
	}
}

void op_dec(it_tok& t) {
	match(t,OP_K);
	match(t,OP);
	op_pos_esp(t);
	match(t,AS_K);
	match(t,ID);
	match(t,NL);
}

void inc_head(it_tok& t) {
	if(peek(t) == PRI_K || peek(t) == PUB_K) {
		vis_esp(t);
		inc_dec(t);
		inc_head(t);
	}
	else if(peek(t) == INC_K) {
		inc_dec(t);
		inc_head(t);
	}
	else if(peek(t) == NL) {
		match(t,NL);
		inc_head(t);
	}
}

void op_head(it_tok& t) {
	if(peek(t) == PRI_K || peek(t) == PUB_K) {
		vis_esp(t);
		op_dec(t);
		op_head(t);
	}
	else if(peek(t) == OP_K) {
		op_dec(t);
		op_head(t);
	}
	else if(peek(t) == NL) {
		match(t,NL);
		op_head(t);
	}
}

void header(it_tok& t) {	
	inc_head(t);
	op_head(t);
}
