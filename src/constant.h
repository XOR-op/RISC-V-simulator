#ifndef RISC_V_SIMULATOR_CONSTANT_H
#define RISC_V_SIMULATOR_CONSTANT_H
namespace riscv{
    const static dword_t END_INST=0x0ff00513;
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
        enum inst_t{
            NOP,HUG,
            LUI,AUIPC,JAL,JALR,
            BEQ,BNE,BLT,BGE,BLTU,BGEU,
            LB,LH,LW,LBU,LHU,
            ADDI,SLTI,SLTIU,XORI,ORI,ANDI,
            SB,SH,SW,
            SLLI,SRLI,SRAI,
            ADD,SUB,SLT,SLTU,XOR,OR,AND,SLL,SRL,SRA,
            END_I
        };
    }
    static bool match(dword_t a,dword_t b,dword_t target){
        return target!=0&&(target==a||target==b);
    }
}
#endif //RISC_V_SIMULATOR_CONSTANT_H
