#ifndef BUILTINS_H
#define BUILTINS_H

#include <cmath>

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

long double pub3_mult_0xbfe180(long double a_0xbe5340,long double b_0xbe5180);
long double pub3_div_0xbfe1b0(long double a_0xbe56a0,long double b_0xbe57a0);
long double pub3_plus_0xbfe1e0(long double a_0xbe5100,long double b_0xbe5400);
long double pub3_minus_0xbfe210(long double a_0xbe5580,long double b_0x279d0);
long double pub3_minus_0xbfe240(long double a_0xbfdd40);
long double pub3_hashtag_0xbfe270(long double a_0xbfda60);
long double priv3_mult_0xbfe2a0(long double a_0xbfdf20,long double b_0xbfdc60);
long double priv3_div_0xbfe2d0(long double a_0xbfdd60,long double b_0xbfe100);
long double priv3_plus_0xbfe300(long double a_0xbfdea0,long double b_0xbfda00);
long double priv3_minus_0xbfe330(long double a_0xbfdce0,long double b_0xbfd9e0);
long double priv3_minus_0xbfe360(long double a_0xbfe080);
long double priv3_hashtag_0xbfe390(long double a_0xbfd9c0);

long double pub3_mult_0xbfe180(long double a_0xbe5340,long double b_0xbe5180){
return priv3_mult_0xbfe2a0(a_0xbe5340,b_0xbe5180);
}

long double pub3_div_0xbfe1b0(long double a_0xbe56a0,long double b_0xbe57a0){
return priv3_div_0xbfe2d0(a_0xbe56a0,b_0xbe57a0);
}

long double pub3_plus_0xbfe1e0(long double a_0xbe5100,long double b_0xbe5400){
return priv3_plus_0xbfe300(a_0xbe5100,b_0xbe5400);
}

long double pub3_minus_0xbfe210(long double a_0xbe5580,long double b_0x279d0){
return priv3_minus_0xbfe330(a_0xbe5580,b_0x279d0);
}

long double pub3_minus_0xbfe240(long double a_0xbfdd40){
return priv3_minus_0xbfe360(a_0xbfdd40);
}

long double pub3_hashtag_0xbfe270(long double a_0xbfda60){
return priv3_hashtag_0xbfe390(a_0xbfda60);
}

long double priv3_mult_0xbfe2a0(long double a_0xbfdf20,long double b_0xbfdc60){
return product(a_0xbfdf20,b_0xbfdc60);
}

long double priv3_div_0xbfe2d0(long double a_0xbfdd60,long double b_0xbfe100){
return division(a_0xbfdd60,b_0xbfe100);
}

long double priv3_plus_0xbfe300(long double a_0xbfdea0,long double b_0xbfda00){
return addition(a_0xbfdea0,b_0xbfda00);
}

long double priv3_minus_0xbfe330(long double a_0xbfdce0,long double b_0xbfd9e0){
return subtraction(a_0xbfdce0,b_0xbfd9e0);
}

long double priv3_minus_0xbfe360(long double a_0xbfe080){
return minus(a_0xbfe080);
}

long double priv3_hashtag_0xbfe390(long double a_0xbfd9c0){
return print(a_0xbfd9c0);
}

long double pub4_mult_0xc0dff0(long double a_0xbe5240,long double b_0xbe5280);
long double pub4_div_0xc0e020(long double a_0xbfde80,long double b_0xbfdf00);
long double pub4_plus_0xc0e050(long double a_0xc109d0,long double b_0xc10c70);
long double pub4_minus_0xc0e080(long double a_0xc10bd0,long double b_0xc10c50);
long double pub4_minus_0xc0e0b0(long double a_0xc109f0);
long double pub4_hashtag_0xc0e0e0(long double a_0xc10b70);
long double priv4_mult_0xc0e110(long double a_0xc10a50,long double b_0xc10cf0);
long double priv4_div_0xc0e140(long double a_0xc13f80,long double b_0xc141c0);
long double priv4_plus_0xc0e170(long double a_0xc13ec0,long double b_0xc144e0);
long double priv4_minus_0xc0e1a0(long double a_0xc13f60,long double b_0xc145a0);
long double priv4_minus_0xc0e1d0(long double a_0xc14220);
long double priv4_hashtag_0xc0e200(long double a_0xc145e0);

long double pub4_mult_0xc0dff0(long double a_0xbe5240,long double b_0xbe5280){
return priv4_mult_0xc0e110(a_0xbe5240,b_0xbe5280);
}

long double pub4_div_0xc0e020(long double a_0xbfde80,long double b_0xbfdf00){
return priv4_div_0xc0e140(a_0xbfde80,b_0xbfdf00);
}

long double pub4_plus_0xc0e050(long double a_0xc109d0,long double b_0xc10c70){
return priv4_plus_0xc0e170(a_0xc109d0,b_0xc10c70);
}

long double pub4_minus_0xc0e080(long double a_0xc10bd0,long double b_0xc10c50){
return priv4_minus_0xc0e1a0(a_0xc10bd0,b_0xc10c50);
}

long double pub4_minus_0xc0e0b0(long double a_0xc109f0){
return priv4_minus_0xc0e1d0(a_0xc109f0);
}

long double pub4_hashtag_0xc0e0e0(long double a_0xc10b70){
return priv4_hashtag_0xc0e200(a_0xc10b70);
}

long double priv4_mult_0xc0e110(long double a_0xc10a50,long double b_0xc10cf0){
return product(a_0xc10a50,b_0xc10cf0);
}

long double priv4_div_0xc0e140(long double a_0xc13f80,long double b_0xc141c0){
return division(a_0xc13f80,b_0xc141c0);
}

long double priv4_plus_0xc0e170(long double a_0xc13ec0,long double b_0xc144e0){
return addition(a_0xc13ec0,b_0xc144e0);
}

long double priv4_minus_0xc0e1a0(long double a_0xc13f60,long double b_0xc145a0){
return subtraction(a_0xc13f60,b_0xc145a0);
}

long double priv4_minus_0xc0e1d0(long double a_0xc14220){
return minus(a_0xc14220);
}

long double priv4_hashtag_0xc0e200(long double a_0xc145e0){
return print(a_0xc145e0);
}

long double pub2_mult_0x2bc60(long double a_0xc13fc0,long double b_0xc140e0);
long double pub2_div_0x2bc90(long double a_0xc140a0,long double b_0xc14640);
long double pub2_plus_0x2bcc0(long double a_0xc14020,long double b_0xc14080);
long double pub2_minus_0x2bcf0(long double a_0xc143e0,long double b_0xc14120);
long double pub2_minus_0x2bd20(long double a_0xc14400);
long double pub2_hashtag_0x2bd50(long double a_0xbfdb60);
long double priv2_mult_0x2bd80(long double a_0xbfdc80,long double b_0xbfdd00);
long double priv2_div_0x2bdb0(long double a_0xc0e3b0,long double b_0xc0e470);
long double priv2_plus_0x2bde0(long double a_0xc0e930,long double b_0xc0e830);
long double priv2_minus_0x2be10(long double a_0xc0e8f0,long double b_0xc0e950);
long double priv2_minus_0x2be40(long double a_0xc0e970);
long double priv2_hashtag_0x2be70(long double a_0xc0eab0);

long double pub2_mult_0x2bc60(long double a_0xc13fc0,long double b_0xc140e0){
return priv2_mult_0x2bd80(a_0xc13fc0,b_0xc140e0);
}

long double pub2_div_0x2bc90(long double a_0xc140a0,long double b_0xc14640){
return priv2_div_0x2bdb0(a_0xc140a0,b_0xc14640);
}

long double pub2_plus_0x2bcc0(long double a_0xc14020,long double b_0xc14080){
return priv2_plus_0x2bde0(a_0xc14020,b_0xc14080);
}

long double pub2_minus_0x2bcf0(long double a_0xc143e0,long double b_0xc14120){
return priv2_minus_0x2be10(a_0xc143e0,b_0xc14120);
}

long double pub2_minus_0x2bd20(long double a_0xc14400){
return priv2_minus_0x2be40(a_0xc14400);
}

long double pub2_hashtag_0x2bd50(long double a_0xbfdb60){
return priv2_hashtag_0x2be70(a_0xbfdb60);
}

long double priv2_mult_0x2bd80(long double a_0xbfdc80,long double b_0xbfdd00){
return product(a_0xbfdc80,b_0xbfdd00);
}

long double priv2_div_0x2bdb0(long double a_0xc0e3b0,long double b_0xc0e470){
return division(a_0xc0e3b0,b_0xc0e470);
}

long double priv2_plus_0x2bde0(long double a_0xc0e930,long double b_0xc0e830){
return addition(a_0xc0e930,b_0xc0e830);
}

long double priv2_minus_0x2be10(long double a_0xc0e8f0,long double b_0xc0e950){
return subtraction(a_0xc0e8f0,b_0xc0e950);
}

long double priv2_minus_0x2be40(long double a_0xc0e970){
return minus(a_0xc0e970);
}

long double priv2_hashtag_0x2be70(long double a_0xc0eab0){
return print(a_0xc0eab0);
}

long double factorial_0xbfceb0(long double n_0xc0e5b0);
long double main_0xbfcee0();

long double factorial_0xbfceb0(long double n_0xc0e5b0){
if (less_equal(n_0xc0e5b0, static_cast<long double>(0L))){
return static_cast<long double>(1L);
}
else {
return pub2_mult_0x2bc60(n_0xc0e5b0, factorial_0xbfceb0(pub2_minus_0x2bcf0(n_0xc0e5b0, static_cast<long double>(1L))));
}
return static_cast<long double>(0L);
}

long double main_0xbfcee0(){
long double a_0xc0e870 = static_cast<long double>(0L);
long double b_0xc0e8d0 = static_cast<long double>(0L);
print(factorial_0xbfceb0(static_cast<long double>(5L)));
pub2_mult_0x2bc60(a_0xc0e870, b_0xc0e8d0);
pub2_div_0x2bc90(a_0xc0e870, b_0xc0e8d0);
pub2_minus_0x2bcf0(a_0xc0e870, b_0xc0e8d0);
pub2_plus_0x2bcc0(a_0xc0e870,b_0xc0e8d0);
pub4_plus_0xc0e050(a_0xc0e870,b_0xc0e8d0);
pub2_plus_0x2bcc0(a_0xc0e870, b_0xc0e8d0);
pub4_hashtag_0xc0e0e0(a_0xc0e870);
long double c_0xbe5720 = read();
print(pub2_plus_0x2bcc0(a_0xc0e870, static_cast<long double>(2L)));
print(pub2_plus_0x2bcc0(b_0xc0e8d0, static_cast<long double>(1L)));
print(c_0xbe5720);
return static_cast<long double>(0L);
}

int main( ) {
main_0xbfcee0( );
}
