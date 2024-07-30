#include "Decoder.hpp"

namespace riscv {
    
    Decoder::Decoder() = default;

    void Decoder::flush() {
        toRoB = toRoB_next;
        toRoB_next.ready = false;
        toRS = toRS_next;
        toRS_next.ready = false;
        toLSB = toLSB_next;
        toLSB_next.ready = false;
    }

    void Decoder::execute(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile) {
        switch (toDecoder.op) {
            case LUI: func_lui(pc, toDecoder, rob, regFile); break;
            case AUIPC: func_auipc(pc, toDecoder, rob, regFile); break;
            case JAL: func_jal(pc, toDecoder, rob, regFile); break;
            case JALR: func_jalr(pc, toDecoder, rob, regFile); break;
            case BEQ:
            case BNE:
            case BLT:
            case BGE:
            case BLTU:
            case BGEU: func_branch(pc, toDecoder, rob, regFile); break;
            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU: func_load(pc, toDecoder, rob, regFile); break;
            case SB:
            case SH:
            case SW: func_store(pc, toDecoder, rob, regFile); break;
            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:
            case SLLI:
            case SRLI:
            case SRAI: func_calc_imm(pc, toDecoder, rob, regFile); break;
            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND: func_calc(pc, toDecoder, rob, regFile); break;
            case EXIT: func_exit(pc, toDecoder, rob, regFile);
        }
    }

    void Decoder::func_lui(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile) {
        toRoB_next = {}
    }

} // riscv