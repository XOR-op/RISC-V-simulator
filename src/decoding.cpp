//
// Created by Vortox Oganesson on 7/7/20.
//
#include "decoding.h"
#include "constant.h"
#include <unordered_map>
using namespace riscv;
const static dword_t OPCODE_MASK = 0b1111111;
const static dword_t FUNCT3_MASK = 0b111u << 12u;
const static dword_t FUNCT7_MASK = 0b1111111u << 25u;
const static dword_t IMM_I_MASK = 0xfffu << 20u;
typedef std::unordered_map<dword_t ,inst::inst_t> transformer_t;
inst::inst_t riscv::select_op(dword_t inst) {
    inst::inst_t rt=inst::NOP;
    if(inst==END_INST)return inst::END_I;
    dword_t ID_opcode = inst & OPCODE_MASK;
    dword_t ID_funct3 = (inst & FUNCT3_MASK) >> 12;
    dword_t ID_funct7 = (inst & FUNCT7_MASK) >> 25;
    switch (ID_opcode) {
        case op::LUI:
            rt = inst::LUI;
            break;
        case op::AUIPC:
            rt = inst::AUIPC;
            break;
        case op::JAL:
            rt = inst::JAL;
            break;
        case op::JALR:
            rt = inst::JALR;
            break;
        case op::BRANCH: {
//            dword_t arr[]{[BRANCH::BEQ]=inst::BEQ, [BRANCH::BNE]=inst::BNE, [BRANCH::BLT]=inst::BLT, [BRANCH::BGE]=inst::BGE, [BRANCH::BLTU]=inst::BLTU, [BRANCH::BGEU]=inst::BGEU,};
            static transformer_t arr={{BRANCH::BEQ,inst::BEQ}, {BRANCH::BNE,inst::BNE}, {BRANCH::BLT,inst::BLT}, {BRANCH::BGE,inst::BGE}, {BRANCH::BLTU,inst::BLTU}, {BRANCH::BGEU,inst::BGEU}};
            rt = static_cast<inst::inst_t>(arr[ID_funct3]);
        }
            break;
        case op::LOAD: {
//            dword_t arr[]{[LOAD::LB]=inst::LB, [LOAD::LH]=inst::LH, [LOAD::LW]=inst::LW,[LOAD::LBU]=inst::LBU, [LOAD::LHU]=inst::LHU,};
            static transformer_t arr{{LOAD::LB,inst::LB}, {LOAD::LH,inst::LH}, {LOAD::LW,inst::LW},{LOAD::LBU,inst::LBU}, {LOAD::LHU,inst::LHU}};
            rt = static_cast<inst::inst_t>(arr[ID_funct3]);
        }
            break;
        case op::STORE: {
//            dword_t arr[]{[STORE::SB]=inst::SB, [STORE::SH]=inst::SH, [STORE::SW]=inst::SW,};
            static transformer_t arr{{STORE::SB,inst::SB}, {STORE::SH,inst::SH}, {STORE::SW,inst::SW},};
            rt = static_cast<inst::inst_t>(arr[ID_funct3]);
        }
            break;
        case op::ARITHMETIC_IMM: {
//            dword_t arr[]{[ARITHMETIC_IMM::ADDI]=inst::ADDI, [ARITHMETIC_IMM::SLTI]=inst::SLTI, [ARITHMETIC_IMM::SLTIU]=inst::SLTIU, [ARITHMETIC_IMM::XORI]=inst::XORI, [ARITHMETIC_IMM::ORI]=inst::ORI, [ARITHMETIC_IMM::ANDI]=inst::ANDI, [ARITHMETIC_IMM::SLLI]=inst::SLLI, [ARITHMETIC_IMM::SRLI_SRAI]=inst::SRLI};
            static transformer_t arr={{ARITHMETIC_IMM::ADDI,inst::ADDI}, {ARITHMETIC_IMM::SLTI,inst::SLTI}, {ARITHMETIC_IMM::SLTIU,inst::SLTIU}, {ARITHMETIC_IMM::XORI,inst::XORI}, {ARITHMETIC_IMM::ORI,inst::ORI}, {ARITHMETIC_IMM::ANDI,inst::ANDI}, {ARITHMETIC_IMM::SLLI,inst::SLLI}, {ARITHMETIC_IMM::SRLI_SRAI,inst::SRLI}};
            rt = static_cast<inst::inst_t>(arr[ID_funct3]);
            if (ID_funct3 == ARITHMETIC_IMM::SRLI_SRAI)
                rt = ID_funct7 == ARITHMETIC_IMM::FUNC7_SRAI ? inst::SRAI : inst::SRLI;
        }
            break;
        case op::ARITHMETIC: {
//            dword_t arr[]{[ARITHMETIC::ADD_SUB]=inst::ADD, [ARITHMETIC::SLT]=inst::SLT, [ARITHMETIC::SLTU]=inst::SLTU, [ARITHMETIC::XOR]=inst::XOR, [ARITHMETIC::OR]=inst::OR, [ARITHMETIC::AND]=inst::AND, [ARITHMETIC::SLL]=inst::SLL, [ARITHMETIC::SRL_SRA]=inst::SRL,};
            static transformer_t arr{{ARITHMETIC::ADD_SUB,inst::ADD}, {ARITHMETIC::SLT,inst::SLT}, {ARITHMETIC::SLTU,inst::SLTU}, {ARITHMETIC::XOR,inst::XOR}, {ARITHMETIC::OR,inst::OR}, {ARITHMETIC::AND,inst::AND}, {ARITHMETIC::SLL,inst::SLL}, {ARITHMETIC::SRL_SRA,inst::SRL}};
            rt = static_cast<inst::inst_t>(arr[ID_funct3]);
            if (ID_funct3 == ARITHMETIC::SRL_SRA)rt = ID_funct7 == ARITHMETIC::FUNC7_SRA ? inst::SRA : inst::SRL;
            if (ID_funct3 == ARITHMETIC::ADD_SUB)rt = ID_funct7 == ARITHMETIC::FUNC7_ADD ? inst::ADD : inst::SUB;
        }
            break;
        default:
            // error
            rt=inst::NOP;
#ifndef N_DEBUG
            std::cerr<<"INVALID instruction detected. May be fatal or pre-fetched code"<<std::endl;
#endif
//            throw std::logic_error("WRONG OPCODE");
    }
//    assert(rt!=inst::NOP);
    return rt;
}
dword_t riscv::b_decode(sgn_dword_t a) {
    sgn_dword_t mid = (((a >> 20) & 0xffffffe0) | (a >> 7 & 0x1f));
    mid &= 0xfffff7ff;
    return (mid | ((a & 0x80) << 4)) & 0xfffffffe;
}
dword_t riscv::j_decode(sgn_dword_t a) {
    sgn_dword_t mid = (a >> 20);
    mid &= 0xfff007fe;
    mid|=((a&0x100000)>>9);
    return mid|(a&0x000ff000);
}
dword_t riscv::i_decode(sgn_dword_t a) {
    return ((sgn_dword_t) (a & IMM_I_MASK)) >> 20;
}

inst::inst_t riscv::select_jmp(dword_t inst) {
    switch (inst&OPCODE_MASK) {
        case op::JAL:
            return riscv::inst::JAL;
        case op::AUIPC:
            return riscv::inst::AUIPC;
        case op::JALR:
            return inst::JALR;
        case op::BRANCH:
            return inst::BEQ; // as a symbol, not really BEQ
        default:
            return riscv::inst::NOP;
    }
}
