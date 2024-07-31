#include "ALU.hpp"

namespace riscv {

    ALU::ALU() = default;

    void ALU::execute(RS2ALU &fromRS) {
        if (fromRS.ready) {
            switch (fromRS.calcType) {
                case CALC_ADD: result_next.value = fromRS.opr1 + fromRS.opr2; break;
                case CALC_SUB: result_next.value = fromRS.opr1 - fromRS.opr2; break;
                case CALC_SLL: result_next.value = fromRS.opr1 << (fromRS.opr2 & 0b11111); break;
                case CALC_XOR: result_next.value = fromRS.opr1 ^ fromRS.opr2; break;
                case CALC_SRL: result_next.value = fromRS.opr1 >> (fromRS.opr2 & 0b11111); break;
                case CALC_SRA: result_next.value = static_cast<signed int>(fromRS.opr1) >> (fromRS.opr2 & 0b11111); break;
                case CALC_OR: result_next.value = fromRS.opr1 | fromRS.opr2; break;
                case CALC_AND: result_next.value = fromRS.opr1 & fromRS.opr2; break;
                case CALC_SEQ: result_next.value = fromRS.opr1 == fromRS.opr2; break;
                case CALC_SNE: result_next.value = fromRS.opr1 != fromRS.opr2; break;
                case CALC_SLT: result_next.value = static_cast<signed int>(fromRS.opr1) < static_cast<signed int>(fromRS.opr2); break;
                case CALC_SLTU: result_next.value = fromRS.opr1 < fromRS.opr2; break;
                case CALC_SGE: result_next.value = static_cast<signed int>(fromRS.opr1) >= static_cast<signed int>(fromRS.opr2); break;
                case CALC_SGEU: result_next.value = fromRS.opr1 >= fromRS.opr2; break;
            }
            result_next.robId = fromRS.robId;
            result_next.ready = true;
        }
    }

    void ALU::next() {
        result = result_next;
        result_next.ready = false;
    }

} // riscv