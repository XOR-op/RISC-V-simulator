//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_CPU_H
#define RISC_V_SIMULATOR_CPU_H

#include "typedef.h"
#include "memory.h"
#include "constant.h"
#include "predictor.h"
#include <vector>

namespace riscv {
    struct cpu {
        std::vector<std::string> inst_history;
        int counter = 0;
        int timeout=1; // finish timeout
        memory* memory;
        // registers *r1=rA, *r2=rB
        dword_t registers[32];
        dword_t pc;
        inst::inst_t ID_EX_op,EX_MEM_op,MEM_WB_op,EX_op,MEM_op,WB_op,MEM_INNER_op;
        dword_t IF_ID_inst, IF_ID_pc;
        dword_t  ID_inst,ID_pc;
        dword_t  ID_EX_rA, ID_EX_rB, ID_EX_imm, ID_EX_rd, ID_EX_pc,ID_EX_r1,ID_EX_r2,ID_EX_is_jmp;
        dword_t  EX_rA, EX_rB, EX_imm, EX_rd, EX_pc,EX_is_jmp;
        dword_t  EX_MEM_ALU_output, EX_MEM_rd, EX_MEM_rB;
        dword_t  MEM_value, MEM_rd, MEM_rB;
        dword_t MEM_INNER_rd,MEM_INNER_rB,MEM_INNER_loc,MEM_ACCESS_counter=0;
        dword_t MEM_WB_output, MEM_WB_rd;
        dword_t WB_reg, WB_rd;

        // branch clear pipeline
        enum {NONE,CLEAR_IF_ID,CLEAR_IF_EX}clear_stat;
        bool ID_EX_is_jmp_bit;
        predictor pre;

        // stall handling
        enum STALL_STAT {
            NO_STALL, STALL_IF, STALL_ID, STALL_EX, STALL_MEM
        };
        enum STALL_INFO{
            NO,MEM_WAIT,RAL_STALL
        };
        STALL_STAT stall = NO_STALL;
        STALL_INFO stall_info=NO;
        int stall_counter = 0;

        struct stall_request_t {
            STALL_STAT my_stall;
            STALL_INFO info;
            int my_stall_counter;
            void operator()(STALL_STAT s,STALL_INFO i,int round){
                my_stall=s;my_stall_counter=round;info=i;}
            void set(cpu* parent){
                if(my_stall!=NO_STALL)
                    parent->stall=my_stall,parent->stall_counter=my_stall_counter,parent->stall_info=info;
                my_stall=NO_STALL,my_stall_counter=0,info=NO;
            }
        }stall_request;

        // forwarding
        struct forwarding {
            dword_t reg_name;
            dword_t value;
            void set(dword_t n, dword_t v) { reg_name = n, value = v; }
            void reset() { reg_name = 0;value=0; }
        }EX_forward,MEM_forward;

        // multiplexer
        dword_t multiplexer(dword_t reg_name,dword_t value,forwarding EX_EX,forwarding MEM_EX);


        void clockIn(bool go = true); // read inter-stage registers into stage registers

        void IF_stage();

        void ID_stage();

        void EX_stage();

        void MEM_stage();

        void WB_stage();

        void run();


        explicit cpu(struct memory* mem_ptr);

    };
}
#endif //RISC_V_SIMULATOR_CPU_H
