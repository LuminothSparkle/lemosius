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
    struct visible_operator;
    struct visible_function;

    std::filesystem::path  source_path;
    std::vector<char>      source_file;
    std::vector<token>     header_tokens;
    std::vector<token>     program_tokens;
    syntax_tree            tree;
    std::vector<inclusion> inclusions;

    std::unordered_map<std::string_view, std::map<token_type, visible_operator>> operator_overloads;
    std::unordered_map<std::string_view, std::map<std::size_t, visible_function>> function_overloads;    // el std::size_t es el n�mero de par�metros de la funci�n
                                                                                                         // esto en realidad se usar� para la parte de la generaci�n de c�digo
                                                                                                         // que en un proyecto normal de gente decente, es lo que seguir�a del sem�ntico
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
    auto create_overload = [&overloads](bool access, const operator_declaration& op) {
       auto res = overloads[op.symbol.source].emplace(op.position.type, program_resources::visible_operator{access, op});
       if(!res.second) {
           using namespace std::string_literals;
           throw std::vector<std::pair<token,std::string>>({
                std::make_pair(op.symbol,                            "Operator overload already defined"s),
                std::make_pair(res.first->second.declaration.symbol, "Previously defined"s)
          });
       }
    };

    for(const auto& [inc_access, inc_pr] : incs) {
        for(const auto& [str_view, overload] : inc_pr.operator_overloads) {
            for(const auto& [pos, visible_op] : overload) {
               if (visible_op.access) {
                  create_overload(inc_access, visible_op.declaration);
               }
            }
        }
    }
    for(const auto& op : ops) {
        create_overload(is_public(op), op);
    }

    return overloads;
}

std::vector<std::string_view> get_operator_views(const decltype(program_resources::operator_overloads)& overloads) {
   std::vector<std::string_view> res;
   for(const auto& [str_view, overload] : overloads) {
      res.push_back(str_view);
   }
   return res;
}

#endif
