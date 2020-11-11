#ifndef PRINT_ERR_LEM_H
#define PRINT_ERR_LEM_H

#include<iostream>
template<typename T>
T print_err(const T& a) {
   std::cerr << a << "\n";
   return a;
}

#endif //PRINT_ERR_LEM_H