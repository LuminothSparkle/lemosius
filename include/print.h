#ifndef PRINT_LEM_H
#define PRINT_LEM_H

#include<iostream>
template<typename T>
T print(const T& a) {
   std::cout << a << "\n";
   return a;
}

#endif //PRINT_LEM_H