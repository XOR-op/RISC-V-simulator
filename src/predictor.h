//
// Created by Vortox Oganesson on 7/9/20.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H
#include "constant.h"
#include "typedef.h"
namespace riscv{
    struct predictor{
        bool predict(dword_t cur_pc){return true;}
    };
}
#endif //RISC_V_SIMULATOR_PREDICTOR_H
