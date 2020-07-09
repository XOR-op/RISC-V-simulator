//
// Created by Vortox Oganesson on 7/9/20.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H
#include "constant.h"
#include "typedef.h"
namespace riscv{
    struct predictor{
        dword_t predict(dword_t cur_pc){return cur_pc+4;}
    };
}
#endif //RISC_V_SIMULATOR_PREDICTOR_H
