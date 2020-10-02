#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"lexer.h"
#include"parser.h"
#include"semantic_utilities.h"

#include<vector>
#include<filesystem>
#include<set>
#include<utility>
#include<string>
#include<algorithm>
#include<iterator>
#include<array>

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
   bool              access;
   program_resources resources;
};

struct program_resources::usable_operator {
   bool                 access;
   operator_declaration declaration;
};

void analyze_operators(program_resources& pr) {
    std::map<std::pair<std::string,token_type>,token> overloads;
    auto check_overload = [&overloads](const operator_declaration& op) {
        auto res = overloads.emplace(std::make_pair(op.symbol.source, op.position.type),op.symbol);
        if(!res.second) {
            using namespace std::string_literals;
            throw std::vector<std::pair<token,std::string>>({
                    std::make_pair(op.symbol,         "Operator overload already defined"s),
                    std::make_pair(res.first->second, "Previously defined"s)
            });
        }
    };
    for(const auto& [inc_access, inc_pr] : pr.inclusions) {
        for(const auto& [op_access, decl] : inc_pr.operators) {
            check_overload(decl);
            pr.operators.emplace_back(op_access && inc_access, decl);
        }
    }
    for(const auto& op : pr.tree.operators) {
        check_overload(op);
        pr.operators.emplace_back(is_public(op), op);
    }
}

#endif
