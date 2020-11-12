#ifndef BUILTINS_LEM_H
#define BUILTINS_LEM_H

#include <cstddef>
#include <iostream>

template<typename T>
T addition( const T& a, const T& b ) {
   return a + b;
}

template<typename T>
T subtraction( const T& a, const T& b ) {
   return a - b;
}

template<typename T>
T product( const T& a, const T& b ) {
   return a * b;
}

template<typename T>
T division( const T& a, const T& b ) {
   return a / b;
}

template<typename T>
T plus( const T& a ) {
   return +a;
}

template<typename T>
T minus( const T& a ) {
   return -a;
}

template<typename T>
T less( const T& a, const T& b ) {
   return a < b;
}

template<typename T>
T less_equal( const T& a, const T& b ) {
   return a <= b;
}

template<typename T>
T greater( const T& a, const T& b ) {
   return a > b;
}

template<typename T>
T greater_equal( const T& a, const T& b ) {
   return a >= b;
}

template<typename T>
T equal( const T& a, const T& b ) {
   return a == b;
}

template<typename T>
T not_equal( const T& a, const T& b ) {
   return a != b;
}

template<typename T>
T& assign( T& a, const T& b ) {
   return a = b;
}

template<typename T>
T print( ) {
   std::cout << "\n";
   return 0;
}

template<typename T>
T print( const T& t ) {
   std::cout << t << "\n";
   return 1;
}

template<typename T, typename... Args>
T print( const T& t, const Args&... args ) {
   return bool( std::cout << t << " " ) + print( args... );
}

template<typename T>
T print_err( ) {
   std::cerr << "\n";
   return 0;
}

template<typename T>
T print_err( const T& t ) {
   std::cerr << t << "\n";
   return 1;
}

template<typename T, typename... Args>
T print_err( const T& t, const Args&... args ) {
   return bool( std::cerr << t << " " ) + print_err<Args...>( args... );
}

template<typename T>
T read( ) {
   T res;
   std::cin >> res;
   return res;
}

template<typename T>
T read( T& t ) {
   std::cin >> t;
   return 1;
}

template<typename T, typename... Args>
T read( T& t, Args&... args ) {
   return bool( std::cin >> t ) + read<Args...>( args... );
}

#endif //BUILTINS_LEM_H
