//
// Created by Vortox Oganesson on 7/7/20.
//

#ifndef RISC_V_SIMULATOR_DECODING_H
#define RISC_V_SIMULATOR_DECODING_H

#include "typedef.h"
#include "constant.h"

namespace riscv{
    inst::inst_t select_op(dword_t inst);
    inst::inst_t select_jmp(dword_t inst); // AUIPC & JAL
    dword_t b_decode(sgn_dword_t a);
    dword_t j_decode(sgn_dword_t a);
    dword_t i_decode(sgn_dword_t a);
}
#endif //RISC_V_SIMULATOR_DECODING_H
