//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_CPU_H
#define RISC_V_SIMULATOR_CPU_H

#include "typedef.h"
#include "memory.h"
#include "constant.h"
#include <vector>

namespace riscv {
    struct cpu {
//        const static dword_t END_INST=0x0ff00513;
        const static dword_t END_INST=0x00c68223;
        memory* memory;
        // registers *r1=rA, *r2=rB
        dword_t registers[32];
        dword_t pc;
        dword_t IF_ID_inst, IF_ID_pc;
        dword_t  ID_EX_rA, ID_EX_rB, ID_EX_imm, ID_EX_rd, ID_EX_pc,ID_EX_r1,ID_EX_r2;
        inst::inst_t ID_EX_op,EX_MEM_op,MEM_INNER_op,MEM_WB_op,EX_op,MEM_op,WB_op;
        dword_t  EX_MEM_output, EX_MEM_rd, EX_MEM_rB;
        dword_t MEM_INNER_rd,MEM_INNER_rB,MEM_INNER_loc,MEM_ACCESS_counter=0;
        dword_t MEM_WB_output, MEM_WB_rd;
        dword_t  ID_inst,ID_pc;
        dword_t  EX_rA, EX_rB, EX_imm, EX_rd, EX_pc;
        dword_t  MEM_value, MEM_rd, MEM_rB;
        dword_t WB_reg, WB_rd;


        // stall handling
        enum STALL_STAT {
            NO_STALL, STALL_IF, STALL_ID, STALL_EX, STALL_MEM
        };
        STALL_STAT stall = NO_STALL;
        int stall_counter = 0;

        struct stall_request_t {
            STALL_STAT my_stall;
            int my_stall_counter;
            void operator()(STALL_STAT s,int round){my_stall=s;my_stall_counter=round;}
            void set(cpu* parent){
                if(my_stall!=NO_STALL)
                    parent->stall=my_stall,parent->stall_counter=my_stall_counter;
                my_stall=NO_STALL,my_stall_counter=0;
            }
        }stall_request;

        // forwarding
        struct forwarding {
            dword_t reg_name;
            dword_t value;
            void set(dword_t n, dword_t v) { reg_name = n, value = v; }
            void reset() { reg_name = 0;value=0; }
        }EX_EX_forward,MEM_EX_forward;

        // multiplexer
        dword_t multiplexer(dword_t reg_name,dword_t value,forwarding EX_EX,forwarding MEM_EX);

//        const static byte_t STALL_IF=1,STALL_ID=1<<1,STALL_EX=1<<2,STALL_MEM=1<<3,STALL_WB=1<<4;

        void clockIn(bool go = true); // read inter-stage registers into stage registers

        void IF_stage();

        void ID_stage();

        void EX_stage();

        void MEM_stage();

        void WB_stage();

        void run();


        explicit cpu(struct memory* mem_ptr);

        std::vector<std::string> inst_history;
        int counter = 0;
    };
}
#endif //RISC_V_SIMULATOR_CPU_H
