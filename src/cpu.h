//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_CPU_H
#define RISC_V_SIMULATOR_CPU_H

#include "typedef.h"
#include "memory.h"
#include <vector>

namespace riscv {
    struct cpu {
        memory* memory;
        dword_t registers[32];
        dword_t pc;
        dword_t IF_ID_inst, IF_ID_pc;
        dword_t ID_EX_op, ID_EX_r1, ID_EX_r2, ID_EX_imm, ID_EX_rd, ID_EX_pc;
        dword_t EX_MEM_op, EX_MEM_output, EX_MEM_rd, EX_MEM_r2;
        dword_t MEM_WB_output, MEM_WB_rd, MEM_WB_op;
        dword_t ID_inst, ID_pc;
        dword_t EX_op, EX_r1, EX_r2, EX_imm, EX_rd, EX_pc;
        dword_t MEM_op, MEM_value, MEM_rd, MEM_r2;
        dword_t WB_reg, WB_rd, WB_op;
        int counter = 0;
        enum STALL_STAT {
            NO_STALL, STALL_IF, STALL_ID, STALL_EX, STALL_MEM
        };
        STALL_STAT stall = NO_STALL;
        int stall_counter = 0;

//        const static byte_t STALL_IF=1,STALL_ID=1<<1,STALL_EX=1<<2,STALL_MEM=1<<3,STALL_WB=1<<4;
        void clockIn(bool go=true); // read inter-stage registers into stage registers
        void IF_stage();

        void ID_stage();

        void EX_stage();

        void MEM_stage();

        void WB_stage();

        std::vector<std::string> inst_history;
        void run() {
            try {
                for (counter = 0; true; ++counter) {
                    clockIn(stall<STALL_IF);
                    if (stall < STALL_IF)
                        IF_stage();
                    clockIn(stall<STALL_ID);
                    if (stall < STALL_ID)
                        ID_stage();
                    clockIn(stall<STALL_EX);
                    if (stall < STALL_EX)
                        EX_stage();
                    clockIn(stall<STALL_MEM);
                    if(stall<STALL_MEM)
                        MEM_stage();
                    clockIn();
                    WB_stage();
                    // todo set stall
                    if(counter%10==0){
                        stall= static_cast<STALL_STAT>(counter%5);
                        stall_counter=counter%3+1;
                    }
                    // subtract stall counter
                    if (stall_counter > 0) {
                        --stall_counter;
                        if (!stall_counter)stall = NO_STALL;
                    }
                }
            } catch (terminal_exception& e) {
                return;
            }
        }

        explicit cpu(struct memory* mem_ptr);
    };
}
#endif //RISC_V_SIMULATOR_CPU_H
