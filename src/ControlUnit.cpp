#include "ControlUnit.hpp"

namespace riscv {
    void ControlUnit::execute(Memory &mem) {
        ir = mem.load_word(pc);
        toDec_next.rd = (ir >> 7) & 0b11111;
        toDec_next.rs1 = (ir >> 15) & 0b11111;
        toDec_next.rs2 = (ir >> 20) & 0b11111;
        toDec_next.ip = pc;
        // decode: jumpAddr & sub-op
        switch (ir & 0b1111111) {
            case 0b0110111: // lui
                toDec_next.imm = ir & kImm_31_12;
                toDec_next.op = LUI;
                break;
            case 0b0010111: // auipc
                toDec_next.imm = ir & kImm_31_12;
                toDec_next.op = AUIPC;
                break;
            case 0b1101111: // jal
                toDec_next.imm = sext(((ir & kImm_31) >> 11) + ((ir & kImm_30_21) >> 20) + ((ir & kImm_20) >> 9) + (ir & kImm_19_12), 20);
                toDec_next.op = JAL;
                break;
            case 0b1100111: // jalr
                toDec_next.imm = sext((ir & kImm_31_20), 11);
                toDec_next.op = JALR;
                break;
            case 0b1100011: // branch
                toDec_next.imm = sext(((ir & kImm_31) >> 19) + ((ir & kImm_30_25) >> 20) + ((ir & kImm_11_8) >> 7) + ((ir & kImm_7) << 4), 12);
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec_next.op = BEQ; break;
                    case 0b001: toDec_next.op = BNE; break;
                    case 0b100: toDec_next.op = BLT; break;
                    case 0b101: toDec_next.op = BGE; break;
                    case 0b110: toDec_next.op = BLTU; break;
                    case 0b111: toDec_next.op = BGEU; break;
                }
                break;
            case 0b0000011: // load
                toDec_next.imm = sext((ir & kImm_31_20) >> 20, 11);
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec_next.op = LB; break;
                    case 0b001: toDec_next.op = LH; break;
                    case 0b010: toDec_next.op = LW; break;
                    case 0b100: toDec_next.op = LBU; break;
                    case 0b101: toDec_next.op = LHU; break;
                }
                break;
            case 0b0100011: // store
                toDec_next.imm = sext(((ir & kImm_31_25) >> 20) + ((ir & kImm_11_7) >> 7), 11);
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec_next.op = SB; break;
                    case 0b001: toDec_next.op = SH; break;
                    case 0b010: toDec_next.op = SW; break;
                }
                break;
            case 0b0010011: // calc_imm
                toDec_next.imm = sext((ir & kImm_31_20) >> 20, 11);
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000: toDec_next.op = ADDI; break;
                    case 0b010: toDec_next.op = SLTI; break;
                    case 0b011: toDec_next.op = SLTIU; break;
                    case 0b100: toDec_next.op = XORI; break;
                    case 0b110: toDec_next.op = ORI; break;
                    case 0b111: toDec_next.op = ANDI; break;
                    case 0b001: toDec_next.op = SLLI; break;
                    case 0b101:
                        switch ((ir & kImm_31_25) >> 25) {
                            case 0b0000000: toDec_next.op = SRLI; break;
                            case 0b0100000: toDec_next.op = SRAI; break;
                        }
                        break;
                }
                break;
            case 0b0110011: // calc
                /* no immediate */
                switch ((ir & kSubOpcode) >> 12) {
                    case 0b000:
                        switch ((ir & kImm_31_25) >> 25) {
                            case 0b0000000: toDec_next.op = ADD; break;
                            case 0b0100000: toDec_next.op = SUB; break;
                        }
                        break;
                    case 0b001: toDec_next.op = SLL; break;
                    case 0b010: toDec_next.op = SLT; break;
                    case 0b011: toDec_next.op = SLTU; break;
                    case 0b100: toDec_next.op = XOR; break;
                    case 0b101:
                        switch ((ir & kImm_31_25) >> 25) {
                            case 0b0000000: toDec_next.op = SRL; break;
                            case 0b0100000: toDec_next.op = SRA; break;
                        }
                        break;
                    case 0b110: toDec_next.op = OR; break;
                    case 0b111: toDec_next.op = AND; break;
                }
                break;
        }
        toDec_next.ready = true;
        if (ir == 0x0ff00513) {
            toDec_next = {EXIT, 0, 0, 0, 0, true};
        }
    }

    void ControlUnit::flush() {
        pc = pc_next;
        toDec = toDec_next;
    }

    ControlUnit::ControlUnit() {
        pc_next = 0x00000000;
        ir = 0;
    }
} // riscv