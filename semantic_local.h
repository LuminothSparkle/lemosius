#ifndef SEMANTIC_LOCAL_H
#define SEMANTIC_LOCAL_H

#include"lexer.h"
#include"parser.h"
#include"semantic_global.h"
#include"semantic_utilities.h"
#include<list>
#include<unordered_set>
#include<typeinfo>

class current_scope {
   current_scope* parent;
   std::unordered_map<std::string_view, token*> variables;
   const std::unordered_map<std::string_view, std::map<std::size_t, program_resources::visible_function>>& function_overloads;

public:
   current_scope(const decltype(program_resources::function_overloads)& f)
   : parent(nullptr), function_overloads(f) {
   }

   current_scope(current_scope* p)     // no puede ser nulo (debería ser referencia, pero no quiero que haya ambigüedad con un constructor por copia)
   : parent(p), function_overloads(p->function_overloads) {
   }

   bool try_declare(std::string_view s) {
      // intentar declarar en .variables
   }

   std::pair<token*, token_type> find_symbol(std::string_view s) {
      // generalmente vamos a usar esta función para buscar una variable siempre, pero hay que identificar tres casos (para el reporte de errores):
      // 1) no existe el símbolo (regresamos nullptr)
      // 2) sí existe el símbolo y sí es variable
      // 3) sí existe el símbolo pero es una función
      // el token apunta al identificador de la variable; el token_type lo construimos nosotros (NUMBER o FUNCTION) dependiendo si es o no variable

      // buscar en las variables del nivel actual
      // si no está entonces hacemos recursión con el padre
      // si no tenemos padre entonces buscamos en las funciones
   }

   // nulo si no la encontramos
   operator_declaration* find_function(std::string_view s, std::size_t arity) {
      // buscar que no esté oculta por una variable
   }
};

void analyze_expression(const expression& e, const program_resources& pr, current_scope& scope);

void analyze_expression(const terminal_expression& e, const program_resources& pr, current_scope& scope) {

}

void analyze_expression(const prefix_expression& e, const program_resources& pr, current_scope& scope) {

}

void analyze_expression(const suffix_expression& e, const program_resources& pr, current_scope& scope) {

}

void analyze_expression(const binary_expression& e, const program_resources& pr, current_scope& scope) {

}

void analyze_expression(const call_expression& e, const program_resources& pr, current_scope& scope) {

}

void analyze_expression(const statement& e, const program_resources& pr, current_scope& scope) {         // sí, if else if !
   if (typeid(e) == typeid(terminal_expression)) {
      return analyze_expression(dynamic_cast<const terminal_expression&>(e), pr, scope);
   } else if (typeid(e) == typeid(prefix_expression)) {
      return analyze_expression(dynamic_cast<const prefix_expression&>(e), pr, scope);
   } else if (typeid(e) == typeid(suffix_expression)) {
      return analyze_expression(dynamic_cast<const suffix_expression&>(e), pr, scope);
   } else if (typeid(e) == typeid(binary_expression)) {
      return analyze_expression(dynamic_cast<const binary_expression&>(e), pr, scope);
   } else if (typeid(e) == typeid(call_expression)) {
      return analyze_expression(dynamic_cast<const call_expression&>(e), pr, scope);
   }
}

void analyze_statement(const statement& s, const program_resources& pr, current_scope& scope);

void analyze_statement(const sequence_statement& s, const program_resources& pr, current_scope& scope) {

}

void analyze_statement(const expression_statement& s, const program_resources& pr, current_scope& scope) {

}

void analyze_statement(const if_statement& s, const program_resources& pr, current_scope& scope) {
   // analizar la condición
   parse_si: {
      current_scope inner(&scope);
      // enviar inner a la recursión
   }

   parte_no: {
      current_scope inner(&scope);
      // enviar inner a la recursión
   }
}

void analyze_statement(const var_statement& s, const program_resources& pr, current_scope& scope) {

}

void analyze_statement(const statement& s, const program_resources& pr, current_scope& scope) {        // sí, if else if !
   if (typeid(s) == typeid(sequence_statement)) {
      return analyze_statement(dynamic_cast<const sequence_statement&>(s), pr, scope);
   } else if (typeid(s) == typeid(expression_statement)) {
      return analyze_statement(dynamic_cast<const expression_statement&>(s), pr, scope);
   } else if (typeid(s) == typeid(if_statement)) {
      return analyze_statement(dynamic_cast<const if_statement&>(s), pr, scope);
   } else if (typeid(s) == typeid(var_statement)) {
      return analyze_statement(dynamic_cast<const var_statement&>(s), pr, scope);
   }
}

void analyze_function(const function_declaration& f, const program_resources& pr, current_scope& scope) {
   //...
   analyze_statement(*f.body, pr, scope);
}

void analyze_program(const syntax_tree& tree, const program_resources& pr) {
   current_scope scope(pr.function_overloads);
   for (const auto& f : tree.functions) {
      analyze_function(f, pr, scope);
   }
}

#endif
