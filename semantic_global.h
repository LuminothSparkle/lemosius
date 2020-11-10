#ifndef SEMANTIC_GLOBAL_H
#define SEMANTIC_GLOBAL_H

#include "semantic_types.h"
#include "semantic_utilities.h"
#include "semantic_local.h"

#include <cstdint>

#include <utility>
#include <vector>
#include <string>

auto generate_usables_operators( const std::vector<inclusion>& incs, const std::vector<operator_declaration>& ops ) {
   decltype( program_resources::operator_overloads ) overloads;
   auto create_overload = [&overloads]( bool access, const operator_declaration & new_op ) {
      const auto& [iter, is_inserted] = overloads[ *new_op.symbol ].insert( { new_op.position->type, { access, &new_op } } );
      if( !is_inserted ) {
         const auto& prev_op = *iter->second.declaration;
         throw std::vector<std::pair<token, std::string>>( {
            { *new_op.symbol,  "Operator overload already defined" },
            { *prev_op.symbol, "Previously defined"                }
         } );
      }
   };
   for( const auto& [inc_access, inc_pr] : incs ) {
      for( const auto& [str_view, overload] : inc_pr.operator_overloads ) {
         for( const auto& [pos, vop] : overload ) {
            if( vop.access ) {
               create_overload( inc_access, *vop.declaration );
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

auto generate_usables_functions( const std::vector<inclusion>& incs, const std::vector<function_declaration>& funcs ) {
   decltype( program_resources::function_overloads ) overloads;
   auto create_overload = [&overloads]( bool access, const function_declaration & new_func ) {
      const auto& [iter, is_inserted] = overloads[ *new_func.name ].insert( { new_func.parameters.size( ), { access, &new_func } } );
      if( !is_inserted ) {
         const auto& prev_func = *iter->second.declaration;
         throw std::vector<std::pair<token, std::string>>( {
            { *new_func.name,  "Function overload already defined" },
            { *prev_func.name, "Previously defined"                }
         } );
      }
   };
   for( const auto& [inc_access, inc_pr] : incs ) {
      for( const auto& [str_view, overload] : inc_pr.function_overloads ) {
         for( const auto& [params, vfunc] : overload ) {
            if( vfunc.access ) {
               create_overload( inc_access, *vfunc.declaration );
            }
         }
      }
   }
   for( const auto& func : funcs ) {
      create_overload( is_public( func ), func );
   }
   return overloads;
}

resolution_table analyze_program( const program_resources& pr ) {
   scope_stack global( pr );
   resolution_table tbl{pr.operator_overloads};

   for( const auto& [sv, overload] : pr.operator_overloads ) {
      for( const auto& [type, vop] : overload ) {
         const auto& handler_func = *vop.declaration->function;
         const auto  arity        = type == INFIX_K ? 2 : 1;
         if (auto [func_decl, exists] = global.find_symbol( handler_func ).get_overload( arity ); exists) {
            tbl.operator_lookup[sv].emplace(type, func_decl);
         } else {
            throw std::pair<token, std::string>( handler_func, "Operator handler function doesn't exist." );
         }
      }
   }
   for( const auto& f : pr.tree.functions ) {
      analyze_function( f, global, tbl);
   }

   return tbl;
}



#endif
