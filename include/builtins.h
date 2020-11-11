#ifndef BUILTINS_H
#define BUILTINS_H

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
   return a * b;
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
bool less( const T& a, const T& b ) {
   return a < b;
}

template<typename T>
bool less_equal( const T& a, const T& b ) {
   return a <= b;
}

template<typename T>
bool greater( const T& a, const T& b ) {
   return a > b;
}

template<typename T>
bool greater_equal( const T& a, const T& b ) {
   return a >= b;
}

template<typename T>
bool equal( const T& a, const T& b ) {
   return a == b;
}

template<typename T>
bool not_equal( const T& a, const T& b ) {
   return a != b;
}

template<typename T>
T& assign( T& a, const T& b ) {
   return a = b;
}

#include <cstddef>
#include <iostream>
std::size_t print( ) {
   std::cout << "\n";
   return 0;
}

template<typename T, typename... Args>
std::size_t print( const T& t, const Args&... args ) {
   return bool( std::cout << t ) + print( args... );
}

#include <cstddef>
#include <iostream>
std::size_t print_err( ) {
   std::cerr << "\n";
   return 0;
}

template<typename T, typename... Args>
std::size_t print_err( const T& t, const Args&... args ) {
   return bool( std::cerr << t ) + print( args... );
}

#include <cstddef>
#include <iostream>
long double read( ) {
   long double res;
   std::cin >> res;
   return res;
}

//template<typename T, typename... Args>
//std::size_t read( T& t, Args&... args ) {
//   return bool(std::cin >> t) + read(args...);
//}

#endif //BUILTINS_H
