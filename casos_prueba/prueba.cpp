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

long double pub3_mult_0x852980(long double a_0x15e1a0,long double b_0x15e5e0);
long double pub3_div_0x8529b0(long double a_0x15e3c0,long double b_0x15e740);
long double pub3_plus_0x8529e0(long double a_0x15e800,long double b_0x15e860);
long double pub3_minus_0x852a10(long double a_0x8524e0,long double b_0x8525a0);
long double pub3_minus_0x852a40(long double a_0x852500);
long double pub3_hashtag_0x852a70(long double a_0x852480);
long double priv3_mult_0x852aa0(long double a_0x8522a0,long double b_0x8523a0);
long double priv3_div_0x852ad0(long double a_0x8523c0,long double b_0x8521c0);
long double priv3_plus_0x852b00(long double a_0x852360,long double b_0x8524a0);
long double priv3_minus_0x852b30(long double a_0x852340,long double b_0x8526e0);
long double priv3_minus_0x852b60(long double a_0x852300);
long double priv3_hashtag_0x852b90(long double a_0x852280);

long double pub3_mult_0x852980(long double a_0x15e1a0,long double b_0x15e5e0){
return priv3_mult_0x852aa0(a_0x15e1a0,b_0x15e5e0);
}

long double pub3_div_0x8529b0(long double a_0x15e3c0,long double b_0x15e740){
return priv3_div_0x852ad0(a_0x15e3c0,b_0x15e740);
}

long double pub3_plus_0x8529e0(long double a_0x15e800,long double b_0x15e860){
return priv3_plus_0x852b00(a_0x15e800,b_0x15e860);
}

long double pub3_minus_0x852a10(long double a_0x8524e0,long double b_0x8525a0){
return priv3_minus_0x852b30(a_0x8524e0,b_0x8525a0);
}

long double pub3_minus_0x852a40(long double a_0x852500){
return priv3_minus_0x852b60(a_0x852500);
}

long double pub3_hashtag_0x852a70(long double a_0x852480){
return priv3_hashtag_0x852b90(a_0x852480);
}

long double priv3_mult_0x852aa0(long double a_0x8522a0,long double b_0x8523a0){
return product<long double>(a_0x8522a0,b_0x8523a0);
}

long double priv3_div_0x852ad0(long double a_0x8523c0,long double b_0x8521c0){
return division<long double>(a_0x8523c0,b_0x8521c0);
}

long double priv3_plus_0x852b00(long double a_0x852360,long double b_0x8524a0){
return addition<long double>(a_0x852360,b_0x8524a0);
}

long double priv3_minus_0x852b30(long double a_0x852340,long double b_0x8526e0){
return subtraction<long double>(a_0x852340,b_0x8526e0);
}

long double priv3_minus_0x852b60(long double a_0x852300){
return minus<long double>(a_0x852300);
}

long double priv3_hashtag_0x852b90(long double a_0x852280){
return print<long double>(a_0x852280);
}

long double pub4_mult_0x856060(long double a_0x15e240,long double b_0x15e360);
long double pub4_div_0x856090(long double a_0x8547f0,long double b_0x8546b0);
long double pub4_plus_0x8560c0(long double a_0x854710,long double b_0x8547b0);
long double pub4_minus_0x8560f0(long double a_0x854290,long double b_0x854090);
long double pub4_minus_0x856120(long double a_0x854210);
long double pub4_hashtag_0x856150(long double a_0x854230);
long double priv4_mult_0x856180(long double a_0x8540b0,long double b_0x854590);
long double priv4_div_0x8561b0(long double a_0x8540f0,long double b_0x8541d0);
long double priv4_plus_0x8561e0(long double a_0x854650,long double b_0x8545b0);
long double priv4_minus_0x856210(long double a_0x8543f0,long double b_0x8542d0);
long double priv4_minus_0x856240(long double a_0x854410);
long double priv4_hashtag_0x856270(long double a_0x854470);

long double pub4_mult_0x856060(long double a_0x15e240,long double b_0x15e360){
return priv4_mult_0x856180(a_0x15e240,b_0x15e360);
}

long double pub4_div_0x856090(long double a_0x8547f0,long double b_0x8546b0){
return priv4_div_0x8561b0(a_0x8547f0,b_0x8546b0);
}

long double pub4_plus_0x8560c0(long double a_0x854710,long double b_0x8547b0){
return priv4_plus_0x8561e0(a_0x854710,b_0x8547b0);
}

long double pub4_minus_0x8560f0(long double a_0x854290,long double b_0x854090){
return priv4_minus_0x856210(a_0x854290,b_0x854090);
}

long double pub4_minus_0x856120(long double a_0x854210){
return priv4_minus_0x856240(a_0x854210);
}

long double pub4_hashtag_0x856150(long double a_0x854230){
return priv4_hashtag_0x856270(a_0x854230);
}

long double priv4_mult_0x856180(long double a_0x8540b0,long double b_0x854590){
return product<long double>(a_0x8540b0,b_0x854590);
}

long double priv4_div_0x8561b0(long double a_0x8540f0,long double b_0x8541d0){
return division<long double>(a_0x8540f0,b_0x8541d0);
}

long double priv4_plus_0x8561e0(long double a_0x854650,long double b_0x8545b0){
return addition<long double>(a_0x854650,b_0x8545b0);
}

long double priv4_minus_0x856210(long double a_0x8543f0,long double b_0x8542d0){
return subtraction<long double>(a_0x8543f0,b_0x8542d0);
}

long double priv4_minus_0x856240(long double a_0x854410){
return minus<long double>(a_0x854410);
}

long double priv4_hashtag_0x856270(long double a_0x854470){
return print<long double>(a_0x854470);
}

long double pub2_mult_0x8605a0(long double a_0x855230,long double b_0x855190);
long double pub2_div_0x8605d0(long double a_0x8554f0,long double b_0x855610);
long double pub2_plus_0x860600(long double a_0x855470,long double b_0x8550b0);
long double pub2_minus_0x860630(long double a_0x8555d0,long double b_0x855430);
long double pub2_minus_0x860660(long double a_0x8552f0);
long double pub2_hashtag_0x860690(long double a_0x855830);
long double priv2_mult_0x8606c0(long double a_0x8554b0,long double b_0x8553b0);
long double priv2_div_0x8606f0(long double a_0x855630,long double b_0x855650);
long double priv2_plus_0x860720(long double a_0x855170,long double b_0x855710);
long double priv2_minus_0x860750(long double a_0x8552b0,long double b_0x8557f0);
long double priv2_minus_0x860780(long double a_0x15e780);
long double priv2_hashtag_0x8607b0(long double a_0x852820);

long double pub2_mult_0x8605a0(long double a_0x855230,long double b_0x855190){
return priv2_mult_0x8606c0(a_0x855230,b_0x855190);
}

long double pub2_div_0x8605d0(long double a_0x8554f0,long double b_0x855610){
return priv2_div_0x8606f0(a_0x8554f0,b_0x855610);
}

long double pub2_plus_0x860600(long double a_0x855470,long double b_0x8550b0){
return priv2_plus_0x860720(a_0x855470,b_0x8550b0);
}

long double pub2_minus_0x860630(long double a_0x8555d0,long double b_0x855430){
return priv2_minus_0x860750(a_0x8555d0,b_0x855430);
}

long double pub2_minus_0x860660(long double a_0x8552f0){
return priv2_minus_0x860780(a_0x8552f0);
}

long double pub2_hashtag_0x860690(long double a_0x855830){
return priv2_hashtag_0x8607b0(a_0x855830);
}

long double priv2_mult_0x8606c0(long double a_0x8554b0,long double b_0x8553b0){
return product<long double>(a_0x8554b0,b_0x8553b0);
}

long double priv2_div_0x8606f0(long double a_0x855630,long double b_0x855650){
return division<long double>(a_0x855630,b_0x855650);
}

long double priv2_plus_0x860720(long double a_0x855170,long double b_0x855710){
return addition<long double>(a_0x855170,b_0x855710);
}

long double priv2_minus_0x860750(long double a_0x8552b0,long double b_0x8557f0){
return subtraction<long double>(a_0x8552b0,b_0x8557f0);
}

long double priv2_minus_0x860780(long double a_0x15e780){
return minus<long double>(a_0x15e780);
}

long double priv2_hashtag_0x8607b0(long double a_0x852820){
return print<long double>(a_0x852820);
}

long double factorial_0x8548a0(long double n_0x835560);
long double main_0x8548d0();

long double factorial_0x8548a0(long double n_0x835560){
if (less_equal<long double>(n_0x835560, static_cast<long double>(0))){
return static_cast<long double>(1);
}
else {
return pub2_mult_0x8605a0(n_0x835560, factorial_0x8548a0(pub2_minus_0x860630(n_0x835560, static_cast<long double>(1))));
}
return static_cast<long double>(0);
}

long double main_0x8548d0(){
long double a_0x8359e0 = static_cast<long double>(0);
long double b_0x835980 = static_cast<long double>(0);
print<long double>(factorial_0x8548a0(static_cast<long double>(5)));
pub2_mult_0x8605a0(a_0x8359e0, b_0x835980);
pub2_div_0x8605d0(a_0x8359e0, b_0x835980);
pub2_minus_0x860630(a_0x8359e0, b_0x835980);
pub2_plus_0x860600(a_0x8359e0,b_0x835980);
pub4_plus_0x8560c0(a_0x8359e0,b_0x835980);
pub2_plus_0x860600(a_0x8359e0, b_0x835980);
pub4_hashtag_0x856150(a_0x8359e0);
long double c_0x8357c0 = read<long double>();
print<long double>(pub2_plus_0x860600(a_0x8359e0, static_cast<long double>(2)));
print<long double>(pub2_plus_0x860600(b_0x835980, static_cast<long double>(1)));
print<long double>(c_0x8357c0);
return static_cast<long double>(0);
}

int main( ) {
main_0x8548d0( );
}
