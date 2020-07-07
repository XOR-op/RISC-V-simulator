//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_TYPEDEF_H
#define RISC_V_SIMULATOR_TYPEDEF_H
#include <iostream>
#include <exception>
namespace riscv{
    typedef uint8_t byte_t;
    typedef uint32_t dword_t;
    typedef int32_t sgn_dword_t;
    struct terminal_exception: public std::exception{};
#define BREAKPOINT(statement) if(statement){int rew=3;--rew;}
}
#endif //RISC_V_SIMULATOR_TYPEDEF_H
