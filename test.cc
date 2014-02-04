#include "base64.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <cstdio>
#include <iterator>
int main(int argc,char *argv[]){
    if(argc == 2){
        std::string tmp{base64::encode(argv[1])};
        std::cout << base64::encode(std::string("aoeu")) << "\n";
        std::cout << base64::decode(tmp) << "\n";
        return 0;
    }
    if(std::string(argv[0]) == "./encode"){
        size_t in_size = 1024*4*3;
        char in_buf[in_size];
        size_t out_size = base64::get_base64_encoded_size(in_size);
        //char out_buf[out_size];

        while(true){
            auto size_readed = std::fread((void *)in_buf,sizeof(in_buf[0]),in_size,stdin);
            base64::up<char []> out{};
            size_t out_size;
            base64::encode(in_buf,size_readed,out,out_size);
            std::fwrite(static_cast<void *>(out.get()),sizeof(*out.get()),out_size,stdout);
            //base64::encode(in_buf,size_readed,out_buf);
            //std::fwrite(static_cast<void*>(out_buf),sizeof(out_buf[0]),base64::get_base64_encoded_size(size_readed),stdout);
            assert(size_readed<=in_size);
            if(size_readed<in_size)
                break;
        }
    }else{
        size_t in_size = 1024*4;
        char in_buf[in_size];
        size_t out_size = in_size / 4 * 3;
        char out_buf[out_size];
        while(true){
            auto size_readed = std::fread((void *)in_buf,sizeof(in_buf[0]),in_size,stdin);
            while(in_buf[size_readed - 1] == '\n')
                --size_readed;
            base64::decode(in_buf,in_buf + size_readed,out_buf);
            std::fwrite((void *)out_buf,sizeof(out_buf[0]),base64::get_base64_decoded_size(in_buf,size_readed),stdout);
            assert(size_readed<=in_size);
            if(size_readed<in_size)
                break;
        }
    }
}
