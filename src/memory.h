//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_MEMORY_H
#define RISC_V_SIMULATOR_MEMORY_H
#include <iostream>
#include <sstream>
#include "typedef.h"
#define NO_EXCEED assert(offset<=0x20000);
namespace riscv{
    struct memory{
        byte_t mem[0x20000+20];
        void open(std::istream& is);
        dword_t read32(dword_t offset){
            NO_EXCEED
            return *((dword_t*)(mem+offset));
        }
        sgn_dword_t read32s(dword_t offset){
            NO_EXCEED
            return *((sgn_dword_t*)(mem+offset));
        }
        uint16_t read16(dword_t offset){
            NO_EXCEED
            return *((uint16_t *)(mem+offset));
        }
        int16_t read16s(dword_t offset){
            NO_EXCEED
            return *((int16_t *)(mem+offset));
        }
        uint8_t read8(dword_t offset){
            NO_EXCEED
            return *((uint8_t *)(mem+offset));
        }
        int8_t read8s(dword_t offset){
            NO_EXCEED
            return *((int8_t *)(mem+offset));
        }
        void write8(dword_t offset,uint8_t val){
            NO_EXCEED
            *(mem+offset)=val;
        }
        void write16(dword_t offset,uint16_t val){
            NO_EXCEED
            *(uint16_t*)(mem+offset)=val;
        }
        void write32(dword_t offset,dword_t val){
            NO_EXCEED
            *(dword_t*)(mem+offset)=val;
        }
    };
}
#endif //RISC_V_SIMULATOR_MEMORY_H
