#include "base64.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <stdio.h>
int main(int,char * argv[]){
    std::unique_ptr<char []> foo;
    size_t foo_l;
    base64::encode(argv[1],strlen(argv[1]),foo,foo_l);
    std::cout << std::string(foo.get(),foo_l) << "\n";

    std::unique_ptr<char []> bar;
    size_t bar_l;
    base64::decode(foo.get(),foo_l,bar,bar_l);
    std::cout << std::string(bar.get(),bar_l) << "\n";
}