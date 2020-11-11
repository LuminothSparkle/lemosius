#ifndef READ_LEM_H
#define READ_LEM_H

#include<iostream>
template<typename T>
T read( ) {
   T res;
   std::cin >> res;
   return res;
}

#endif //READ_LEM_H
#ifndef NOT_EQUAL_LEM_H
#define NOT_EQUAL_LEM_H

template<typename T>
T not_equal(const T& a, const T& b) {
   return a != b;
}

#endif //NOT_EQUAL_LEM_H
#ifndef EQUAL_LEM_H
#define EQUAL_LEM_H

template<typename T>
T equal(const T& a, const T& b) {
   return a == b;
}

#endif //EQUAL_LEM_H
#ifndef PRINT_ERR_LEM_H
#define PRINT_ERR_LEM_H

#include<iostream>
template<typename T>
T print_err(const T& a) {
   std::cerr << a << "\n";
   return a;
}

#endif //PRINT_ERR_LEM_H
#ifndef GREATER_EQUAL_LEM_H
#define GREATER_EQUAL_LEM_H

template<typename T>
T greater_equal(const T& a, const T& b) {
   return a >= b;
}

#endif //GREATER_EQUAL_LEM_H
#ifndef LESS_EQUAL_LEM_H
#define LESS_EQUAL_LEM_H

template<typename T>
T less_equal(const T& a, const T& b) {
   return a <= b;
}

#endif //LESS_EQUAL_LEM_H
#ifndef LESS_LEM_H
#define LESS_LEM_H

template<typename T>
T less(const T& a, const T& b) {
   return a < b;
}

#endif //LESS_LEM_H
#ifndef GREATER_LEM_H
#define GREATER_LEM_H

template<typename T>
T greater(const T& a, const T& b) {
   return a > b;
}

#endif //GREATER_LEM_H
#ifndef MINUS_LEM_H
#define MINUS_LEM_H

template<typename T>
T minus(const T& a) {
   return -a;
}

#endif //MINUS_LEM_H
#ifndef PLUS_LEM_H
#define PLUS_LEM_H

template<typename T>
T plus(const T& a) {
   return +a;
}

#endif //PLUS_LEM_H
#ifndef DIVISION_LEM_H
#define DIVISION_LEM_H

template<typename T>
T division(const T& a, const T& b) {
   return a / b;
}

#endif //DIVISION_LEM_H
#ifndef ASSIGN_LEM_H
#define ASSIGN_LEM_H

template<typename T>
T assign(const T& a, const T& b) {
   return a = b;
}

#endif //ASSIGN_LEM_H
#ifndef PRODUCT_LEM_H
#define PRODUCT_LEM_H

template<typename T>
T product(const T& a, const T& b) {
   return a * b;
}

#endif //PRODUCT_LEM_H
#ifndef PRINT_LEM_H
#define PRINT_LEM_H

#include<iostream>
template<typename T>
T print(const T& a) {
   std::cout << a << "\n";
   return a;
}

#endif //PRINT_LEM_H
#ifndef SUBTRACTION_LEM_H
#define SUBTRACTION_LEM_H

template<typename T>
T subtraction(const T& a, const T& b) {
   return a - b;
}

#endif //SUBTRACTION_LEM_H
#ifndef ADDITION_LEM_H
#define ADDITION_LEM_H

template<typename T>
T addition(const T& a, const T& b) {
   return a + b;
}

#endif //ADDITION_LEM_H
long double pub3_mult_0x825200(long double a_0x91b3c0,long double b_0x91b900);
long double pub3_div_0x825230(long double a_0x91b7a0,long double b_0x91b5a0);
long double pub3_plus_0x825260(long double a_0x91b620,long double b_0x91b2c0);
long double pub3_minus_0x825290(long double a_0x91b880,long double b_0x91b8c0);
long double pub3_minus_0x8252c0(long double a_0x917910);
long double pub3_hashtag_0x8252f0(long double a_0x917930);
long double priv3_mult_0x825320(long double a_0x836d60,long double b_0x836e80);
long double priv3_div_0x825350(long double a_0x837040,long double b_0x836fe0);
long double priv3_plus_0x825380(long double a_0x8372a0,long double b_0x837160);
long double priv3_minus_0x8253b0(long double a_0x836dc0,long double b_0x8370a0);
long double priv3_minus_0x8253e0(long double a_0x836ec0);
long double priv3_hashtag_0x825410(long double a_0x8372c0);

long double pub3_mult_0x825200(long double a_0x91b3c0,long double b_0x91b900){
return priv3_mult_0x825320(a_0x91b3c0,b_0x91b900);
}

long double pub3_div_0x825230(long double a_0x91b7a0,long double b_0x91b5a0){
return priv3_div_0x825350(a_0x91b7a0,b_0x91b5a0);
}

long double pub3_plus_0x825260(long double a_0x91b620,long double b_0x91b2c0){
return priv3_plus_0x825380(a_0x91b620,b_0x91b2c0);
}

long double pub3_minus_0x825290(long double a_0x91b880,long double b_0x91b8c0){
return priv3_minus_0x8253b0(a_0x91b880,b_0x91b8c0);
}

long double pub3_minus_0x8252c0(long double a_0x917910){
return priv3_minus_0x8253e0(a_0x917910);
}

long double pub3_hashtag_0x8252f0(long double a_0x917930){
return priv3_hashtag_0x825410(a_0x917930);
}

long double priv3_mult_0x825320(long double a_0x836d60,long double b_0x836e80){
return product(a_0x836d60,b_0x836e80);
}

long double priv3_div_0x825350(long double a_0x837040,long double b_0x836fe0){
return division(a_0x837040,b_0x836fe0);
}

long double priv3_plus_0x825380(long double a_0x8372a0,long double b_0x837160){
return addition(a_0x8372a0,b_0x837160);
}

long double priv3_minus_0x8253b0(long double a_0x836dc0,long double b_0x8370a0){
return subtraction(a_0x836dc0,b_0x8370a0);
}

long double priv3_minus_0x8253e0(long double a_0x836ec0){
return minus(a_0x836ec0);
}

long double priv3_hashtag_0x825410(long double a_0x8372c0){
return print(a_0x8372c0);
}

#ifndef READ_LEM_H
#define READ_LEM_H

#include<iostream>
template<typename T>
T read( ) {
   T res;
   std::cin >> res;
   return res;
}

#endif //READ_LEM_H
#ifndef NOT_EQUAL_LEM_H
#define NOT_EQUAL_LEM_H

template<typename T>
T not_equal(const T& a, const T& b) {
   return a != b;
}

#endif //NOT_EQUAL_LEM_H
#ifndef EQUAL_LEM_H
#define EQUAL_LEM_H

template<typename T>
T equal(const T& a, const T& b) {
   return a == b;
}

#endif //EQUAL_LEM_H
#ifndef PRINT_ERR_LEM_H
#define PRINT_ERR_LEM_H

#include<iostream>
template<typename T>
T print_err(const T& a) {
   std::cerr << a << "\n";
   return a;
}

#endif //PRINT_ERR_LEM_H
#ifndef GREATER_EQUAL_LEM_H
#define GREATER_EQUAL_LEM_H

template<typename T>
T greater_equal(const T& a, const T& b) {
   return a >= b;
}

#endif //GREATER_EQUAL_LEM_H
#ifndef LESS_EQUAL_LEM_H
#define LESS_EQUAL_LEM_H

template<typename T>
T less_equal(const T& a, const T& b) {
   return a <= b;
}

#endif //LESS_EQUAL_LEM_H
#ifndef LESS_LEM_H
#define LESS_LEM_H

template<typename T>
T less(const T& a, const T& b) {
   return a < b;
}

#endif //LESS_LEM_H
#ifndef GREATER_LEM_H
#define GREATER_LEM_H

template<typename T>
T greater(const T& a, const T& b) {
   return a > b;
}

#endif //GREATER_LEM_H
#ifndef MINUS_LEM_H
#define MINUS_LEM_H

template<typename T>
T minus(const T& a) {
   return -a;
}

#endif //MINUS_LEM_H
#ifndef PLUS_LEM_H
#define PLUS_LEM_H

template<typename T>
T plus(const T& a) {
   return +a;
}

#endif //PLUS_LEM_H
#ifndef DIVISION_LEM_H
#define DIVISION_LEM_H

template<typename T>
T division(const T& a, const T& b) {
   return a / b;
}

#endif //DIVISION_LEM_H
#ifndef ASSIGN_LEM_H
#define ASSIGN_LEM_H

template<typename T>
T assign(const T& a, const T& b) {
   return a = b;
}

#endif //ASSIGN_LEM_H
#ifndef PRODUCT_LEM_H
#define PRODUCT_LEM_H

template<typename T>
T product(const T& a, const T& b) {
   return a * b;
}

#endif //PRODUCT_LEM_H
#ifndef PRINT_LEM_H
#define PRINT_LEM_H

#include<iostream>
template<typename T>
T print(const T& a) {
   std::cout << a << "\n";
   return a;
}

#endif //PRINT_LEM_H
#ifndef SUBTRACTION_LEM_H
#define SUBTRACTION_LEM_H

template<typename T>
T subtraction(const T& a, const T& b) {
   return a - b;
}

#endif //SUBTRACTION_LEM_H
#ifndef ADDITION_LEM_H
#define ADDITION_LEM_H

template<typename T>
T addition(const T& a, const T& b) {
   return a + b;
}

#endif //ADDITION_LEM_H
long double pub4_mult_0x83f300(long double a_0x836f80,long double b_0x8370e0);
long double pub4_div_0x83f330(long double a_0x837320,long double b_0x837120);
long double pub4_plus_0x83f360(long double a_0x91b300,long double b_0x91b340);
long double pub4_minus_0x83f390(long double a_0x83a770,long double b_0x83a2b0);
long double pub4_minus_0x83f3c0(long double a_0x83a4d0);
long double pub4_hashtag_0x83f3f0(long double a_0x83a390);
long double priv4_mult_0x83f420(long double a_0x83a3f0,long double b_0x83a970);
long double priv4_div_0x83f450(long double a_0x83a8f0,long double b_0x83a6f0);
long double priv4_plus_0x83f480(long double a_0x83a5d0,long double b_0x83a690);
long double priv4_minus_0x83f4b0(long double a_0x83a710,long double b_0x83a530);
long double priv4_minus_0x83f4e0(long double a_0x83a9d0);
long double priv4_hashtag_0x83f510(long double a_0x83a310);

long double pub4_mult_0x83f300(long double a_0x836f80,long double b_0x8370e0){
return priv4_mult_0x83f420(a_0x836f80,b_0x8370e0);
}

long double pub4_div_0x83f330(long double a_0x837320,long double b_0x837120){
return priv4_div_0x83f450(a_0x837320,b_0x837120);
}

long double pub4_plus_0x83f360(long double a_0x91b300,long double b_0x91b340){
return priv4_plus_0x83f480(a_0x91b300,b_0x91b340);
}

long double pub4_minus_0x83f390(long double a_0x83a770,long double b_0x83a2b0){
return priv4_minus_0x83f4b0(a_0x83a770,b_0x83a2b0);
}

long double pub4_minus_0x83f3c0(long double a_0x83a4d0){
return priv4_minus_0x83f4e0(a_0x83a4d0);
}

long double pub4_hashtag_0x83f3f0(long double a_0x83a390){
return priv4_hashtag_0x83f510(a_0x83a390);
}

long double priv4_mult_0x83f420(long double a_0x83a3f0,long double b_0x83a970){
return product(a_0x83a3f0,b_0x83a970);
}

long double priv4_div_0x83f450(long double a_0x83a8f0,long double b_0x83a6f0){
return division(a_0x83a8f0,b_0x83a6f0);
}

long double priv4_plus_0x83f480(long double a_0x83a5d0,long double b_0x83a690){
return addition(a_0x83a5d0,b_0x83a690);
}

long double priv4_minus_0x83f4b0(long double a_0x83a710,long double b_0x83a530){
return subtraction(a_0x83a710,b_0x83a530);
}

long double priv4_minus_0x83f4e0(long double a_0x83a9d0){
return minus(a_0x83a9d0);
}

long double priv4_hashtag_0x83f510(long double a_0x83a310){
return print(a_0x83a310);
}

#ifndef READ_LEM_H
#define READ_LEM_H

#include<iostream>
template<typename T>
T read( ) {
   T res;
   std::cin >> res;
   return res;
}

#endif //READ_LEM_H
#ifndef NOT_EQUAL_LEM_H
#define NOT_EQUAL_LEM_H

template<typename T>
T not_equal(const T& a, const T& b) {
   return a != b;
}

#endif //NOT_EQUAL_LEM_H
#ifndef EQUAL_LEM_H
#define EQUAL_LEM_H

template<typename T>
T equal(const T& a, const T& b) {
   return a == b;
}

#endif //EQUAL_LEM_H
#ifndef PRINT_ERR_LEM_H
#define PRINT_ERR_LEM_H

#include<iostream>
template<typename T>
T print_err(const T& a) {
   std::cerr << a << "\n";
   return a;
}

#endif //PRINT_ERR_LEM_H
#ifndef GREATER_EQUAL_LEM_H
#define GREATER_EQUAL_LEM_H

template<typename T>
T greater_equal(const T& a, const T& b) {
   return a >= b;
}

#endif //GREATER_EQUAL_LEM_H
#ifndef LESS_EQUAL_LEM_H
#define LESS_EQUAL_LEM_H

template<typename T>
T less_equal(const T& a, const T& b) {
   return a <= b;
}

#endif //LESS_EQUAL_LEM_H
#ifndef LESS_LEM_H
#define LESS_LEM_H

template<typename T>
T less(const T& a, const T& b) {
   return a < b;
}

#endif //LESS_LEM_H
#ifndef GREATER_LEM_H
#define GREATER_LEM_H

template<typename T>
T greater(const T& a, const T& b) {
   return a > b;
}

#endif //GREATER_LEM_H
#ifndef MINUS_LEM_H
#define MINUS_LEM_H

template<typename T>
T minus(const T& a) {
   return -a;
}

#endif //MINUS_LEM_H
#ifndef PLUS_LEM_H
#define PLUS_LEM_H

template<typename T>
T plus(const T& a) {
   return +a;
}

#endif //PLUS_LEM_H
#ifndef DIVISION_LEM_H
#define DIVISION_LEM_H

template<typename T>
T division(const T& a, const T& b) {
   return a / b;
}

#endif //DIVISION_LEM_H
#ifndef ASSIGN_LEM_H
#define ASSIGN_LEM_H

template<typename T>
T assign(const T& a, const T& b) {
   return a = b;
}

#endif //ASSIGN_LEM_H
#ifndef PRODUCT_LEM_H
#define PRODUCT_LEM_H

template<typename T>
T product(const T& a, const T& b) {
   return a * b;
}

#endif //PRODUCT_LEM_H
#ifndef PRINT_LEM_H
#define PRINT_LEM_H

#include<iostream>
template<typename T>
T print(const T& a) {
   std::cout << a << "\n";
   return a;
}

#endif //PRINT_LEM_H
#ifndef SUBTRACTION_LEM_H
#define SUBTRACTION_LEM_H

template<typename T>
T subtraction(const T& a, const T& b) {
   return a - b;
}

#endif //SUBTRACTION_LEM_H
#ifndef ADDITION_LEM_H
#define ADDITION_LEM_H

template<typename T>
T addition(const T& a, const T& b) {
   return a + b;
}

#endif //ADDITION_LEM_H
long double pub2_mult_0x841e30(long double a_0x836e40,long double b_0x837080);
long double pub2_div_0x841e60(long double a_0x83a8d0,long double b_0x83a290);
long double pub2_plus_0x841e90(long double a_0x83a8b0,long double b_0x83a910);
long double pub2_minus_0x841ec0(long double a_0x8408d0,long double b_0x840a70);
long double pub2_minus_0x841ef0(long double a_0x840990);
long double pub2_hashtag_0x841f20(long double a_0x8409b0);
long double priv2_mult_0x841f50(long double a_0x8407f0,long double b_0x8408f0);
long double priv2_div_0x841f80(long double a_0x840850,long double b_0x840770);
long double priv2_plus_0x841fb0(long double a_0x8436f0,long double b_0x843750);
long double priv2_minus_0x841fe0(long double a_0x843570,long double b_0x843a10);
long double priv2_minus_0x842010(long double a_0x843450);
long double priv2_hashtag_0x842040(long double a_0x8435f0);

long double pub2_mult_0x841e30(long double a_0x836e40,long double b_0x837080){
return priv2_mult_0x841f50(a_0x836e40,b_0x837080);
}

long double pub2_div_0x841e60(long double a_0x83a8d0,long double b_0x83a290){
return priv2_div_0x841f80(a_0x83a8d0,b_0x83a290);
}

long double pub2_plus_0x841e90(long double a_0x83a8b0,long double b_0x83a910){
return priv2_plus_0x841fb0(a_0x83a8b0,b_0x83a910);
}

long double pub2_minus_0x841ec0(long double a_0x8408d0,long double b_0x840a70){
return priv2_minus_0x841fe0(a_0x8408d0,b_0x840a70);
}

long double pub2_minus_0x841ef0(long double a_0x840990){
return priv2_minus_0x842010(a_0x840990);
}

long double pub2_hashtag_0x841f20(long double a_0x8409b0){
return priv2_hashtag_0x842040(a_0x8409b0);
}

long double priv2_mult_0x841f50(long double a_0x8407f0,long double b_0x8408f0){
return product(a_0x8407f0,b_0x8408f0);
}

long double priv2_div_0x841f80(long double a_0x840850,long double b_0x840770){
return division(a_0x840850,b_0x840770);
}

long double priv2_plus_0x841fb0(long double a_0x8436f0,long double b_0x843750){
return addition(a_0x8436f0,b_0x843750);
}

long double priv2_minus_0x841fe0(long double a_0x843570,long double b_0x843a10){
return subtraction(a_0x843570,b_0x843a10);
}

long double priv2_minus_0x842010(long double a_0x843450){
return minus(a_0x843450);
}

long double priv2_hashtag_0x842040(long double a_0x8435f0){
return print(a_0x8435f0);
}

#ifndef READ_LEM_H
#define READ_LEM_H

#include<iostream>
template<typename T>
T read( ) {
   T res;
   std::cin >> res;
   return res;
}

#endif //READ_LEM_H
#ifndef NOT_EQUAL_LEM_H
#define NOT_EQUAL_LEM_H

template<typename T>
T not_equal(const T& a, const T& b) {
   return a != b;
}

#endif //NOT_EQUAL_LEM_H
#ifndef EQUAL_LEM_H
#define EQUAL_LEM_H

template<typename T>
T equal(const T& a, const T& b) {
   return a == b;
}

#endif //EQUAL_LEM_H
#ifndef PRINT_ERR_LEM_H
#define PRINT_ERR_LEM_H

#include<iostream>
template<typename T>
T print_err(const T& a) {
   std::cerr << a << "\n";
   return a;
}

#endif //PRINT_ERR_LEM_H
#ifndef GREATER_EQUAL_LEM_H
#define GREATER_EQUAL_LEM_H

template<typename T>
T greater_equal(const T& a, const T& b) {
   return a >= b;
}

#endif //GREATER_EQUAL_LEM_H
#ifndef LESS_EQUAL_LEM_H
#define LESS_EQUAL_LEM_H

template<typename T>
T less_equal(const T& a, const T& b) {
   return a <= b;
}

#endif //LESS_EQUAL_LEM_H
#ifndef LESS_LEM_H
#define LESS_LEM_H

template<typename T>
T less(const T& a, const T& b) {
   return a < b;
}

#endif //LESS_LEM_H
#ifndef GREATER_LEM_H
#define GREATER_LEM_H

template<typename T>
T greater(const T& a, const T& b) {
   return a > b;
}

#endif //GREATER_LEM_H
#ifndef MINUS_LEM_H
#define MINUS_LEM_H

template<typename T>
T minus(const T& a) {
   return -a;
}

#endif //MINUS_LEM_H
#ifndef PLUS_LEM_H
#define PLUS_LEM_H

template<typename T>
T plus(const T& a) {
   return +a;
}

#endif //PLUS_LEM_H
#ifndef DIVISION_LEM_H
#define DIVISION_LEM_H

template<typename T>
T division(const T& a, const T& b) {
   return a / b;
}

#endif //DIVISION_LEM_H
#ifndef ASSIGN_LEM_H
#define ASSIGN_LEM_H

template<typename T>
T assign(const T& a, const T& b) {
   return a = b;
}

#endif //ASSIGN_LEM_H
#ifndef PRODUCT_LEM_H
#define PRODUCT_LEM_H

template<typename T>
T product(const T& a, const T& b) {
   return a * b;
}

#endif //PRODUCT_LEM_H
#ifndef PRINT_LEM_H
#define PRINT_LEM_H

#include<iostream>
template<typename T>
T print(const T& a) {
   std::cout << a << "\n";
   return a;
}

#endif //PRINT_LEM_H
#ifndef SUBTRACTION_LEM_H
#define SUBTRACTION_LEM_H

template<typename T>
T subtraction(const T& a, const T& b) {
   return a - b;
}

#endif //SUBTRACTION_LEM_H
#ifndef ADDITION_LEM_H
#define ADDITION_LEM_H

template<typename T>
T addition(const T& a, const T& b) {
   return a + b;
}

#endif //ADDITION_LEM_H
int main();

int main(){
long double a_0x843550 = 0L;
long double b_0x843710 = 0L;
pub2_mult_0x841e30(a_0x843550, b_0x843710);
pub2_plus_0x841e90(a_0x843550,b_0x843710);
pub4_plus_0x83f360(a_0x843550,b_0x843710);
pub2_plus_0x841e90(a_0x843550, b_0x843710);
pub4_hashtag_0x83f3f0(a_0x843550);
print(pub2_plus_0x841e90(a_0x843550, 2L));
print(pub2_plus_0x841e90(b_0x843710, 1L));
}

