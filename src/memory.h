//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_MEMORY_H
#define RISC_V_SIMULATOR_MEMORY_H

#include <iostream>
#include <sstream>
#include "typedef.h"

#define NO_EXCEED assert(offset<=0x20000);
namespace riscv {
    struct memory {
        byte_t mem[0x20000+20];

        void open(std::istream& is);

        dword_t read32(dword_t offset);

        sgn_dword_t read32s(dword_t offset);

        uint16_t read16(dword_t offset);

        int16_t read16s(dword_t offset);

        uint8_t read8(dword_t offset);

        int8_t read8s(dword_t offset);

        void write8(dword_t offset, uint8_t val);

        void write16(dword_t offset, uint16_t val);

        void write32(dword_t offset, dword_t val);
    };
}
#endif //RISC_V_SIMULATOR_MEMORY_H
