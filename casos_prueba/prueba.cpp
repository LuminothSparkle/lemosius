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

long double pub3_mult_0xc568c0(long double a_0x8eee80,long double b_0x8ee9c0);
long double pub3_div_0xc568f0(long double a_0x8eee20,long double b_0x8eeea0);
long double pub3_plus_0xc56920(long double a_0x8ee800,long double b_0x8ee820);
long double pub3_minus_0xc56950(long double a_0x8ee940,long double b_0x8ee960);
long double pub3_minus_0xc56980(long double a_0xc404c0);
long double pub3_hashtag_0xc569b0(long double a_0xc40700);
long double priv3_mult_0xc569e0(long double a_0xc40560,long double b_0xc404e0);
long double priv3_div_0xc56a10(long double a_0xc40460,long double b_0xc405a0);
long double priv3_plus_0xc56a40(long double a_0xc40780,long double b_0xc401a0);
long double priv3_minus_0xc56a70(long double a_0xc40260,long double b_0xc407e0);
long double priv3_minus_0xc56aa0(long double a_0xc40840);
long double priv3_hashtag_0xc56ad0(long double a_0xc40880);

long double pub3_mult_0xc568c0(long double a_0x8eee80,long double b_0x8ee9c0){
return priv3_mult_0xc569e0(a_0x8eee80,b_0x8ee9c0);
}

long double pub3_div_0xc568f0(long double a_0x8eee20,long double b_0x8eeea0){
return priv3_div_0xc56a10(a_0x8eee20,b_0x8eeea0);
}

long double pub3_plus_0xc56920(long double a_0x8ee800,long double b_0x8ee820){
return priv3_plus_0xc56a40(a_0x8ee800,b_0x8ee820);
}

long double pub3_minus_0xc56950(long double a_0x8ee940,long double b_0x8ee960){
return priv3_minus_0xc56a70(a_0x8ee940,b_0x8ee960);
}

long double pub3_minus_0xc56980(long double a_0xc404c0){
return priv3_minus_0xc56aa0(a_0xc404c0);
}

long double pub3_hashtag_0xc569b0(long double a_0xc40700){
return priv3_hashtag_0xc56ad0(a_0xc40700);
}

long double priv3_mult_0xc569e0(long double a_0xc40560,long double b_0xc404e0){
return product<long double>(a_0xc40560,b_0xc404e0);
}

long double priv3_div_0xc56a10(long double a_0xc40460,long double b_0xc405a0){
return division<long double>(a_0xc40460,b_0xc405a0);
}

long double priv3_plus_0xc56a40(long double a_0xc40780,long double b_0xc401a0){
return addition<long double>(a_0xc40780,b_0xc401a0);
}

long double priv3_minus_0xc56a70(long double a_0xc40260,long double b_0xc407e0){
return subtraction<long double>(a_0xc40260,b_0xc407e0);
}

long double priv3_minus_0xc56aa0(long double a_0xc40840){
return minus<long double>(a_0xc40840);
}

long double priv3_hashtag_0xc56ad0(long double a_0xc40880){
return print<long double>(a_0xc40880);
}

long double pub4_mult_0xc5f1a0(long double a_0x8eed60,long double b_0x8eea00);
long double pub4_div_0xc5f1d0(long double a_0x8eeee0,long double b_0xc40420);
long double pub4_plus_0xc5f200(long double a_0xc62600,long double b_0xc624e0);
long double pub4_minus_0xc5f230(long double a_0xc62580,long double b_0xc625a0);
long double pub4_minus_0xc5f260(long double a_0xc626a0);
long double pub4_hashtag_0xc5f290(long double a_0xc62380);
long double priv4_mult_0xc5f2c0(long double a_0xc62400,long double b_0xc62420);
long double priv4_div_0xc5f2f0(long double a_0xc5ee00,long double b_0xc5ea80);
long double priv4_plus_0xc5f320(long double a_0xc5ef00,long double b_0xc5eb20);
long double priv4_minus_0xc5f350(long double a_0xc5eea0,long double b_0xc5eb00);
long double priv4_minus_0xc5f380(long double a_0xc5eca0);
long double priv4_hashtag_0xc5f3b0(long double a_0xc5ec00);

long double pub4_mult_0xc5f1a0(long double a_0x8eed60,long double b_0x8eea00){
return priv4_mult_0xc5f2c0(a_0x8eed60,b_0x8eea00);
}

long double pub4_div_0xc5f1d0(long double a_0x8eeee0,long double b_0xc40420){
return priv4_div_0xc5f2f0(a_0x8eeee0,b_0xc40420);
}

long double pub4_plus_0xc5f200(long double a_0xc62600,long double b_0xc624e0){
return priv4_plus_0xc5f320(a_0xc62600,b_0xc624e0);
}

long double pub4_minus_0xc5f230(long double a_0xc62580,long double b_0xc625a0){
return priv4_minus_0xc5f350(a_0xc62580,b_0xc625a0);
}

long double pub4_minus_0xc5f260(long double a_0xc626a0){
return priv4_minus_0xc5f380(a_0xc626a0);
}

long double pub4_hashtag_0xc5f290(long double a_0xc62380){
return priv4_hashtag_0xc5f3b0(a_0xc62380);
}

long double priv4_mult_0xc5f2c0(long double a_0xc62400,long double b_0xc62420){
return product<long double>(a_0xc62400,b_0xc62420);
}

long double priv4_div_0xc5f2f0(long double a_0xc5ee00,long double b_0xc5ea80){
return division<long double>(a_0xc5ee00,b_0xc5ea80);
}

long double priv4_plus_0xc5f320(long double a_0xc5ef00,long double b_0xc5eb20){
return addition<long double>(a_0xc5ef00,b_0xc5eb20);
}

long double priv4_minus_0xc5f350(long double a_0xc5eea0,long double b_0xc5eb00){
return subtraction<long double>(a_0xc5eea0,b_0xc5eb00);
}

long double priv4_minus_0xc5f380(long double a_0xc5eca0){
return minus<long double>(a_0xc5eca0);
}

long double priv4_hashtag_0xc5f3b0(long double a_0xc5ec00){
return print<long double>(a_0xc5ec00);
}

long double pub2_mult_0xc61ef0(long double a_0xc5ec80,long double b_0xc5eee0);
long double pub2_div_0xc61f20(long double a_0xc5ed60,long double b_0xc5efa0);
long double pub2_plus_0xc61f50(long double a_0xc5f120,long double b_0xc5eb40);
long double pub2_minus_0xc61f80(long double a_0xc5f0a0,long double b_0xc5ecc0);
long double pub2_minus_0xc61fb0(long double a_0xc5ece0);
long double pub2_hashtag_0xc61fe0(long double a_0xc5ede0);
long double priv2_mult_0xc62010(long double a_0xc401e0,long double b_0xc40200);
long double priv2_div_0xc62040(long double a_0xc61830,long double b_0xc61cb0);
long double priv2_plus_0xc62070(long double a_0xc61e30,long double b_0xc61a50);
long double priv2_minus_0xc620a0(long double a_0xc61a90,long double b_0xc61c30);
long double priv2_minus_0xc620d0(long double a_0xc61870);
long double priv2_hashtag_0xc62100(long double a_0xc61970);

long double pub2_mult_0xc61ef0(long double a_0xc5ec80,long double b_0xc5eee0){
return priv2_mult_0xc62010(a_0xc5ec80,b_0xc5eee0);
}

long double pub2_div_0xc61f20(long double a_0xc5ed60,long double b_0xc5efa0){
return priv2_div_0xc62040(a_0xc5ed60,b_0xc5efa0);
}

long double pub2_plus_0xc61f50(long double a_0xc5f120,long double b_0xc5eb40){
return priv2_plus_0xc62070(a_0xc5f120,b_0xc5eb40);
}

long double pub2_minus_0xc61f80(long double a_0xc5f0a0,long double b_0xc5ecc0){
return priv2_minus_0xc620a0(a_0xc5f0a0,b_0xc5ecc0);
}

long double pub2_minus_0xc61fb0(long double a_0xc5ece0){
return priv2_minus_0xc620d0(a_0xc5ece0);
}

long double pub2_hashtag_0xc61fe0(long double a_0xc5ede0){
return priv2_hashtag_0xc62100(a_0xc5ede0);
}

long double priv2_mult_0xc62010(long double a_0xc401e0,long double b_0xc40200){
return product<long double>(a_0xc401e0,b_0xc40200);
}

long double priv2_div_0xc62040(long double a_0xc61830,long double b_0xc61cb0){
return division<long double>(a_0xc61830,b_0xc61cb0);
}

long double priv2_plus_0xc62070(long double a_0xc61e30,long double b_0xc61a50){
return addition<long double>(a_0xc61e30,b_0xc61a50);
}

long double priv2_minus_0xc620a0(long double a_0xc61a90,long double b_0xc61c30){
return subtraction<long double>(a_0xc61a90,b_0xc61c30);
}

long double priv2_minus_0xc620d0(long double a_0xc61870){
return minus<long double>(a_0xc61870);
}

long double priv2_hashtag_0xc62100(long double a_0xc61970){
return print<long double>(a_0xc61970);
}

long double factorial_0xc53dd0(long double n_0xc618b0);
long double main_0xc53e00();

long double factorial_0xc53dd0(long double n_0xc618b0){
if (less_equal<long double>(n_0xc618b0, static_cast<long double>(0))){
return static_cast<long double>(1);
}
else {
return pub2_mult_0xc61ef0(n_0xc618b0, factorial_0xc53dd0(pub2_minus_0xc61f80(n_0xc618b0, static_cast<long double>(1))));
}
return static_cast<long double>(0);
}

long double main_0xc53e00(){
long double a_0xc61d90 = static_cast<long double>(0);
long double b_0xc61e70 = static_cast<long double>(0);
print<long double>(factorial_0xc53dd0(static_cast<long double>(5)));
pub2_mult_0xc61ef0(a_0xc61d90, b_0xc61e70);
pub2_div_0xc61f20(a_0xc61d90, b_0xc61e70);
pub2_minus_0xc61f80(a_0xc61d90, b_0xc61e70);
pub2_plus_0xc61f50(a_0xc61d90,b_0xc61e70);
pub4_plus_0xc5f200(a_0xc61d90,b_0xc61e70);
pub2_plus_0xc61f50(a_0xc61d90, b_0xc61e70);
pub4_hashtag_0xc5f290(a_0xc61d90);
long double c_0x8eea20 = read<long double>();
print<long double>(pub2_plus_0xc61f50(a_0xc61d90, static_cast<long double>(2)));
print<long double>(pub2_plus_0xc61f50(b_0xc61e70, static_cast<long double>(1)));
print<long double>(c_0x8eea20);
return static_cast<long double>(0);
}

int main( ) {
main_0xc53e00( );
}
