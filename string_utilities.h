#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>
#include <memory>
#include <vector>
#include <sstream>

template<typename CharContainer>
std::vector<std::string> get_words( const CharContainer& s ) {
   std::vector<std::string> res;
   std::istringstream iss( std::string( s.begin( ), s.end( ) ) );
   std::string temp;
   while( iss >> temp ) {
      res.emplace_back( std::move( temp ) );
   }
   return res;
}

template<typename CharContainer>
std::vector<std::string> get_lines( const CharContainer& s ) {
   std::vector<std::string> res;
   std::istringstream iss( std::string( s.begin( ), s.end( ) ) );
   std::string temp;
   while( std::getline( iss, temp ) ) {
      res.emplace_back( std::move( temp ) );
   }
   return res;
}

template<typename InputIterator, typename Transform>
std::string transform_join( InputIterator first, InputIterator last, const Transform& T, const char* sep = "", const char* prefix = "", const char* suffix = "" ) {
   std::string res = prefix;
   for( auto it = first; it != last; std::advance( it, 1 ) ) {
      res += T( *it ) + ( std::next( it ) != last ? sep : "" );
   }
   return res + suffix;
}

template<typename InputContainer, typename Transform>
std::string transform_join( const InputContainer& C, const Transform& T, const char* sep = "", const char* prefix = "", const char* suffix = "" ) {
   return transform_join( C.begin( ), C.end( ), T, sep, prefix, suffix );
}

template<typename InputIterator>
std::string join( InputIterator first, InputIterator last, const char* sep = "", const char* prefix = "", const char* suffix = "" ) {
   return transform_join( first, last, []( const auto & s ) {
      return s;
   }, sep, prefix, suffix );
}

template<typename InputContainer>
std::string join( const InputContainer& C, const char* sep = "", const char* prefix = "", const char* suffix = "" ) {
   return join( C.begin( ), C.end( ), sep, prefix, suffix );
}

template<typename T> requires( !std::is_pointer_v<T> )
std::string to_string( const T& t, const std::string& prefix = "", const std::string& suffix = "" ) {
   return prefix + t.str( ) + suffix;
}

template<typename T>
std::string to_string( const T* t, const std::string& prefix = "", const std::string& suffix = "" ) {
   return ( t != nullptr ? to_string( *t, prefix, suffix ) : "" );
}

template<typename T>
std::string to_string( const std::unique_ptr<T>& p, const std::string& prefix = "", const std::string& suffix = "" ) {
   return to_string( p.get( ), prefix, suffix );
}

#endif // STRING_UTILITIES_H
