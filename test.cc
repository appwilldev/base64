#include "base64.hpp"
#include <cstring>
#include <iostream>
#include <string>
int main(int,char * argv[]){
    auto re = base64::encode(argv[1],strlen(argv[1]));
    char * tmp = std::get<0>(re).get();
    std::cout << std::string(tmp,std::get<1>(re)) << "\n";
}