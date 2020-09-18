#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H
#include<algorithm>
#include<iterator>
#include<string_view>
#include<utility>
#include<ostream>
#include<cstddef>
#include"token.h"

unsigned line_off(const char* ini, const token& t) {
    return 1 + std::count(ini, t.original.data(), '\n');
}

unsigned col_off(const char* ini, const token& t) {
    return 1 + (t.original.data() - std::find(std::reverse_iterator(t.original.data( )), std::reverse_iterator(ini), '\n').base( ));
}

std::string_view view_off(const token& t, int len, const char* fin) {
    return { t.original.data( ), std::size_t(std::find(t.original.data( ), (fin - t.original.data( ) <= len ? fin : t.original.data( ) + len), '\n') - t.original.data( )) };
}

void error_report(std::ostream& os, const char* ini, const char* fin, const std::pair<token, const char*>& e) {
    os << "Error en linea " << line_off(ini, e.first) << ", columna " << col_off(ini, e.first) << ":\n"
       << "\t" << view_off(e.first, 10, fin) << "\n"
       << "\t^\n"
       << e.second << "\n";
}

#endif
