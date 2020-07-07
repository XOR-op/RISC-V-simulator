//
// Created by Vortox Oganesson on 7/6/20.
//

#ifndef RISC_V_SIMULATOR_CONSTANT_H
#define RISC_V_SIMULATOR_CONSTANT_H
namespace riscv{
    namespace op{
        enum op{
            LUI=0b0110111,
            AUIPC=0b0010111,
            JAL=0b1101111,
            JALR=0b1100111,
            BRANCH=0b1100011,
            LOAD=0b0000011,
            STORE=0b0100011,
            ARITHMETIC_IMM=0b0010011,
            ARITHMETIC=0b0110011
        };
    }
    namespace BRANCH{
        enum funct3{
            BEQ,BNE,BLT=0b100,BGE,BLTU,BGEU
        };
    }
    namespace LOAD{
        enum funct3{
            LB,LH,LW,LBU=0b100,LHU
        };
    }
    namespace STORE{
        enum funct3{
            SB,SH,SW
        };
    }
    namespace ARITHMETIC_IMM{
        enum funct3{
            ADDI,SLLI,SLTI,SLTIU,XORI,SRLI_SRAI,ORI,ANDI
        };
        enum funct7{
            FUNC7_SRLI=0,
            FUNC7_SRAI=0b0100000
        };
    }
    namespace ARITHMETIC{
        enum funct3{
            ADD_SUB,SLL,SLT,SLTU,XOR,SRL_SRA,OR,AND
        };
        enum funct7{
            FUNC7_SRL=0,
            FUNC7_SRA=0b0100000,
            FUNC7_ADD=0,
            FUNC7_SUB=0b0100000
        };
    }
    namespace inst{
        enum {
            NOP,
            LUI,AUIPC,JAL,JALR,
            BEQ,BNE,BLT,BGE,BLTU,BGEU,
            LB,LH,LW,LBU,LHU,
            SB,SH,SW,
            ADDI,SLLI,SLTI,SLTIU,XORI,SRLI,SRAI,ORI,ANDI,
            ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND
        };
    }
}
#endif //RISC_V_SIMULATOR_CONSTANT_H
