#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"lexer.h"
#include"parser.h"
#include"semantic_utilities.h"

#include<cstdint>
#include<vector>
#include<filesystem>
#include<unordered_map>
#include<map>
#include<utility>
#include<string>
#include<algorithm>
#include<iterator>
#include<array>

struct program_resources {
    struct inclusion;
    struct visible_operator;
    struct visible_function;

    std::filesystem::path source_path;
    std::vector<char>     source_file;
    std::vector<token>    header_tokens;
    std::vector<token>    program_tokens;
    syntax_tree           tree;

    std::vector<inclusion> inclusions;
    std::unordered_map<std::string_view, std::map<token_type,  visible_operator>> operator_overloads;
    std::unordered_map<std::string_view, std::map<std::size_t, visible_function>> function_overloads;
};

struct program_resources::inclusion {
    bool              access;
    program_resources resources;
};

struct program_resources::visible_operator {
   bool                 access;
   operator_declaration declaration;
};

struct program_resources::visible_function {
   bool                 access;
   function_declaration declaration;
};

auto generate_usables_operators(const std::vector<program_resources::inclusion>& incs, const std::vector<operator_declaration>& ops) {
    decltype(program_resources::operator_overloads) overloads;
    using namespace std::string_literals;
    auto create_overload = [&overloads] (bool access, const operator_declaration& op) {
       const auto& [it, is_inserted] = overloads[op.symbol.source].emplace(op.position.type, program_resources::visible_operator{access, op});
       if(!is_inserted) {
            throw std::vector<std::pair<token,std::string>>({
                std::make_pair(op.symbol,                     "Operator overload already defined"s),
                std::make_pair(it->second.declaration.symbol, "Previously defined"s)
            });
       }
    };
    for(const auto& [inc_access, inc_pr] : incs) {
        for(const auto& [str_view, overload] : inc_pr.operator_overloads) {
            for(const auto& [pos, visible_op] : overload) {
                if(visible_op.access) {
                    create_overload(inc_access, visible_op.declaration);
                }
            }
        }
    }
    for(const auto& op : ops) {
        if (op.precedence != nullptr && !get_representation<std::int32_t>(op.precedence->source).second) {
           throw std::make_pair(*op.precedence, "Semantic Error: An operator precedence must be an integer between 0 to 4294967295"s );
        }
        create_overload(is_public(op), op);
    }
    return overloads;
}

auto get_operator_views(const decltype(program_resources::operator_overloads)& overloads) {
    std::vector<std::string_view> res;
    for(const auto& [str_view, overload] : overloads) {
        res.push_back(str_view);
    }
    return res;
}

auto get_operator_decls(const decltype(program_resources::operator_overloads)& overloads) {
    std::vector<operator_declaration> res;
    for(const auto& [str_view, overload] : overloads) {
        for(const auto& [pos, visible_op] : overload) {
           res.push_back(visible_op.declaration);
        }
    }
    return res;
}

#endif
