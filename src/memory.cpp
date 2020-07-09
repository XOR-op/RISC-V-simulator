//
// Created by Vortox Oganesson on 7/6/20.
//
#include "memory.h"
#include <string.h>
#include <cstdlib>
#include <cassert>
using namespace riscv;
void memory::open(std::istream& is) {
    memset(mem,0, sizeof(mem));
    int cur = 0, tmp;
    std::string buf;
    while (std::getline(is, buf)) {
        if (buf[0] == '@') {
            std::stringstream ss;
            ss << std::hex << buf.substr(1);
            ss >> cur;
        } else {
            // imply little-endian
            std::stringstream ss;
            ss << std::hex << buf;
            while (ss >> tmp) {
                mem[cur] = (byte_t) tmp;
                cur += 1;
            }
        }
    }
}
dword_t memory::read32(dword_t offset) {
    NO_EXCEED
    return *((dword_t*) (mem+offset));
}
sgn_dword_t memory::read32s(dword_t offset) {
    NO_EXCEED
    return *((sgn_dword_t*) (mem+offset));
}
uint16_t memory::read16(dword_t offset) {
    NO_EXCEED
    return *((uint16_t*) (mem+offset));
}
int16_t memory::read16s(dword_t offset) {
    NO_EXCEED
    return *((int16_t*) (mem+offset));
}
uint8_t memory::read8(dword_t offset) {
    NO_EXCEED
    return *((uint8_t*) (mem+offset));
}
int8_t memory::read8s(dword_t offset) {
    NO_EXCEED
    return *((int8_t*) (mem+offset));
}
void memory::write8(dword_t offset, uint8_t val) {
    NO_EXCEED
    *(mem+offset) = val;
}
void memory::write16(dword_t offset, uint16_t val) {
    NO_EXCEED
    *(uint16_t*) (mem+offset) = val;
}
void memory::write32(dword_t offset, dword_t val) {
    NO_EXCEED
    *(dword_t*) (mem+offset) = val;
}
