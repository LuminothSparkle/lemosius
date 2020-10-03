#ifndef SEMANTIC_H
#define SEMANTIC_H

#include"lexer.h"
#include"parser.h"
#include"semantic_utilities.h"

#include<re2/re2.h>

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
    using usable_operators = std::unordered_map<operator_declaration,bool,hash_operator_declaration>;

    std::filesystem::path source_path;
    std::vector<char>     source_file;
    std::vector<token>    header_tokens;
    std::vector<token>    program_tokens;
    syntax_tree           tree;

    std::vector<inclusion> inclusions;
    usable_operators       operators;
};

struct program_resources::inclusion {
    bool              access;
    program_resources resources;
};

auto generate_usables_operators(const std::vector<program_resources::inclusion>& incs, const std::vector<operator_declaration>& ops) {
    program_resources::usable_operators usables;
    std::map<std::pair<std::string,token_type>,token> overloads;
    auto check_overload = [&overloads](const operator_declaration& op) {
        auto [it, is_inserted] = overloads.try_emplace(std::make_pair(op.symbol.str(), op.position.type),op.symbol);
        if(!is_inserted) {
            using namespace std::string_literals;
            throw std::vector<std::pair<token,std::string>>({
                    std::make_pair(op.symbol,  "Operator overload already defined"s),
                    std::make_pair(it->second, "Previously defined"s)
            });
        }
    };
    for(const auto& [inc_access, inc_pr] : incs) {
        for(const auto& [decl, op_access] : inc_pr.operators) {
            check_overload(decl);
            usables.try_emplace(decl, op_access && inc_access);
        }
    }
    for(const auto& op : ops) {
        check_overload(op);
        usables.try_emplace(op, is_public(op));
    }
    return usables;
}

auto generate_operator_forms(const program_resources::usable_operators& ops) {
    auto compare = [](const auto& s1, const auto& s2) {
        return s1.size() > s2.size() || s1 < s2;    // Los operadores largos tienen prioridad de identificarse que los cortos
    };                                              // por ejemplo ++ puede ser visto como dos operadores + y creo que eso genera más problemas
    std::set<std::string,decltype(compare)> set_forms;
    std::transform(ops.begin(), ops.end(), std::inserter(set_forms,set_forms.begin()),
    [](const auto& pair_op){
        return RE2::QuoteMeta(pair_op.first.symbol.str());
    });
    std::vector<std::string> forms;
    while(!set_forms.empty()) {
        forms.push_back(std::move(set_forms.extract(set_forms.begin()).value()));
    }
    return forms;
}

#endif
