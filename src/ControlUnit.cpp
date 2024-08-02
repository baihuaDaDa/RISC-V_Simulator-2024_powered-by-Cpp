#include "ControlUnit.hpp"

namespace riscv {
    void ControlUnit::execute(Memory &mem, bool isFlush, RoB2CU &fromRoB, bool isJump, bool isFull) {
        if (isFlush) {
            pc_next = fromRoB.jumpAddr;
            exit = false;
            return;
        }
        if (exit) return;
        if (isFull) return;
        ir = mem.load_instruction(pc);
        toDec.rd = (ir >> 7) & 0b11111;
        toDec.rs1 = (ir >> 15) & 0b11111;
        toDec.rs2 = (ir >> 20) & 0b11111;
        toDec.instrAddr = pc;
        toDec.isJump = isJump;
        // decode: jumpAddr & sub-op
        switch (ir & 0b1111111) {
            case 0b0110111: // lui
                toDec.imm = ir & kImm_31_12;
                toDec.op = LUI;
                break;
            case 0b0010111: // auipc
                toDec.imm = ir & kImm_31_12;
                toDec.op = AUIPC;
                break;
            case 0b1101111: // jal
                toDec.imm = sext(((ir & kImm_31) >> 11) + ((ir & kImm_30_21) >> 20) + ((ir & kImm_20) >> 9) + (ir & kImm_19_12), 20);
                toDec.op = JAL;
                break;
            case 0b1100111: // jalr
                toDec.imm = sext((ir & kImm_31_20) >> 20, 11);
                toDec.op = JALR;
                break;
            case 0b1100011: // branch
                toDec.imm = sext(((ir & kImm_31) >> 19) + ((ir & kImm_30_25) >> 20) + ((ir & kImm_11_8) >> 7) + ((ir & kImm_7) << 4), 12);
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec.op = BEQ; break;
                    case 0b001: toDec.op = BNE; break;
                    case 0b100: toDec.op = BLT; break;
                    case 0b101: toDec.op = BGE; break;
                    case 0b110: toDec.op = BLTU; break;
                    case 0b111: toDec.op = BGEU; break;
                }
                break;
            case 0b0000011: // load
                toDec.imm = sext((ir & kImm_31_20) >> 20, 11);
                toDec.age = memInstrCnt++;
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec.op = LB; break;
                    case 0b001: toDec.op = LH; break;
                    case 0b010: toDec.op = LW; break;
                    case 0b100: toDec.op = LBU; break;
                    case 0b101: toDec.op = LHU; break;
                }
                break;
            case 0b0100011: // store
                toDec.imm = sext(((ir & kImm_31_25) >> 20) + ((ir & kImm_11_7) >> 7), 11);
                toDec.age = memInstrCnt++;
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec.op = SB; break;
                    case 0b001: toDec.op = SH; break;
                    case 0b010: toDec.op = SW; break;
                }
                break;
            case 0b0010011: // calc_imm
                toDec.imm = sext((ir & kImm_31_20) >> 20, 11);
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec.op = ADDI; break;
                    case 0b010: toDec.op = SLTI; break;
                    case 0b011: toDec.op = SLTIU; break;
                    case 0b100: toDec.op = XORI; break;
                    case 0b110: toDec.op = ORI; break;
                    case 0b111: toDec.op = ANDI; break;
                    case 0b001: toDec.op = SLLI; break;
                    case 0b101:
                        switch ((ir & kImm_31_25) >> 25) {
                            case 0b0000000: toDec.op = SRLI; break;
                            case 0b0100000: toDec.op = SRAI; break;
                        }
                        break;
                }
                break;
            case 0b0110011: // calc
                /* no immediate */
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000:
                        switch ((ir & kImm_31_25) >> 25) {
                            case 0b0000000: toDec.op = ADD; break;
                            case 0b0100000: toDec.op = SUB; break;
                        }
                        break;
                    case 0b001: toDec.op = SLL; break;
                    case 0b010: toDec.op = SLT; break;
                    case 0b011: toDec.op = SLTU; break;
                    case 0b100: toDec.op = XOR; break;
                    case 0b101:
                        switch ((ir & kImm_31_25) >> 25) {
                            case 0b0000000: toDec.op = SRL; break;
                            case 0b0100000: toDec.op = SRA; break;
                        }
                        break;
                    case 0b110: toDec.op = OR; break;
                    case 0b111: toDec.op = AND; break;
                }
                break;
        }
        toDec.ready = true;
        if (ir == 0x0ff00513) {
            toDec.op = EXIT;
            exit = true;
        }
        if (toDec.op == JAL) {
            toDec.jumpAddr = pc_next = pc + toDec.imm;
        } else if (toDec.op == JALR) {
            toDec.jumpAddr = pc_next = toDec.imm + mem.load_instruction(toDec.rs1);
        } else if (toDec.op == BEQ || toDec.op == BNE || toDec.op == BLT || toDec.op == BGE
                   || toDec.op == BLTU || toDec.op == BGEU) {
            if (isJump) toDec.jumpAddr = pc_next = pc + toDec.imm;
        } else {
            pc_next = pc + 4;
        }
    }

    void ControlUnit::next() {
        pc = pc_next;
    }

    ControlUnit::ControlUnit() = default;
} // riscv