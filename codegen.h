#ifndef CODEGEN_H
#define CODEGEN_H

#include "semantic_types.h"

#include <ostream>
#include <string>

void write_expression( const expression&, const resolution_table&, std::ostream& );

void write_expression( const terminal_expression& e, const resolution_table& tbl, std::ostream& os) {
   if( *e.tok_ptr == IDENTIFIER_L) {
      os << e.tok_ptr->source << "_" << tbl.variable_lookup.find(e.tok_ptr)->second;
   } else {
      os << "double(" << e.tok_ptr->source << ")";
   }
}

void write_expression( const prefix_expression& e, const resolution_table& tbl, std::ostream& os) {
   os << tbl.operator_overloads.find(e.op->source)->second.find(PREFIX_K)->second.declaration->symbol->source;
   if (auto vis_func = tbl.function_lookup.find(e.op)->second; vis_func != nullptr) {
      os << "_" << vis_func;
   }
   os << "(";
   write_expression(*e.exp, tbl, os);
   os << ")";
}

void write_expression( const suffix_expression& e, const resolution_table& tbl, std::ostream& os) {
   os << tbl.operator_overloads.find(e.op->source)->second.find(SUFFIX_K)->second.declaration->symbol->source;
   if (auto vis_func = tbl.function_lookup.find(e.op)->second; vis_func != nullptr) {
      os << "_" << vis_func;
   }
   os << "(";
   write_expression(*e.exp, tbl, os);
   os << ")";
}

void write_expression( const binary_expression& e, const resolution_table& tbl, std::ostream& os) {
   os << tbl.operator_overloads.find(e.op->source)->second.find(INFIX_K)->second.declaration->symbol->source;
   if (auto vis_func = tbl.function_lookup.find(e.op)->second; vis_func != nullptr) {
      os << "_" << vis_func;
   }
   os << "(";
   write_expression(*e.exp1, tbl, os);
   os << ", ";
   write_expression(*e.exp1, tbl, os);
   os << ")";
}

void write_expression( const call_expression& e, const resolution_table& tbl, std::ostream& os) {
   os << e.function_name->source;
   if (auto vis_func = tbl.function_lookup.find(e.function_name)->second; vis_func != nullptr) {
      os << "_" << vis_func;
   }
   os << "(";
   for (int i = 0; i < e.params.size( ); ++i) {
      write_expression(*e.params[i], tbl, os);
      if (i + 1 < e.params.size( )) {
         os << ",";
      }
   }
   os << ")";
}

void write_expression( const expression& e, const resolution_table& tbl, std::ostream& os) {       // sí, if else if !
   if( typeid( e ) == typeid( terminal_expression ) ) {
      return write_expression( dynamic_cast<const terminal_expression&>( e ), tbl, os);
   } else if( typeid( e ) == typeid( prefix_expression ) ) {
      return write_expression( dynamic_cast<const prefix_expression&>( e ),   tbl, os);
   } else if( typeid( e ) == typeid( suffix_expression ) ) {
      return write_expression( dynamic_cast<const suffix_expression&>( e ),   tbl, os);
   } else if( typeid( e ) == typeid( binary_expression ) ) {
      return write_expression( dynamic_cast<const binary_expression&>( e ),   tbl, os);
   } else if( typeid( e ) == typeid( call_expression ) ) {
      return write_expression( dynamic_cast<const call_expression&>( e ),     tbl, os);
   }
}

void write_statement( const statement&, const resolution_table&, std::ostream& );

void write_statement( const sequence_statement& s, const resolution_table& tbl, std::ostream& os) {
   for( const auto& stmt : s.body ) {
      write_statement( *stmt, tbl, os);
   }
}

void write_statement( const expression_statement& s, const resolution_table& tbl, std::ostream& os) {
   if( s.body != nullptr ) {
      write_expression( *s.body, tbl, os);
   }
   os << ";\n";
}

void write_statement( const if_statement& s, const resolution_table& tbl, std::ostream& os) {
   /*
      la estructura del if_statement está fea :( hubiera sido mejor algo así:

         struct if_statement : statement {
            std::unique_ptr<sequence_statement> condition;
            std::vector<std::unique_ptr<expression>> body_if;
            std::vector<std::unique_ptr<expression>> body_else;
         };

      donde un "else if" se guarda como un if_statement como único elemento en body_else ya que, a final de cuentas

         if cosa1 {

         } else if cosa2 {

         } else {

         }

      es equivalente a

         if cosa1 {

         } else {
            if cosa2 {

            } else {

            }
         }
   */

   // éste te toca
}

void write_statement( const var_statement& s, const resolution_table& tbl, std::ostream& os) {
   os << "double " << s.name->source << "_" << s.name;
   if( s.value != nullptr ) {
      os << " = ";
      write_expression( *s.value, tbl, os);
   }
   os << ";\n";
}

void write_statement( const return_statement& s, const resolution_table& tbl, std::ostream& os) {
   os << "return ";
   if( s.return_value != nullptr ) {
      write_expression( *s.return_value, tbl, os);
   } else {
      os << "0";
   }
   os << ";\n";
}

void write_statement( const statement& s, const resolution_table& tbl, std::ostream& os) {      // sí, if else if !
   if( typeid( s ) == typeid( sequence_statement ) ) {
      return write_statement( dynamic_cast<const sequence_statement&>( s ),   tbl, os);
   } else if( typeid( s ) == typeid( expression_statement ) ) {
      return write_statement( dynamic_cast<const expression_statement&>( s ), tbl, os);
   } else if( typeid( s ) == typeid( if_statement ) ) {
      return write_statement( dynamic_cast<const if_statement&>( s ),         tbl, os);
   } else if( typeid( s ) == typeid( var_statement ) ) {
      return write_statement( dynamic_cast<const var_statement&>( s ),        tbl, os);
   } else if( typeid( s ) == typeid( return_statement ) ) {
      return write_statement( dynamic_cast<const return_statement&>( s ),     tbl, os);
   }
}

void write_function( const function_declaration& f, const resolution_table& tbl, std::ostream& os) {
   os << "double " << f.name->source << "_" << f.name << "(";
   for (int i = 0; i < f.parameters.size( ); ++i) {
      os << "double " << f.parameters[i]->source << "_" << f.parameters[i]->source;
      if (i + 1 < f.parameters.size( )) {
         os << ",";
      }
   }
   os << ") {\n";
   write_statement( *f.body, tbl, os);
   os << "}\n\n";
}

void write_program(const program_resources& pr, const resolution_table& tbl, std::ostream& os) {
   for( const auto& f : pr.tree.functions ) {
      write_function( f, tbl, os );
   }
}

#endif
