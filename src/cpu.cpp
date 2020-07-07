//
// Created by Vortox Oganesson on 7/6/20.
//
#include "cpu.h"
#include "constant.h"

using namespace riscv;
const static dword_t OPCODE_MASK = 0b1111111;
const static dword_t FUNCT3_MASK = 0b111u << 12u;
const static dword_t FUNCT7_MASK = 0b1111111u << 25u;
const static dword_t RS1_MASK = 0b11111u << 15u;
const static dword_t RS2_MASK = 0b11111u << 20u;
const static dword_t RD_MASK = 0b11111u << 7u;
const static dword_t IMM_I_MASK = 0xfffu << 20u;
const static dword_t IMM_U_MASK = 0xfffffu << 12u;
bool r1_table[]{[inst::LUI]=false, [inst::AUIPC]=false, [inst::JAL]=false, [inst::JALR]=true, [inst::BEQ]=true, [inst::BNE]=true, [inst::BLT]=true, [inst::BGE]=true, [inst::BLTU]=true, [inst::BGEU]=true, [inst::LB]=true, [inst::LH]=true, [inst::LW]=true, [inst::LBU]=true, [inst::LHU]=true, [inst::ADDI]=true, [inst::SLTI]=true, [inst::SLTIU]=true, [inst::XORI]=true, [inst::ORI]=true, [inst::ANDI]=true, [inst::SB]=true, [inst::SH]=true, [inst::SW]=true, [inst::SLLI]=true, [inst::SRLI]=true, [inst::SRAI]=true, [inst::ADD]=true, [inst::SUB]=true, [inst::SLT]=true, [inst::SLTU]=true, [inst::XOR]=true, [inst::OR]=true, [inst::AND]=true, [inst::SLL]=true, [inst::SRL]=true, [inst::SRA]=true,};
bool r2_table[]{[inst::LUI]=false, [inst::AUIPC]=false, [inst::JAL]=false, [inst::JALR]=false, [inst::BEQ]=true, [inst::BNE]=true, [inst::BLT]=true, [inst::BGE]=true, [inst::BLTU]=true, [inst::BGEU]=true, [inst::LB]=false, [inst::LH]=false, [inst::LW]=false, [inst::LBU]=false, [inst::LHU]=false, [inst::ADDI]=false, [inst::SLTI]=false, [inst::SLTIU]=false, [inst::XORI]=false, [inst::ORI]=false, [inst::ANDI]=false, [inst::SB]=true, [inst::SH]=true, [inst::SW]=true, [inst::SLLI]=false, [inst::SRLI]=false, [inst::SRAI]=false, [inst::ADD]=true, [inst::SUB]=true, [inst::SLT]=true, [inst::SLTU]=true, [inst::XOR]=true, [inst::OR]=true, [inst::AND]=true, [inst::SLL]=true, [inst::SRL]=true, [inst::SRA]=true,};
bool rd_table[]{[inst::LUI]=true, [inst::AUIPC]=true, [inst::JAL]=true, [inst::JALR]=true, [inst::BEQ]=false, [inst::BNE]=false, [inst::BLT]=false, [inst::BGE]=false, [inst::BLTU]=false, [inst::BGEU]=false, [inst::LB]=true, [inst::LH]=true, [inst::LW]=true, [inst::LBU]=true, [inst::LHU]=true, [inst::ADDI]=true, [inst::SLTI]=true, [inst::SLTIU]=true, [inst::XORI]=true, [inst::ORI]=true, [inst::ANDI]=true, [inst::SB]=false, [inst::SH]=false, [inst::SW]=false, [inst::SLLI]=true, [inst::SRLI]=true, [inst::SRAI]=true, [inst::ADD]=true, [inst::SUB]=true, [inst::SLT]=true, [inst::SLTU]=true, [inst::XOR]=true, [inst::OR]=true, [inst::AND]=true, [inst::SLL]=true, [inst::SRL]=true, [inst::SRA]=true,};
char imm_table[]{[inst::LUI]='U', [inst::AUIPC]='U', [inst::JAL]='J', [inst::JALR]='I', [inst::BEQ]='B', [inst::BNE]='B', [inst::BLT]='B', [inst::BGE]='B', [inst::BLTU]='B', [inst::BGEU]='B', [inst::LB]='I', [inst::LH]='I', [inst::LW]='I', [inst::LBU]='I', [inst::LHU]='I', [inst::ADDI]='I', [inst::SLTI]='I', [inst::SLTIU]='I', [inst::XORI]='I', [inst::ORI]='I', [inst::ANDI]='I', [inst::SB]='S', [inst::SH]='S', [inst::SW]='S', [inst::SLLI]='T', [inst::SRLI]='T', [inst::SRAI]='T', [inst::ADD]='R', [inst::SUB]='R', [inst::SLT]='R', [inst::SLTU]='R', [inst::XOR]='R', [inst::OR]='R', [inst::AND]='R', [inst::SLL]='R', [inst::SRL]='R', [inst::SRA]='R',};
const static dword_t TEST_ = 0xffffffff;

dword_t select_op(dword_t inst) {
    dword_t rt=inst::NOP;
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
            dword_t arr[]{[BRANCH::BEQ]=inst::BEQ, [BRANCH::BNE]=inst::BNE, [BRANCH::BLT]=inst::BLT, [BRANCH::BGE]=inst::BGE, [BRANCH::BLTU]=inst::BLTU, [BRANCH::BGEU]=inst::BGEU,};
            rt = arr[ID_funct3];
        }
            break;
        case op::LOAD: {
            dword_t arr[]{[LOAD::LB]=inst::LB, [LOAD::LH]=inst::LH, [LOAD::LW]=inst::LW,[LOAD::LBU]=inst::LBU, [LOAD::LHU]=inst::LHU,};
            rt = arr[ID_funct3];
        }
            break;
        case op::STORE: {
            dword_t arr[]{[STORE::SB]=inst::SB, [STORE::SH]=inst::SH, [STORE::SW]=inst::SW,};
            rt = arr[ID_funct3];
        }
            break;
        case op::ARITHMETIC_IMM: {
            dword_t arr[]{[ARITHMETIC_IMM::ADDI]=inst::ADDI, [ARITHMETIC_IMM::SLTI]=inst::SLTI, [ARITHMETIC_IMM::SLTIU]=inst::SLTIU, [ARITHMETIC_IMM::XORI]=inst::XORI, [ARITHMETIC_IMM::ORI]=inst::ORI, [ARITHMETIC_IMM::ANDI]=inst::ANDI, [ARITHMETIC_IMM::SLLI]=inst::SLLI, [ARITHMETIC_IMM::SRLI_SRAI]=inst::SRLI};
            rt = arr[ID_funct3];
            if (ID_funct3 == ARITHMETIC_IMM::SRLI_SRAI)
                rt = ID_funct7 == ARITHMETIC_IMM::FUNC7_SRAI ? inst::SRAI : inst::SRLI;
        }
            break;
        case op::ARITHMETIC: {
            dword_t arr[]{[ARITHMETIC::ADD_SUB]=inst::ADD, [ARITHMETIC::SLT]=inst::SLT, [ARITHMETIC::SLTU]=inst::SLTU, [ARITHMETIC::XOR]=inst::XOR, [ARITHMETIC::OR]=inst::OR, [ARITHMETIC::AND]=inst::AND, [ARITHMETIC::SLL]=inst::SLL, [ARITHMETIC::SRL_SRA]=inst::SRL,};
            rt = arr[ID_funct3];
            if (ID_funct3 == ARITHMETIC::SRL_SRA)rt = ID_funct7 == ARITHMETIC::FUNC7_SRA ? inst::SRA : inst::SRL;
            if (ID_funct3 == ARITHMETIC::ADD_SUB)rt = ID_funct7 == ARITHMETIC::FUNC7_ADD ? inst::ADD : inst::SUB;
        }
            break;
        default:
            // error
            throw std::logic_error("WRONG OPCODE");
    }
    assert(rt!=inst::NOP);
    return rt;
}
void cpu::clockIn() {
    ID_inst=IF_ID_inst,ID_pc=IF_ID_pc;
    EX_op=ID_EX_op,EX_r1=ID_EX_r1,EX_r2=ID_EX_r2,EX_rd=ID_EX_rd,EX_imm=ID_EX_imm,EX_pc=ID_EX_pc;
    MEM_op=EX_MEM_op, MEM_rd=EX_MEM_rd, MEM_value=EX_MEM_output,MEM_r2=EX_MEM_r2;
    WB_op=MEM_WB_op,WB_reg=MEM_WB_output,WB_rd=MEM_WB_rd;
}
void cpu::IF_stage() {
    assert(pc<=0x20000);
    assert(registers[0]==0);
    IF_ID_inst = memory->read32(pc);
    IF_ID_pc=pc;
    std::stringstream ss;std::string str;
    ss<<std::hex<<IF_ID_inst;ss>>str;
    inst_history.emplace_back(str);
    if(IF_ID_inst==0x00c68223)throw terminal_exception();
    pc+=4;
}
dword_t b_decode(sgn_dword_t a) {
    sgn_dword_t mid = (((a >> 20) & 0xffffffe0) | (a >> 7 & 0x1f));
    mid &= 0xfffff7ff;
    return (mid | ((a & 0x80) << 4)) & 0xfffffffe;
}
dword_t j_decode(sgn_dword_t a) {
    sgn_dword_t mid = (a >> 20);
    mid &= 0xfff007fe;
    mid|=((a&0x100000)>>9);
    return mid|(a&0x000ff000);
}
dword_t i_decode(sgn_dword_t a) {
    return ((sgn_dword_t) (a & IMM_I_MASK)) >> 20;
}



void cpu::ID_stage() {
    ID_EX_op = select_op(ID_inst);
    assert(ID_EX_op);
    if (r1_table[ID_EX_op])ID_EX_r1 = registers[(ID_inst & RS1_MASK) >> 15];
    else ID_EX_r1 = TEST_;
    if (r2_table[ID_EX_op])ID_EX_r2 = registers[(ID_inst & RS2_MASK) >> 20];
    else ID_EX_r2 = TEST_;
    if (rd_table[ID_EX_op])ID_EX_rd = (ID_inst & RD_MASK) >> 7;
    else ID_EX_rd = TEST_;
    ID_EX_imm = TEST_;
    ID_EX_pc=ID_pc;
    switch (imm_table[ID_EX_op]) {
        case 'R':
            // do nothing
            break;
        case 'I':
            ID_EX_imm = i_decode((sgn_dword_t) ID_inst);
            break;
        case 'U':
            ID_EX_imm = ID_inst & IMM_U_MASK;
            break;
        case 'S':
            ID_EX_imm = ((((sgn_dword_t) ID_inst) >> 20) & 0xffffffe0) | (ID_inst >> 7 & 0x1f);
            break;
        case 'B':
            ID_EX_imm = b_decode((sgn_dword_t) ID_inst);
            break;
        case 'J':
            ID_EX_imm = j_decode((sgn_dword_t) ID_inst);
            break;
        case 'T':
            // shamt
            ID_EX_imm = ((dword_t) i_decode((sgn_dword_t) ID_inst)) & 0x1f;
            break;
        default:
            throw std::logic_error("WRONG DECODE TYPE");
    }
}



void cpu::EX_stage() {
    using namespace inst;
    switch (EX_op) {
        case LUI:{
            EX_MEM_output=EX_imm;
        }break;
        case AUIPC:{
            pc=EX_pc+EX_imm;
            EX_MEM_output=pc;
        }break;
        case JAL:{
            EX_MEM_output=EX_pc+4;
            pc=EX_pc+EX_imm;
            assert(pc<=0x20000);
        }break;
        case JALR:{
            EX_MEM_output=EX_pc+4;
            pc=EX_r1+EX_imm;
        }break;
        case BEQ:{
            if(EX_r1==EX_r2)pc=EX_pc+EX_imm;
        }break;
        case BNE:{
            if(EX_r1!=EX_r2)pc=EX_pc+EX_imm;
        }break;
        case BLT:{
            if((sgn_dword_t )EX_r1<(sgn_dword_t)EX_r2)pc=EX_pc+EX_imm;
        }break;
        case BGE:{
            if((sgn_dword_t )EX_r1>=(sgn_dword_t)EX_r2)pc=EX_pc+EX_imm;
        }break;
        case BLTU:{
            if(EX_r1<EX_r2)pc=EX_pc+EX_imm;
        }break;
        case BGEU:{
            if(EX_r1>=EX_r2)pc=EX_pc+EX_imm;
        }break;
        case LB:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case LH:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case LW:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case LBU:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case LHU:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case ADDI:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case SLTIU:{
            EX_MEM_output=EX_r1<EX_imm;
        }break;
        case SLTI:{
            EX_MEM_output=(sgn_dword_t)EX_r1<(sgn_dword_t)EX_imm;
        }break;
        case XORI:{
            EX_MEM_output=EX_r1^EX_imm;
        }break;
        case ORI:{
            EX_MEM_output=EX_r1|EX_imm;
        }break;
        case ANDI:{
            EX_MEM_output=EX_r1&EX_imm;
        }break;
        case SB:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case SH:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case SW:{
            EX_MEM_output=EX_r1+EX_imm;
        }break;
        case SLLI:{
            EX_MEM_output=EX_r1<<EX_imm;
        }break;
        case SRLI:{
            EX_MEM_output=EX_r1>>EX_imm;
        }break;
        case SRAI:{
            EX_MEM_output=((sgn_dword_t)EX_r1)>>EX_imm;
        }break;
        case ADD:{
            EX_MEM_output=EX_r1+EX_r2;
        }break;
        case SUB:{
            EX_MEM_output=EX_r1-EX_r2;
        }break;
        case SLT:{
            EX_MEM_output=(sgn_dword_t )EX_r1<(sgn_dword_t )EX_r2;
        }break;
        case SLTU:{
            EX_MEM_output=EX_r1<EX_r2;
        }break;
        case XOR:{
            EX_MEM_output=EX_r1^EX_r2;
        }break;
        case OR:{
            EX_MEM_output=EX_r1|EX_r2;

        }break;
        case AND:{
            EX_MEM_output=EX_r1&EX_r2;

        }break;
        case SLL:{
            EX_MEM_output=EX_r1<<EX_r2;

        }break;
        case SRL:{
            EX_MEM_output=EX_r1>>EX_r2;
        }break;
        case SRA:{
            EX_MEM_output=((sgn_dword_t )EX_r1)>>EX_r2;
        }break;
        default:
            throw std::logic_error("WRONG OP!");
    }
    EX_MEM_op=EX_op;
    EX_MEM_rd=EX_rd;
    EX_MEM_r2=EX_r2;
}



void cpu::MEM_stage() {
    using namespace inst;
    MEM_WB_op=MEM_op;
    MEM_WB_rd=MEM_rd;
    MEM_WB_output=MEM_value;
    switch (MEM_op) {
        case LB:{
            MEM_WB_output=memory->read8s(MEM_value);
        }break;
        case LH:{
            MEM_WB_output=memory->read16s(MEM_value);
        }break;
        case LW:{
            MEM_WB_output=memory->read32s(MEM_value);
        }break;
        case LBU:{
            MEM_WB_output=memory->read8(MEM_value);
        }break;
        case LHU:{
            MEM_WB_output=memory->read16(MEM_value);
        }break;
        case SB:{
            memory->write8(MEM_value, MEM_r2 & 0xff);
        }break;
        case SH:{
            memory->write16(MEM_value, MEM_r2 & 0xffff);
        }break;
        case SW:{
            memory->write32(MEM_value, MEM_r2);
        }break;
    }
}



void cpu::WB_stage() {
    using namespace inst;
    switch (WB_op) {
        case LUI:
        case AUIPC:
        case JAL:
        case JALR:
        case LB:
        case LH:
        case LW:
        case LBU:
        case LHU:
        case ADDI:
        case SLTI:
        case SLTIU:
        case XORI:
        case ORI:
        case ANDI:
        case SLLI:
        case SRLI:
        case SRAI:
        case ADD:
        case SUB:
        case SLT:
        case SLTU:
        case XOR:
        case OR:
        case AND:
        case SLL:
        case SRL:
        case SRA:
            assert(WB_rd<32);
            if(WB_rd!=0)
                registers[WB_rd]=WB_reg;
            break;
    }
}



cpu::cpu(struct memory* mem_ptr) {
    memory=mem_ptr;
    pc=0;
    memset(registers,-1,sizeof(registers));
    registers[0]=0;
}
