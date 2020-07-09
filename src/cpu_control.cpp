#include "cpu.h"

using namespace riscv;
void cpu::run() {
    IF_ID_inst = ID_EX_op = EX_MEM_op = MEM_WB_op = inst::NOP;
    for (counter = 0; timeout > 0; ++counter) {
        assert(registers[0] == 0);
        clockIn();
        if (stall < STALL_IF)
            IF_stage();
        // to simulate parallelization, WB must be called before ID
        WB_stage();
        if (stall < STALL_ID)
            ID_stage();
        // ID need to be before EX
        if (stall < STALL_EX)
            EX_stage();
        if (stall < STALL_MEM)
            MEM_stage();
        // subtract stall counter
        if (stall_counter > 0) {
            --stall_counter;
            if (!stall_counter) {
                stall = NO_STALL;
                stall_info = NO;
            }
        }
        stall_request.set(this);
        if (stall == NO_STALL) {
            switch (EX_MEM_op) {
                case inst::LB:
                case inst::LBU:
                case inst::LH:
                case inst::LHU:
                case inst::LW:
                    if ((ID_EX_r1 == EX_MEM_rd || ID_EX_r2 == EX_MEM_rd) && EX_MEM_rd != 0) {
                        // READ after LOAD hazard
                        assert(stall == NO_STALL);
                        stall_request(STALL_EX, RAL_STALL, 1);
                        stall_request.set(this);
                    }
                default:
                    break;
            }
        } else {
            assert(stall == STALL_EX);
        }
        if (clear_stat == CLEAR_IF_ID) {
            IF_ID_inst = inst::NOP;
        } else if(clear_stat == CLEAR_IF_EX){
            IF_ID_inst=inst::NOP;
            ID_EX_op=inst::NOP;
            ID_EX_r1=ID_EX_r2=ID_EX_rd=0;
        }
        clear_stat = NONE;
    }
}

void cpu::clockIn(bool go) {
    if (!go)return;
    if (stall < STALL_ID) {
        ID_inst = IF_ID_inst, ID_pc = IF_ID_pc;
    } else ID_inst = inst::HUG;
    EX_rA = multiplexer(ID_EX_r1, ID_EX_rA, EX_forward, MEM_forward);
    EX_rB = multiplexer(ID_EX_r2, ID_EX_rB, EX_forward, MEM_forward);
    ID_EX_rA = EX_rA, ID_EX_rB = EX_rB; // save forwarding data to avoid loss due to stall
    if (stall < STALL_MEM) {
        MEM_op = EX_MEM_op, MEM_rd = EX_MEM_rd, MEM_value = EX_MEM_ALU_output, MEM_rB = EX_MEM_rB;
    } else MEM_op = inst::HUG;
    if (stall < STALL_EX) {
        EX_is_jmp_bit=ID_EX_is_jmp_bit;
        EX_op = ID_EX_op, EX_rd = ID_EX_rd, EX_imm = ID_EX_imm, EX_pc = ID_EX_pc;
    } else {
        EX_op = inst::HUG;
        if (stall_info == RAL_STALL) {
            // info been fetched
            EX_MEM_rd = 0;
            EX_MEM_op = inst::NOP;
        }
    }
    WB_op = MEM_WB_op, WB_reg = MEM_WB_output, WB_rd = MEM_WB_rd;
    EX_forward.reset();
    MEM_forward.reset();
}

