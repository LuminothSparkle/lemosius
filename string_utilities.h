#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include<string>

template<typename InputIterator, typename Transform>
std::string transform_join(InputIterator first, InputIterator last, Transform f, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    std::string res = prefix;
    for(auto it = first; it != last; ++it) {
        res += f(*it) + (it + 1 != last ? sep : suffix);
    }
    return res;
}

template<typename Container, typename Transform>
std::string transform_join(Container c, Transform f, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    return transform_join(c.begin(),c.end(),f,sep,prefix,suffix);
}

template<typename InputIterator>
std::string join(InputIterator first, InputIterator last, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    return transform_join(first,last,[](const std::string& s){
        return s;
    },sep,prefix,suffix);
}

template<typename Container>
std::string join(Container c, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    return join(c.begin(),c.end(),sep,prefix,suffix);
}

#endif
