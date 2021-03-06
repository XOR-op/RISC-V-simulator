#include "cpu.h"
#include "constant.h"
#include "decoding.h"
#include <unordered_map>

using std::unordered_map;
using namespace riscv;
const static dword_t RS1_MASK = 0b11111u << 15u;
const static dword_t RS2_MASK = 0b11111u << 20u;
const static dword_t RD_MASK = 0b11111u << 7u;
const static dword_t IMM_U_MASK = 0xfffffu << 12u;
/*
bool r1_table[]{[inst::LUI]=false, [inst::AUIPC]=false, [inst::JAL]=false, [inst::JALR]=true, [inst::BEQ]=true, [inst::BNE]=true, [inst::BLT]=true, [inst::BGE]=true, [inst::BLTU]=true, [inst::BGEU]=true, [inst::LB]=true, [inst::LH]=true, [inst::LW]=true, [inst::LBU]=true, [inst::LHU]=true, [inst::ADDI]=true, [inst::SLTI]=true, [inst::SLTIU]=true, [inst::XORI]=true, [inst::ORI]=true, [inst::ANDI]=true, [inst::SB]=true, [inst::SH]=true, [inst::SW]=true, [inst::SLLI]=true, [inst::SRLI]=true, [inst::SRAI]=true, [inst::ADD]=true, [inst::SUB]=true, [inst::SLT]=true, [inst::SLTU]=true, [inst::XOR]=true, [inst::OR]=true, [inst::AND]=true, [inst::SLL]=true, [inst::SRL]=true, [inst::SRA]=true,};
bool r2_table[]{[inst::LUI]=false, [inst::AUIPC]=false, [inst::JAL]=false, [inst::JALR]=false, [inst::BEQ]=true, [inst::BNE]=true, [inst::BLT]=true, [inst::BGE]=true, [inst::BLTU]=true, [inst::BGEU]=true, [inst::LB]=false, [inst::LH]=false, [inst::LW]=false, [inst::LBU]=false, [inst::LHU]=false, [inst::ADDI]=false, [inst::SLTI]=false, [inst::SLTIU]=false, [inst::XORI]=false, [inst::ORI]=false, [inst::ANDI]=false, [inst::SB]=true, [inst::SH]=true, [inst::SW]=true, [inst::SLLI]=false, [inst::SRLI]=false, [inst::SRAI]=false, [inst::ADD]=true, [inst::SUB]=true, [inst::SLT]=true, [inst::SLTU]=true, [inst::XOR]=true, [inst::OR]=true, [inst::AND]=true, [inst::SLL]=true, [inst::SRL]=true, [inst::SRA]=true,};
bool rd_table[]{[inst::LUI]=true, [inst::AUIPC]=true, [inst::JAL]=true, [inst::JALR]=true, [inst::BEQ]=false, [inst::BNE]=false, [inst::BLT]=false, [inst::BGE]=false, [inst::BLTU]=false, [inst::BGEU]=false, [inst::LB]=true, [inst::LH]=true, [inst::LW]=true, [inst::LBU]=true, [inst::LHU]=true, [inst::ADDI]=true, [inst::SLTI]=true, [inst::SLTIU]=true, [inst::XORI]=true, [inst::ORI]=true, [inst::ANDI]=true, [inst::SB]=false, [inst::SH]=false, [inst::SW]=false, [inst::SLLI]=true, [inst::SRLI]=true, [inst::SRAI]=true, [inst::ADD]=true, [inst::SUB]=true, [inst::SLT]=true, [inst::SLTU]=true, [inst::XOR]=true, [inst::OR]=true, [inst::AND]=true, [inst::SLL]=true, [inst::SRL]=true, [inst::SRA]=true,};
char imm_table[]{[inst::LUI]='U', [inst::AUIPC]='U', [inst::JAL]='J', [inst::JALR]='I', [inst::BEQ]='B', [inst::BNE]='B', [inst::BLT]='B', [inst::BGE]='B', [inst::BLTU]='B', [inst::BGEU]='B', [inst::LB]='I', [inst::LH]='I', [inst::LW]='I', [inst::LBU]='I', [inst::LHU]='I', [inst::ADDI]='I', [inst::SLTI]='I', [inst::SLTIU]='I', [inst::XORI]='I', [inst::ORI]='I', [inst::ANDI]='I', [inst::SB]='S', [inst::SH]='S', [inst::SW]='S', [inst::SLLI]='T', [inst::SRLI]='T', [inst::SRAI]='T', [inst::ADD]='R', [inst::SUB]='R', [inst::SLT]='R', [inst::SLTU]='R', [inst::XOR]='R', [inst::OR]='R', [inst::AND]='R', [inst::SLL]='R', [inst::SRL]='R', [inst::SRA]='R', [inst::END_I]='R'};
bool access_mem_table[]{[inst::LUI]=false, [inst::AUIPC]=false, [inst::JAL]=false, [inst::JALR]=false, [inst::BEQ]=false, [inst::BNE]=false, [inst::BLT]=false, [inst::BGE]=false, [inst::BLTU]=false, [inst::BGEU]=false, [inst::LB]=true, [inst::LH]=true, [inst::LW]=true, [inst::LBU]=true, [inst::LHU]=true, [inst::ADDI]=false, [inst::SLTI]=false, [inst::SLTIU]=false, [inst::XORI]=false, [inst::ORI]=false, [inst::ANDI]=false, [inst::SB]=true, [inst::SH]=true, [inst::SW]=true, [inst::SLLI]=false, [inst::SRLI]=false, [inst::SRAI]=false, [inst::ADD]=false, [inst::SUB]=false, [inst::SLT]=false, [inst::SLTU]=false, [inst::XOR]=false, [inst::OR]=false, [inst::AND]=false, [inst::SLL]=false, [inst::SRL]=false, [inst::SRA]=false,};
bool branch_table[]{[inst::LUI]=false, [inst::AUIPC]=false, [inst::JAL]=false, [inst::JALR]=false, [inst::BEQ]=true, [inst::BNE]=true, [inst::BLT]=true, [inst::BGE]=true, [inst::BLTU]=true, [inst::BGEU]=true, [inst::LB]=false, [inst::LH]=false, [inst::LW]=false, [inst::LBU]=false, [inst::LHU]=false, [inst::ADDI]=false, [inst::SLTI]=false, [inst::SLTIU]=false, [inst::XORI]=false, [inst::ORI]=false, [inst::ANDI]=false, [inst::SB]=false, [inst::SH]=false, [inst::SW]=false, [inst::SLLI]=false, [inst::SRLI]=false, [inst::SRAI]=false, [inst::ADD]=false, [inst::SUB]=false, [inst::SLT]=false, [inst::SLTU]=false, [inst::XOR]=false, [inst::OR]=false, [inst::AND]=false, [inst::SLL]=false, [inst::SRL]=false, [inst::SRA]=false, [inst::END_I]=false};
*/
unordered_map<inst::inst_t,bool> r1_table{{inst::LUI,false},{inst::AUIPC,false},{inst::JAL,false},{inst::JALR,true},{inst::BEQ,true},{inst::BNE,true},{inst::BLT,true},{inst::BGE,true},{inst::BLTU,true},{inst::BGEU,true},{inst::LB,true},{inst::LH,true},{inst::LW,true},{inst::LBU,true},{inst::LHU,true},{inst::ADDI,true},{inst::SLTI,true},{inst::SLTIU,true},{inst::XORI,true},{inst::ORI,true},{inst::ANDI,true},{inst::SB,true},{inst::SH,true},{inst::SW,true},{inst::SLLI,true},{inst::SRLI,true},{inst::SRAI,true},{inst::ADD,true},{inst::SUB,true},{inst::SLT,true},{inst::SLTU,true},{inst::XOR,true},{inst::OR,true},{inst::AND,true},{inst::SLL,true},{inst::SRL,true},{inst::SRA,true},};
unordered_map<inst::inst_t,bool> r2_table{{inst::LUI,false},{inst::AUIPC,false},{inst::JAL,false},{inst::JALR,false},{inst::BEQ,true},{inst::BNE,true},{inst::BLT,true},{inst::BGE,true},{inst::BLTU,true},{inst::BGEU,true},{inst::LB,false},{inst::LH,false},{inst::LW,false},{inst::LBU,false},{inst::LHU,false},{inst::ADDI,false},{inst::SLTI,false},{inst::SLTIU,false},{inst::XORI,false},{inst::ORI,false},{inst::ANDI,false},{inst::SB,true},{inst::SH,true},{inst::SW,true},{inst::SLLI,false},{inst::SRLI,false},{inst::SRAI,false},{inst::ADD,true},{inst::SUB,true},{inst::SLT,true},{inst::SLTU,true},{inst::XOR,true},{inst::OR,true},{inst::AND,true},{inst::SLL,true},{inst::SRL,true},{inst::SRA,true},};
unordered_map<inst::inst_t,bool> rd_table{{inst::LUI,true},{inst::AUIPC,true},{inst::JAL,true},{inst::JALR,true},{inst::BEQ,false},{inst::BNE,false},{inst::BLT,false},{inst::BGE,false},{inst::BLTU,false},{inst::BGEU,false},{inst::LB,true},{inst::LH,true},{inst::LW,true},{inst::LBU,true},{inst::LHU,true},{inst::ADDI,true},{inst::SLTI,true},{inst::SLTIU,true},{inst::XORI,true},{inst::ORI,true},{inst::ANDI,true},{inst::SB,false},{inst::SH,false},{inst::SW,false},{inst::SLLI,true},{inst::SRLI,true},{inst::SRAI,true},{inst::ADD,true},{inst::SUB,true},{inst::SLT,true},{inst::SLTU,true},{inst::XOR,true},{inst::OR,true},{inst::AND,true},{inst::SLL,true},{inst::SRL,true},{inst::SRA,true},};
unordered_map<inst::inst_t,char> imm_table{{inst::LUI,'U'},{inst::AUIPC,'U'},{inst::JAL,'J'},{inst::JALR,'I'},{inst::BEQ,'B'},{inst::BNE,'B'},{inst::BLT,'B'},{inst::BGE,'B'},{inst::BLTU,'B'},{inst::BGEU,'B'},{inst::LB,'I'},{inst::LH,'I'},{inst::LW,'I'},{inst::LBU,'I'},{inst::LHU,'I'},{inst::ADDI,'I'},{inst::SLTI,'I'},{inst::SLTIU,'I'},{inst::XORI,'I'},{inst::ORI,'I'},{inst::ANDI,'I'},{inst::SB,'S'},{inst::SH,'S'},{inst::SW,'S'},{inst::SLLI,'T'},{inst::SRLI,'T'},{inst::SRAI,'T'},{inst::ADD,'R'},{inst::SUB,'R'},{inst::SLT,'R'},{inst::SLTU,'R'},{inst::XOR,'R'},{inst::OR,'R'},{inst::AND,'R'},{inst::SLL,'R'},{inst::SRL,'R'},{inst::SRA,'R'},{inst::END_I,'R'}};
unordered_map<inst::inst_t,bool> access_mem_table{{inst::LUI,false},{inst::AUIPC,false},{inst::JAL,false},{inst::JALR,false},{inst::BEQ,false},{inst::BNE,false},{inst::BLT,false},{inst::BGE,false},{inst::BLTU,false},{inst::BGEU,false},{inst::LB,true},{inst::LH,true},{inst::LW,true},{inst::LBU,true},{inst::LHU,true},{inst::ADDI,false},{inst::SLTI,false},{inst::SLTIU,false},{inst::XORI,false},{inst::ORI,false},{inst::ANDI,false},{inst::SB,true},{inst::SH,true},{inst::SW,true},{inst::SLLI,false},{inst::SRLI,false},{inst::SRAI,false},{inst::ADD,false},{inst::SUB,false},{inst::SLT,false},{inst::SLTU,false},{inst::XOR,false},{inst::OR,false},{inst::AND,false},{inst::SLL,false},{inst::SRL,false},{inst::SRA,false},};
unordered_map<inst::inst_t,bool> branch_table{{inst::LUI,false},{inst::AUIPC,false},{inst::JAL,false},{inst::JALR,false},{inst::BEQ,true},{inst::BNE,true},{inst::BLT,true},{inst::BGE,true},{inst::BLTU,true},{inst::BGEU,true},{inst::LB,false},{inst::LH,false},{inst::LW,false},{inst::LBU,false},{inst::LHU,false},{inst::ADDI,false},{inst::SLTI,false},{inst::SLTIU,false},{inst::XORI,false},{inst::ORI,false},{inst::ANDI,false},{inst::SB,false},{inst::SH,false},{inst::SW,false},{inst::SLLI,false},{inst::SRLI,false},{inst::SRAI,false},{inst::ADD,false},{inst::SUB,false},{inst::SLT,false},{inst::SLTU,false},{inst::XOR,false},{inst::OR,false},{inst::AND,false},{inst::SLL,false},{inst::SRL,false},{inst::SRA,false},{inst::END_I, false}};
const static dword_t TEST_ = 0xffffffff;

void cpu::IF_stage() {
    assert(pc <= 0x20000);
    assert(registers[0] == 0);
    IF_ID_inst = mem->read32(pc);
    IF_ID_pc = pc;
#ifndef N_DEBUG
//    std::stringstream ss;
//    std::string str;
//    ss << std::hex << IF_ID_inst;
//    ss >> str;
//    inst_history.emplace_back(str);
#endif
    pc += 4;
}


void cpu::ID_stage() {
    if (ID_inst == inst::HUG)return;
    else if (ID_inst == inst::NOP) {
        ID_EX_op = inst::NOP;
        return;
    }
    ID_EX_op = select_op(ID_inst);
    if (ID_EX_op == inst::NOP) {
        ID_EX_r1 = ID_EX_r2 = ID_EX_rd = 0;
        return;
    }
    ID_EX_r1 = r1_table[ID_EX_op] ? (ID_inst & RS1_MASK) >> 15 : 0;
    ID_EX_r2 = r2_table[ID_EX_op] ? (ID_inst & RS2_MASK) >> 20 : 0;
    ID_EX_rA = registers[ID_EX_r1];
    ID_EX_rB = registers[ID_EX_r2];
    ID_EX_rd = rd_table[ID_EX_op] ? (ID_inst & RD_MASK) >> 7 : 0;
    ID_EX_imm = TEST_;
    ID_EX_pc = ID_pc;
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

    if (ID_EX_op == inst::JAL || ID_EX_op == inst::AUIPC || branch_table[ID_EX_op]) {
        // jmp or branch
        using namespace inst;
        switch (ID_EX_op) {
            case JAL:
                pc = ID_pc+ID_EX_imm;
                clear_stat = CLEAR_IF_ID;
                assert(pc <= 0x20000);
                break;
            case AUIPC:
                pc = ID_pc+ID_EX_imm;
                clear_stat = CLEAR_IF_ID;
                break;
            case BEQ:
            case BNE:
            case BLT:
            case BLTU:
            case BGE:
            case BGEU: {
                ID_EX_is_jmp_bit = pre.predict(ID_pc);
                if (ID_EX_is_jmp_bit) {
                    pc = ID_pc+ID_EX_imm;
                    clear_stat = CLEAR_IF_ID;
                }
            }
                break;
            case END_I:
                break; // do not change PC
            default:
                assert(0);
        }
    }

}


void cpu::EX_stage() {
    using namespace inst;
//    if (EX_op == HUG)return;else
    if (EX_op == NOP || EX_op == HUG) {
        EX_MEM_op = NOP;
        EX_MEM_rd = 0;
        return;
    } else if (EX_op == END_I) {
        EX_MEM_op = END_I;
        return;
    }
    bool hit = false;
    switch (EX_op) {
        case LUI: {
            EX_MEM_ALU_output = EX_imm;
        }
            break;
        case BEQ: {
            if (EX_rA == EX_rB)hit = true;
        }
            break;
        case BNE: {
            if (EX_rA != EX_rB)hit = true;
        }
            break;
        case BLT: {
            if ((sgn_dword_t) EX_rA < (sgn_dword_t) EX_rB)hit = true;
        }
            break;
        case BGE: {
            if ((sgn_dword_t) EX_rA >= (sgn_dword_t) EX_rB)hit = true;
        }
            break;
        case BLTU: {
            if (EX_rA < EX_rB)hit = true;
        }
            break;
        case BGEU: {
            if (EX_rA >= EX_rB)hit = true;
        }
            break;
        case AUIPC: {
            EX_MEM_ALU_output = EX_pc+4;
        }
            break;
        case JAL: {
            EX_MEM_ALU_output = EX_pc+4;
        }
            break;
        case JALR: {
            pc = (EX_rA+EX_imm) & 0xfffffffe;
            clear_stat = CLEAR_IF_EX;
            EX_MEM_ALU_output = EX_pc+4;
        }
            break;
        case LB: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case LH: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case LW: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case LBU: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case LHU: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case ADDI: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case SLTIU: {
            EX_MEM_ALU_output = EX_rA < EX_imm;
        }
            break;
        case SLTI: {
            EX_MEM_ALU_output = (sgn_dword_t) EX_rA < (sgn_dword_t) EX_imm;
        }
            break;
        case XORI: {
            EX_MEM_ALU_output = EX_rA ^ EX_imm;
        }
            break;
        case ORI: {
            EX_MEM_ALU_output = EX_rA | EX_imm;
        }
            break;
        case ANDI: {
            EX_MEM_ALU_output = EX_rA & EX_imm;
        }
            break;
        case SB: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case SH: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case SW: {
            EX_MEM_ALU_output = EX_rA+EX_imm;
        }
            break;
        case SLLI: {
            EX_MEM_ALU_output = EX_rA << EX_imm;
        }
            break;
        case SRLI: {
            EX_MEM_ALU_output = EX_rA >> EX_imm;
        }
            break;
        case SRAI: {
            EX_MEM_ALU_output = ((sgn_dword_t) EX_rA) >> EX_imm;
        }
            break;
        case ADD: {
            EX_MEM_ALU_output = EX_rA+EX_rB;
        }
            break;
        case SUB: {
            EX_MEM_ALU_output = EX_rA-EX_rB;
        }
            break;
        case SLT: {
            EX_MEM_ALU_output = (sgn_dword_t) EX_rA < (sgn_dword_t) EX_rB;
        }
            break;
        case SLTU: {
            EX_MEM_ALU_output = EX_rA < EX_rB;
        }
            break;
        case XOR: {
            EX_MEM_ALU_output = EX_rA ^ EX_rB;
        }
            break;
        case OR: {
            EX_MEM_ALU_output = EX_rA | EX_rB;

        }
            break;
        case AND: {
            EX_MEM_ALU_output = EX_rA & EX_rB;

        }
            break;
        case SLL: {
            EX_MEM_ALU_output = EX_rA << EX_rB;

        }
            break;
        case SRL: {
            EX_MEM_ALU_output = EX_rA >> EX_rB;
        }
            break;
        case SRA: {
            EX_MEM_ALU_output = ((sgn_dword_t) EX_rA) >> EX_rB;
        }
            break;
        default:
            throw std::logic_error("WRONG OP!");
    }
    if (branch_table[EX_op] && EX_op != END_I) {
        pre.set(EX_pc, hit);
        if (hit ^ EX_is_jmp_bit) {
            // wrong
            clear_stat = CLEAR_IF_EX;
            pc = hit ? EX_pc+EX_imm : EX_pc+4; // EX must called after ID to avoid two b<cond.>s
        }
    }
    EX_MEM_op = EX_op;
    EX_MEM_rd = EX_rd;
    EX_MEM_rB = EX_rB;
    if (EX_rd && !access_mem_table[EX_op])
        EX_forward.set(EX_MEM_rd, EX_MEM_ALU_output);
}


void cpu::MEM_stage() {
    using namespace inst;
    if (MEM_ACCESS_counter >= 1) {
        --MEM_ACCESS_counter;
        if (MEM_ACCESS_counter) {
            // this cycle is 2->1 cycle, so skipped
            MEM_WB_op = inst::NOP;
            return;
        } else {
            // restore registers from backup
            MEM_WB_op = MEM_INNER_op;
            MEM_WB_rd = MEM_INNER_rd;
            MEM_value = MEM_INNER_loc;
            switch (MEM_WB_op) {
                case LB: {
                    MEM_WB_output = mem->read8s(MEM_value);
                }
                    break;
                case LH: {
                    MEM_WB_output = mem->read16s(MEM_value);
                }
                    break;
                case LW: {
                    MEM_WB_output = mem->read32s(MEM_value);
                }
                    break;
                case LBU: {
                    MEM_WB_output = mem->read8(MEM_value);
                }
                    break;
                case LHU: {
                    MEM_WB_output = mem->read16(MEM_value);
                }
                    break;
                case SB: {
                    mem->write8(MEM_value, MEM_INNER_rB & 0xff);
                }
                    break;
                case SH: {
                    mem->write16(MEM_value, MEM_INNER_rB & 0xffff);
                }
                    break;
                case SW: {
                    mem->write32(MEM_value, MEM_INNER_rB);
                }
                    break;
                default:
                    throw std::logic_error("?");
            }
            if (MEM_WB_rd)
                MEM_forward.set(MEM_WB_rd, MEM_WB_output);
        }
    } else {
        if (MEM_op == HUG)return;
        else if (MEM_op == NOP) {
            MEM_WB_op = NOP;
            return;
        }
        // preset
        MEM_WB_op = MEM_op;
        MEM_WB_rd = MEM_rd;
        MEM_WB_output = MEM_value;
        switch (MEM_op) {
            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU:
            case SB:
            case SH:
            case SW:
                // left 2 cycle
                MEM_INNER_rB = MEM_rB;
                MEM_INNER_rd = MEM_rd;
                MEM_INNER_op = MEM_op;

                assert(MEM_value <= 0x20000);
                MEM_INNER_loc = MEM_value;
                MEM_ACCESS_counter = 2;
                stall_request(STALL_EX, MEM_WAIT, 2);
                MEM_WB_op = inst::NOP;
                break;
            default:
                MEM_forward.set(MEM_rd, MEM_value);
        }
    }
}


void cpu::WB_stage() {
    using namespace inst;
    if (WB_op == NOP || WB_op == HUG)return;
    switch (WB_op) {
        case inst::END_I:
            timeout = 0;
            break;
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
            assert(WB_rd < 32);
            if (WB_rd != 0)
                registers[WB_rd] = WB_reg;
            break;
        default:
            break;
    }
}


cpu::cpu(
        struct memory* mem_ptr) {
    mem = mem_ptr;
    pc = 0;
    memset(registers, -1, sizeof(registers));
    registers[0] = 0;
}


dword_t cpu::multiplexer(dword_t reg_name, dword_t value, cpu::forwarding EX_EX, cpu::forwarding MEM_EX) {
//    assert(EX_EX.reg_name == 0 || EX_EX.reg_name != MEM_EX.reg_name);
    return reg_name == 0 ? 0 : EX_EX.reg_name == reg_name ? EX_EX.value : (MEM_EX.reg_name == reg_name
                                                                           ? MEM_EX.value
                                                                           : value);
}
