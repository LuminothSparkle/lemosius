#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include<string>

template<typename InputIterator, typename Transform>
std::string transform_join(InputIterator first, InputIterator last, Transform T, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    std::string res = prefix;
    for(auto it = first; it != last; ++it) {
        res += T(*it) + (std::next(it) != last ? sep : suffix);
    }
    return res;
}

template<typename Container, typename Transform>
std::string transform_join(Container C, Transform T, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    return transform_join(C.begin(),C.end(),T,sep,prefix,suffix);
}

template<typename InputIterator>
std::string join(InputIterator first, InputIterator last, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    return transform_join(first,last,[](const auto& s){
        return s;
    },sep,prefix,suffix);
}

template<typename Container>
std::string join(Container C, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    return join(C.begin(),C.end(),sep,prefix,suffix);
}

/*
std::string join(const std::vector<std::string>& v, const char* sep = "", const char* prefix = "", const char* suffix = "") {
    std::string res = prefix;
    for(auto it = v.begin(); it != v.end(); ++it) {
        res += *it + (std::next(it) != v.end() ? sep : suffix);
    }
    return res;
};
*/


#endif // STRING_UTILITIES_H
