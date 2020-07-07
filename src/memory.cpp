//
// Created by Vortox Oganesson on 7/6/20.
//
#include "memory.h"
using namespace riscv;
void memory::open(std::istream& is) {
    bzero(mem,sizeof(mem));
    int cur=0,tmp;
    std::string buf;
    while (std::getline(is,buf)){

        if(buf[0]=='@'){
            std::stringstream ss;
            ss<<std::hex<<buf.substr(1);
            ss>>cur;
        } else{
            // imply little-endian
            std::stringstream ss;ss<<std::hex<<buf;
            while (ss>>tmp){
                mem[cur]=(byte_t)tmp;
                cur+=1;
            }
        }
    }
}
