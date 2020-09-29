#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"lexer.h"
#include"parser.h"
#include"semantic_utilities.h"

#include<vector>
#include<filesystem>

struct program_resources {
    struct inclusion;
    struct usable_operator;
    std::filesystem::path source_path;
    std::vector<char>     source_file;
    std::vector<token>    header_tokens;
    std::vector<token>    program_tokens;
    syntax_tree           tree;

    std::vector<inclusion>       inclusions;
    std::vector<usable_operator> operators;
};

struct program_resources::inclusion {
   bool access;
   program_resources resources;
};

struct program_resources::usable_operator {
   bool access;
   operator_declaration declaration;
};

#endif
