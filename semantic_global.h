#ifndef SEMANTIC_GLOBAL_H
#define SEMANTIC_GLOBAL_H

#include "semantic_types.h"

#include "semantic_utilities.h"
#include "semantic_local.h"

#include <cstdint>

#include <utility>
#include <vector>
#include <string>

auto generate_usables_operators( const std::vector<program_resources::inclusion>& incs, const std::vector<operator_declaration>& ops ) {
   decltype( program_resources::operator_overloads ) overloads;
   auto create_overload = [&overloads]( bool access, const operator_declaration & op ) {
      const auto& [it, is_inserted] = overloads[op.symbol->source].emplace( op.position->type, program_resources::visible_operator{access, &op} );
      if( !is_inserted ) {
         throw std::vector<std::pair<token, std::string>>( {
            { *op.symbol,                      "Operator overload already defined" },
            { *it->second.declaration->symbol, "Previously defined"                }
         } );
      }
   };
   for( const auto& [inc_access, inc_pr] : incs ) {
      for( const auto& [str_view, overload] : inc_pr.operator_overloads ) {
         for( const auto& [pos, visible_op] : overload ) {
            if( visible_op.access ) {
               create_overload( inc_access, *visible_op.declaration );
            }
         }
      }
   }
   for( const auto& op : ops ) {
      if( op.precedence != nullptr ) {
         get_representation<std::int32_t>( *op.precedence, "An operator precedence must be an integer between 0 to 4294967295" );
      }
      create_overload( is_public( op ), op );
   }
   return overloads;
}

auto generate_usables_functions( const std::vector<program_resources::inclusion>& incs, const std::vector<function_declaration>& funcs ) {
   decltype( program_resources::function_overloads ) overloads;

   auto create_overload = [&overloads]( bool access, const function_declaration & func ) {
      const auto& [it, is_inserted] = overloads[func.name->source].emplace( func.parameters.size(), program_resources::visible_function{access, &func} );
      if( !is_inserted ) {
         throw std::vector<std::pair<token, std::string>>( {
            { *func.name,                    "Function overload already defined" },
            { *it->second.declaration->name, "Previously defined"                }
         } );
      }
   };
   for( const auto& [inc_access, inc_pr] : incs ) {
      for( const auto& [str_view, overload] : inc_pr.function_overloads ) {
         for( const auto& [params, visible_func] : overload ) {
            if( visible_func.access ) {
               create_overload( inc_access, *visible_func.declaration );
            }
         }
      }
   }
   for( const auto& func : funcs ) {
      create_overload( is_public( func ), func );
   }
   return overloads;
}

auto get_operator_views( const decltype( program_resources::operator_overloads )& overloads ) {
   std::vector<std::string_view> res;
   for( const auto& [str_view, overload] : overloads ) {
      res.push_back( str_view );
   }
   return res;
}

auto get_operator_decls( const decltype( program_resources::operator_overloads )& overloads ) {
   std::vector<operator_declaration> res;
   for( const auto& [str_view, overload] : overloads ) {
      for( const auto& [pos, visible_op] : overload ) {
         res.push_back( *visible_op.declaration );
      }
   }
   return res;
}

void analyze_program( const program_resources& pr ) {
   for( const auto& [sv, overload] : pr.operator_overloads ) {
      for( const auto& [type, vop] : overload ) {
         const auto& [access, decl] = vop;
         auto it = pr.function_overloads.find( decl->function->source );
         if( it == pr.function_overloads.end() || ( type == INFIX_K && !it->second.contains( 2 ) ) || ( type != INFIX_K && !it->second.contains( 1 ) ) ) {
            throw std::pair<token, std::string>( *decl->function, "Operator handler function doesn't exist." );
         }
      }
   }
   scope_stack global( pr );
   for( const auto& f : pr.tree.functions ) {
      analyze_function( f, global );
   }
}



#endif
