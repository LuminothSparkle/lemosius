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
