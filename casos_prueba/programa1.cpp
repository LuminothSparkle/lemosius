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

#ifndef MATH_LEM_H
#define MATH_LEM_H

#include <cmath>

//TRIGONOMETRICS

template<typename T>
T cos( const T& a ) {
   return std::cos( a );
}

template<typename T>
T sin( const T& a ) {
   return std::sin( a );
}

template<typename T>
T tan( const T& a ) {
   return std::tan( a );
}

template<typename T>
T acos( const T& a ) {
   return std::acos( a );
}

template<typename T>
T asin( const T& a ) {
   return std::asin( a );
}

template<typename T>
T atan( const T& a ) {
   return std::atan( a );
}

//HYPERBOLICS

template<typename T>
T cosh( const T& a ) {
   return std::cosh( a );
}

template<typename T>
T sinh( const T& a ) {
   return std::sinh( a );
}

template<typename T>
T tanh( const T& a ) {
   return std::tanh( a );
}

template<typename T>
T acosh( const T& a ) {
   return std::acosh( a );
}

template<typename T>
T asinh( const T& a ) {
   return std::asinh( a );
}

template<typename T>
T atanh( const T& a ) {
   return std::atanh( a );
}

template<typename T>
T atan2( const T& a, const T& b ) {
   return std::atan2( a, b );
}

//EXPONENTIALS

template<typename T>
T exp( const T& a ) {
   return std::exp( a );
}

template<typename T>
T exp2( const T& a ) {
   return std::exp2( a );
}

template<typename T>
T expm1( const T& a ) {
   return std::expm1( a );
}

template<typename T>
T log( const T& a ) {
   return std::log( a );
}

template<typename T>
T log10( const T& a ) {
   return std::log10( a );
}

template<typename T>
T log2( const T& a ) {
   return std::log2( a );
}

template<typename T>
T log1p( const T& a ) {
   return std::log1p( a );
}

//POWER

template<typename T>
T pow( const T& a, const T& b ) {
   return std::pow( a, b );
}

template<typename T>
T sqrt( const T& a ) {
   return std::sqrt( a );
}

template<typename T>
T cbrt( const T& a ) {
   return std::cbrt( a );
}

template<typename T>
T hypot( const T& a, const T& b ) {
   return std::hypot( a, b );
}

template<typename T>
T hypot( const T& a, const T& b, const T& c ) {
   return std::hypot( a, b, c );
}

//FLOATATING POINT MANIPULATION

template<typename T>
T ldexp( const T& a, const T& b ) {
   return std::ldexp( a, b );
}

template<typename T>
T frexp( const T& a, T& b ) {
   int temp;
   T res = std::ldexp( a, &temp );
   b = temp;
   return res;
}

template<typename T>
T modf( const T& a, T& b ) {
   return std::modf( a, &b );
}

template<typename T>
T nextafter( const T& a, const T& b ) {
   return std::nextafter( a, b );
}

template<typename T>
T nexttoward( const T& a, const T& b ) {
   return std::nexttoward( a, b );
}

template<typename T>
T copysign( const T& a, const T& b ) {
   return std::copysign( a, b );
}

//ERROR AND GAMMA

template<typename T>
T erf( const T& a ) {
   return std::erf( a );
}

template<typename T>
T erfc( const T& a ) {
   return std::erfc( a );
}

template<typename T>
T tgamma( const T& a ) {
   return std::tgamma( a );
}

template<typename T>
T lgamma( const T& a ) {
   return std::lgamma( a );
}

//NEARES INTEGER FLOATING

template<typename T>
T ceil( const T& a ) {
   return std::ceil( a );
}

template<typename T>
T floor( const T& a ) {
   return std::floor( a );
}

template<typename T>
T trunc( const T& a ) {
   return std::trunc( a );
}

template<typename T>
T round( const T& a ) {
   return std::round( a );
}

//BASIC

template<typename T>
T abs( const T& a ) {
   return std::fabs( a );
}

template<typename T>
T mod( const T& a, const T& b ) {
   return std::fmod( a, b );
}

template<typename T>
T ma( const T& a, const T& b, const T& c ) {
   return std::fma( a, b, c );
}

template<typename T>
T min( const T& a, const T& b ) {
   return std::fmin( a, b );
}

template<typename T>
T max( const T& a, const T& b ) {
   return std::fmax( a, b );
}

template<typename T>
T dim( const T& a, const T& b ) {
   return std::fdim( a, b );
}

template<typename T>
T remainder( const T& a, const T& b ) {
   return std::remainder( a, b );
}

//CLASSIFICATION COMPARATION

template<typename T>
T isfinite( const T& a ) {
   return std::isfinite( a );
}

template<typename T>
T isinf( const T& a ) {
   return std::isinf( a );
}

template<typename T>
T isnan( const T& a ) {
   return std::isnan( a );
}

template<typename T>
T isnorma( const T& a ) {
   return std::isnormal( a );
}

template<typename T>
T signbit( const T& a ) {
   return std::signbit( a );
}

template<typename T>
T isunordered( const T& a ) {
   return std::isunordered( a );
}

#endif //MATH_LEM

long double factorial_0x8456b0(long double a_0x1be320);
long double inv_division_0x8456e0(long double a_0x1bdd60,long double b_0x1be140);
long double inv_int_division_0x845710(long double a_0x1be2c0,long double b_0x1bddc0);
long double integer_division_0x845740(long double a_0x1be040,long double b_0x1bde80);
long double implies_0x845770(long double a_0x1be3c0,long double b_0x1be460);
long double biconditional_0x8457a0(long double a_0x1bdd20,long double b_0x1bdec0);
long double logic_not_0x8457d0(long double a_0x1be340);
long double logic_and_0x845800(long double a_0x1be3e0,long double b_0x1be400);
long double logic_or_0x845830(long double a_0x84c8f0,long double b_0x84c6b0);
long double percentage_0x845860(long double a_0x84cd10);
long double gcd_0x845890(long double a_0x84cad0,long double b_0x84c970);
long double mcd_0x8458c0(long double a_0x84ca10,long double b_0x84c950);
long double main_0x8458f0();

long double factorial_0x8456b0(long double a_0x1be320){
if (less_equal<long double>(a_0x1be320, static_cast<long double>(0))){
return static_cast<long double>(1);
}
return product<long double>(a_0x1be320, factorial_0x8456b0(subtraction<long double>(a_0x1be320, static_cast<long double>(1))));
}

long double inv_division_0x8456e0(long double a_0x1bdd60,long double b_0x1be140){
return division<long double>(b_0x1be140, a_0x1bdd60);
}

long double inv_int_division_0x845710(long double a_0x1be2c0,long double b_0x1bddc0){
return floor<long double>(division<long double>(b_0x1bddc0, a_0x1be2c0));
}

long double integer_division_0x845740(long double a_0x1be040,long double b_0x1bde80){
return floor<long double>(division<long double>(a_0x1be040, b_0x1bde80));
}

long double implies_0x845770(long double a_0x1be3c0,long double b_0x1be460){
return logic_or_0x845830(not_equal<long double>(b_0x1be460, static_cast<long double>(0)), logic_and_0x845800(equal<long double>(a_0x1be3c0, static_cast<long double>(0)), equal<long double>(b_0x1be460, static_cast<long double>(0))));
}

long double biconditional_0x8457a0(long double a_0x1bdd20,long double b_0x1bdec0){
return logic_or_0x845830(logic_and_0x845800(not_equal<long double>(a_0x1bdd20, static_cast<long double>(0)), not_equal<long double>(b_0x1bdec0, static_cast<long double>(0))), logic_and_0x845800(equal<long double>(a_0x1bdd20, static_cast<long double>(0)), equal<long double>(b_0x1bdec0, static_cast<long double>(0))));
}

long double logic_not_0x8457d0(long double a_0x1be340){
return not_equal<long double>(a_0x1be340, static_cast<long double>(0));
}

long double logic_and_0x845800(long double a_0x1be3e0,long double b_0x1be400){
return product<long double>(not_equal<long double>(a_0x1be3e0, static_cast<long double>(0)), not_equal<long double>(b_0x1be400, static_cast<long double>(0)));
}

long double logic_or_0x845830(long double a_0x84c8f0,long double b_0x84c6b0){
return not_equal<long double>(addition<long double>(not_equal<long double>(a_0x84c8f0, static_cast<long double>(0)), not_equal<long double>(b_0x84c6b0, static_cast<long double>(0))), static_cast<long double>(0));
}

long double percentage_0x845860(long double a_0x84cd10){
return division<long double>(a_0x84cd10, static_cast<long double>(100));
}

long double gcd_0x845890(long double a_0x84cad0,long double b_0x84c970){
if (equal<long double>(b_0x84c970, static_cast<long double>(0))){
return a_0x84cad0;
}
assign<long double>(a_0x84cad0, mod<long double>(a_0x84cad0, b_0x84c970));
return gcd_0x845890(b_0x84c970,a_0x84cad0);
}

long double mcd_0x8458c0(long double a_0x84ca10,long double b_0x84c950){
return product<long double>(division<long double>(a_0x84ca10, gcd_0x845890(a_0x84ca10,b_0x84c950)), b_0x84c950);
}

long double main_0x8458f0(){
long double a_0x84cb30;
long double b_0x84cb50;
long double c_0x84cc90;
read<long double>(a_0x84cb30,b_0x84cb50);
read<long double>(c_0x84cc90);
print<long double>(logic_or_0x845830(a_0x84cb30, logic_and_0x845800(b_0x84cb50, c_0x84cc90)));
return static_cast<long double>(0);
}

int main( ) {
main_0x8458f0( );
}
