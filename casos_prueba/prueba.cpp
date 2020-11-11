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
   return bool(std::cout << t) + print(args...);
}

#include <cstddef>
#include <iostream>
std::size_t print_err( ) {
   std::cerr << "\n";
   return 0;
}

template<typename T, typename... Args>
std::size_t print_err( const T& t, const Args&... args ) {
   return bool(std::cerr << t) + print(args...);
}

#include <cstddef>
#include <iostream>
long double read( ) {
   long double res;
   std::cin >> res;
   return res;
}

/**
template<typename T, typename... Args>
std::size_t read( T& t, Args&... args ) {
   return bool(std::cin >> t) + read(args...);
}
/**/

#endif //BUILTINS_H

long double pub3_mult_0x830ea0(long double a_0xe2aa80,long double b_0xe2aaa0);
long double pub3_div_0x830ed0(long double a_0xe2aca0,long double b_0xe2a720);
long double pub3_plus_0x830f00(long double a_0xe2a980,long double b_0xe2a900);
long double pub3_minus_0x830f30(long double a_0xe2ab80,long double b_0xe27a10);
long double pub3_minus_0x830f60(long double a_0xe2c0b0);
long double pub3_hashtag_0x830f90(long double a_0xe2c490);
long double priv3_mult_0x830fc0(long double a_0xe2c230,long double b_0xe2c4b0);
long double priv3_div_0x830ff0(long double a_0xe2c630,long double b_0xe2c390);
long double priv3_plus_0x831020(long double a_0xe2c5d0,long double b_0xe2c3d0);
long double priv3_minus_0x831050(long double a_0xe2c1f0,long double b_0xe2c5b0);
long double priv3_minus_0x831080(long double a_0xe2c3f0);
long double priv3_hashtag_0x8310b0(long double a_0xe2c250);

long double pub3_mult_0x830ea0(long double a_0xe2aa80,long double b_0xe2aaa0){
return priv3_mult_0x830fc0(a_0xe2aa80,b_0xe2aaa0);
}

long double pub3_div_0x830ed0(long double a_0xe2aca0,long double b_0xe2a720){
return priv3_div_0x830ff0(a_0xe2aca0,b_0xe2a720);
}

long double pub3_plus_0x830f00(long double a_0xe2a980,long double b_0xe2a900){
return priv3_plus_0x831020(a_0xe2a980,b_0xe2a900);
}

long double pub3_minus_0x830f30(long double a_0xe2ab80,long double b_0xe27a10){
return priv3_minus_0x831050(a_0xe2ab80,b_0xe27a10);
}

long double pub3_minus_0x830f60(long double a_0xe2c0b0){
return priv3_minus_0x831080(a_0xe2c0b0);
}

long double pub3_hashtag_0x830f90(long double a_0xe2c490){
return priv3_hashtag_0x8310b0(a_0xe2c490);
}

long double priv3_mult_0x830fc0(long double a_0xe2c230,long double b_0xe2c4b0){
return product(a_0xe2c230,b_0xe2c4b0);
}

long double priv3_div_0x830ff0(long double a_0xe2c630,long double b_0xe2c390){
return division(a_0xe2c630,b_0xe2c390);
}

long double priv3_plus_0x831020(long double a_0xe2c5d0,long double b_0xe2c3d0){
return addition(a_0xe2c5d0,b_0xe2c3d0);
}

long double priv3_minus_0x831050(long double a_0xe2c1f0,long double b_0xe2c5b0){
return subtraction(a_0xe2c1f0,b_0xe2c5b0);
}

long double priv3_minus_0x831080(long double a_0xe2c3f0){
return minus(a_0xe2c3f0);
}

long double priv3_hashtag_0x8310b0(long double a_0xe2c250){
return print(a_0xe2c250);
}

long double pub4_mult_0x842320(long double a_0xe2ac80,long double b_0xe2ace0);
long double pub4_div_0x842350(long double a_0xe2c030,long double b_0xe2c070);
long double pub4_plus_0x842380(long double a_0x83ed80,long double b_0x83ee00);
long double pub4_minus_0x8423b0(long double a_0x83ec80,long double b_0x83eda0);
long double pub4_minus_0x8423e0(long double a_0x83ee80);
long double pub4_hashtag_0x842410(long double a_0x83ec40);
long double priv4_mult_0x842440(long double a_0x83ec60,long double b_0x83eee0);
long double priv4_div_0x842470(long double a_0x83d640,long double b_0x83d7e0);
long double priv4_plus_0x8424a0(long double a_0x83d240,long double b_0x83d440);
long double priv4_minus_0x8424d0(long double a_0x83d4a0,long double b_0x83d6a0);
long double priv4_minus_0x842500(long double a_0x83d700);
long double priv4_hashtag_0x842530(long double a_0x83d5a0);

long double pub4_mult_0x842320(long double a_0xe2ac80,long double b_0xe2ace0){
return priv4_mult_0x842440(a_0xe2ac80,b_0xe2ace0);
}

long double pub4_div_0x842350(long double a_0xe2c030,long double b_0xe2c070){
return priv4_div_0x842470(a_0xe2c030,b_0xe2c070);
}

long double pub4_plus_0x842380(long double a_0x83ed80,long double b_0x83ee00){
return priv4_plus_0x8424a0(a_0x83ed80,b_0x83ee00);
}

long double pub4_minus_0x8423b0(long double a_0x83ec80,long double b_0x83eda0){
return priv4_minus_0x8424d0(a_0x83ec80,b_0x83eda0);
}

long double pub4_minus_0x8423e0(long double a_0x83ee80){
return priv4_minus_0x842500(a_0x83ee80);
}

long double pub4_hashtag_0x842410(long double a_0x83ec40){
return priv4_hashtag_0x842530(a_0x83ec40);
}

long double priv4_mult_0x842440(long double a_0x83ec60,long double b_0x83eee0){
return product(a_0x83ec60,b_0x83eee0);
}

long double priv4_div_0x842470(long double a_0x83d640,long double b_0x83d7e0){
return division(a_0x83d640,b_0x83d7e0);
}

long double priv4_plus_0x8424a0(long double a_0x83d240,long double b_0x83d440){
return addition(a_0x83d240,b_0x83d440);
}

long double priv4_minus_0x8424d0(long double a_0x83d4a0,long double b_0x83d6a0){
return subtraction(a_0x83d4a0,b_0x83d6a0);
}

long double priv4_minus_0x842500(long double a_0x83d700){
return minus(a_0x83d700);
}

long double priv4_hashtag_0x842530(long double a_0x83d5a0){
return print(a_0x83d5a0);
}

long double pub2_mult_0x83ff80(long double a_0x83d560,long double b_0x83d3e0);
long double pub2_div_0x83ffb0(long double a_0x83d280,long double b_0x83d540);
long double pub2_plus_0x83ffe0(long double a_0x83d600,long double b_0x83d620);
long double pub2_minus_0x840010(long double a_0x83d780,long double b_0x83d8c0);
long double pub2_minus_0x840040(long double a_0x83d2c0);
long double pub2_hashtag_0x840070(long double a_0xe2c130);
long double priv2_mult_0x8400a0(long double a_0xe2c410,long double b_0xe2c330);
long double priv2_div_0x8400d0(long double a_0x82aaf0,long double b_0x82ad90);
long double priv2_plus_0x840100(long double a_0x82ad10,long double b_0x82aad0);
long double priv2_minus_0x840130(long double a_0x82ad50,long double b_0x82abb0);
long double priv2_minus_0x840160(long double a_0x82ac90);
long double priv2_hashtag_0x840190(long double a_0x82acd0);

long double pub2_mult_0x83ff80(long double a_0x83d560,long double b_0x83d3e0){
return priv2_mult_0x8400a0(a_0x83d560,b_0x83d3e0);
}

long double pub2_div_0x83ffb0(long double a_0x83d280,long double b_0x83d540){
return priv2_div_0x8400d0(a_0x83d280,b_0x83d540);
}

long double pub2_plus_0x83ffe0(long double a_0x83d600,long double b_0x83d620){
return priv2_plus_0x840100(a_0x83d600,b_0x83d620);
}

long double pub2_minus_0x840010(long double a_0x83d780,long double b_0x83d8c0){
return priv2_minus_0x840130(a_0x83d780,b_0x83d8c0);
}

long double pub2_minus_0x840040(long double a_0x83d2c0){
return priv2_minus_0x840160(a_0x83d2c0);
}

long double pub2_hashtag_0x840070(long double a_0xe2c130){
return priv2_hashtag_0x840190(a_0xe2c130);
}

long double priv2_mult_0x8400a0(long double a_0xe2c410,long double b_0xe2c330){
return product(a_0xe2c410,b_0xe2c330);
}

long double priv2_div_0x8400d0(long double a_0x82aaf0,long double b_0x82ad90){
return division(a_0x82aaf0,b_0x82ad90);
}

long double priv2_plus_0x840100(long double a_0x82ad10,long double b_0x82aad0){
return addition(a_0x82ad10,b_0x82aad0);
}

long double priv2_minus_0x840130(long double a_0x82ad50,long double b_0x82abb0){
return subtraction(a_0x82ad50,b_0x82abb0);
}

long double priv2_minus_0x840160(long double a_0x82ac90){
return minus(a_0x82ac90);
}

long double priv2_hashtag_0x840190(long double a_0x82acd0){
return print(a_0x82acd0);
}

long double main_0x83e750();

long double main_0x83e750(){
long double a_0xe2a9e0 = 0L;
long double b_0xe2ac40 = 0L;
pub2_mult_0x83ff80(a_0xe2a9e0, b_0xe2ac40);
pub2_plus_0x83ffe0(a_0xe2a9e0,b_0xe2ac40);
pub4_plus_0x842380(a_0xe2a9e0,b_0xe2ac40);
pub2_plus_0x83ffe0(a_0xe2a9e0, b_0xe2ac40);
pub4_hashtag_0x842410(a_0xe2a9e0);
print(pub2_plus_0x83ffe0(a_0xe2a9e0, 2L));
print(pub2_plus_0x83ffe0(b_0xe2ac40, 1L));
return 0L;
}

int main( ) {
main_0x83e750( );
}
