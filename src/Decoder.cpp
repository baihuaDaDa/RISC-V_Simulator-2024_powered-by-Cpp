#include "Decoder.hpp"

namespace riscv {
    
    Decoder::Decoder() = default;

    void Decoder::next() {
        toRoB = toRoB_next;
        toRoB_next.ready = false;
        toRS = toRS_next;
        toRS_next.ready = false;
        toLSB = toLSB_next;
        toLSB_next.ready = false;
        toRegSta = toRegSta_next;
        toRegSta_next.ready = false;
        isFull = isFull_next;
    }

    void Decoder::execute(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool isFlush) {
        if (isFlush) {
            isFull_next = false;
            return;
        }
        if (!toDecoder.ready) return;
        switch (toDecoder.op) {
            case LUI: func_lui(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case AUIPC: func_auipc(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case JAL: func_jal(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case JALR: func_jalr(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case BEQ:
            case BNE:
            case BLT:
            case BGE:
            case BLTU:
            case BGEU: func_branch(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU: func_load(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case SB:
            case SH:
            case SW: func_store(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:
            case SLLI:
            case SRLI:
            case SRAI: func_calc_imm(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND: func_calc(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB); break;
            case EXIT: func_exit(toDecoder, rob, regFile, rs, lsb, fromALU, fromMem, fromSB);
        }
    }

    void Decoder::func_lui(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (!rob.full(toRoB)) {
            toRoB_next = {RoB_REG, toDecoder.rd, toDecoder.imm, toDecoder.instrAddr, 0, WRITE_RESULT, toDecoder.isJump, true};
            toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
            isFull_next = false;
            toDecoder.ready = false;
        } else isFull_next = true;
    }

    void Decoder::func_auipc(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (!rob.full(toRoB)) {
            toRoB_next = {RoB_REG, toDecoder.rd, toDecoder.instrAddr + toDecoder.imm, toDecoder.instrAddr, 0, WRITE_RESULT, toDecoder.isJump, true};
            toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
            isFull_next = false;
            toDecoder.ready = false;
        } else isFull_next = true;
    }

    void Decoder::func_jal(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (!rob.full(toRoB)) {
            toRoB_next = {RoB_REG, toDecoder.rd, toDecoder.instrAddr + 4, toDecoder.instrAddr, toDecoder.jumpAddr, WRITE_RESULT, true, true};
            toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
            isFull_next = false;
            toDecoder.ready = false;
        } else isFull_next = true;
    }

    void Decoder::func_jalr(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (rob.full(toRoB) || rs.full(toRS)) {
            isFull_next = true;
            return;
        }
        isFull_next = false;
        toDecoder.ready = false;
        toRoB_next = {RoB_JALR, toDecoder.rd, 0, toDecoder.instrAddr, toDecoder.jumpAddr, EXECUTE, true, true};
        toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
        int regStatus = regFile.get_dependency(toDecoder.rs1, rob.toReg, toRegSta);
        toRS_next = {CALC_ADD, regStatus, -1, 0, toDecoder.imm, rob.next_rob_id(toRoB), true};
        if (regStatus == -1) toRS_next.Vj = regFile.load_reg(toDecoder.rs1, rob.toReg);
        else {
            auto result = rob.find_value(regStatus, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toRS_next.Vj = result.value;
                toRS_next.Qj = -1;
            }
        }
    }

    void Decoder::func_branch(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (rob.full(toRoB) || rs.full(toRS)) {
            isFull_next = true;
            return;
        }
        isFull_next = false;
        toDecoder.ready = false;
        int rs1Status = regFile.get_dependency(toDecoder.rs1, rob.toReg, toRegSta);
        int rs2Status = regFile.get_dependency(toDecoder.rs2, rob.toReg, toRegSta);
        toRoB_next = {RoB_BRANCH, 0, 0, toDecoder.instrAddr, toDecoder.jumpAddr, EXECUTE, toDecoder.isJump, true};
        toRS_next = {CalcType(toDecoder.op - BEQ + CALC_SEQ), rs1Status, rs2Status, 0, 0, rob.next_rob_id(toRoB), true};
        if (rs1Status == -1) toRS_next.Vj = regFile.load_reg(toDecoder.rs1, rob.toReg);
        else {
            auto result = rob.find_value(rs1Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toRS_next.Vj = result.value;
                toRS_next.Qj = -1;
            }
        }
        if (rs2Status == -1) toRS_next.Vk = regFile.load_reg(toDecoder.rs2, rob.toReg);
        else {
            auto result = rob.find_value(rs2Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toRS_next.Vk = result.value;
                toRS_next.Qk = -1;
            }
        }
    }

    void Decoder::func_load(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (rob.full(toRoB) || lsb.lb_full(toLSB)) {
            isFull_next = true;
            return;
        }
        isFull_next = false;
        toDecoder.ready = false;
        int regStatus = regFile.get_dependency(toDecoder.rs1, rob.toReg, toRegSta);
        toRoB_next = {RoB_REG, toDecoder.rd, 0, toDecoder.instrAddr, 0, EXECUTE, false, true};
        toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
        toLSB_next = {MemType(toDecoder.op - LB + MEM_LB), regStatus, -1, 0, toDecoder.imm, 0, rob.next_rob_id(toRoB), toDecoder.age, true};
        if (regStatus == -1) toLSB_next.Vj = regFile.load_reg(toDecoder.rs1, rob.toReg);
        else {
            auto result = rob.find_value(regStatus, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toLSB_next.Vj = result.value;
                toLSB_next.Qj = -1;
            }
        }
    }

    void Decoder::func_store(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (rob.full(toRoB) || lsb.sb_full(toLSB, rob.toSB)) {
            isFull_next = true;
            return;
        }
        isFull_next = false;
        toDecoder.ready = false;
        int rs1Status = regFile.get_dependency(toDecoder.rs1, rob.toReg, toRegSta);
        int rs2Status = regFile.get_dependency(toDecoder.rs2, rob.toReg, toRegSta);
        toRoB_next = {RoBType(toDecoder.op - SB + RoB_STORE_BYTE), 0, 0, toDecoder.instrAddr, 0, EXECUTE, false, true};
        toLSB_next = {MemType(toDecoder.op - SB + MEM_SB), rs1Status, rs2Status, 0, 0, toDecoder.imm, rob.next_rob_id(toRoB), toDecoder.age, true};
        if (rs1Status == -1) toLSB_next.Vj = regFile.load_reg(toDecoder.rs1, rob.toReg);
        else {
            auto result = rob.find_value(rs1Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toLSB_next.Vj = result.value;
                toLSB_next.Qj = -1;
            }
        }
        if (rs2Status == -1) toLSB_next.Vk = regFile.load_reg(toDecoder.rs2, rob.toReg);
        else {
            auto result = rob.find_value(rs2Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toLSB_next.Vk = result.value;
                toLSB_next.Qk = -1;
            }
        }
    }

    void Decoder::func_calc(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (rob.full(toRoB) || rs.full(toRS)) {
            isFull_next = true;
            return;
        }
        isFull_next = false;
        toDecoder.ready = false;
        toRoB_next = {RoB_REG, toDecoder.rd, 0, toDecoder.instrAddr, 0, EXECUTE, false, true};
        toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
        int rs1Status = regFile.get_dependency(toDecoder.rs1, rob.toReg, toRegSta);
        int rs2Status = regFile.get_dependency(toDecoder.rs2, rob.toReg, toRegSta);
        toRS_next = {op_to_calc(toDecoder.op), rs1Status, rs2Status, 0, 0, rob.next_rob_id(toRoB), true};
        if (rs1Status == -1) toRS_next.Vj = regFile.load_reg(toDecoder.rs1, rob.toReg);
        else {
            auto result = rob.find_value(rs1Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toRS_next.Vj = result.value;
                toRS_next.Qj = -1;
            }
        }
        if (rs2Status == -1) toRS_next.Vk = regFile.load_reg(toDecoder.rs2, rob.toReg);
        else {
            auto result = rob.find_value(rs2Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toRS_next.Vk = result.value;
                toRS_next.Qk = -1;
            }
        }
    }

    void Decoder::func_calc_imm(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (rob.full(toRoB) || rs.full(toRS)) {
            isFull_next = true;
            return;
        }
        isFull_next = false;
        toDecoder.ready = false;
        toRoB_next = {RoB_REG, toDecoder.rd, 0, toDecoder.instrAddr, 0, EXECUTE, false, true};
        toRegSta_next = {toDecoder.rd, rob.next_rob_id(toRoB), true};
        int rs1Status = regFile.get_dependency(toDecoder.rs1, rob.toReg, toRegSta);
        toRS_next = {op_to_calc(toDecoder.op), rs1Status, -1, 0, toDecoder.imm, rob.next_rob_id(toRoB), true};
        if (rs1Status == -1) toRS_next.Vj = regFile.load_reg(toDecoder.rs1, rob.toReg);
        else {
            auto result = rob.find_value(rs1Status, toRoB, fromALU, fromMem, fromSB);
            if (result.found) {
                toRS_next.Vj = result.value;
                toRS_next.Qj = -1;
            }
        }
    }

    void Decoder::func_exit(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) {
        if (!rob.full(toRoB)) {
            toRoB_next = {Rob_EXIT, 0, 0, toDecoder.instrAddr, 0, WRITE_RESULT, false, true};
            isFull_next = false;
            toDecoder.ready = false;
        } else isFull_next = true;
    }

    CalcType Decoder::op_to_calc(Operation op) {
        switch (op) {
            case ADD:
            case ADDI: return CALC_ADD;
            case SUB: return CALC_SUB;
            case AND:
            case ANDI: return CALC_AND;
            case OR:
            case ORI: return CALC_OR;
            case SLT:
            case SLTI: return CALC_SLT;
            case SLTU:
            case SLTIU: return CALC_SLTU;
            case SLL:
            case SLLI: return CALC_SLL;
            case SRL:
            case SRLI: return CALC_SRL;
            case SRA:
            case SRAI: return CALC_SRA;
            case XOR:
            case XORI: return CALC_XOR;
            default: throw "wrong op.";
        }
    }

} // riscv