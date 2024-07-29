#include "ALU.hpp"

namespace riscv {

    ALU::ALU() = default;

    void ALU::execute(RS2ALU &input) {
        switch (input.calcType) {
            case ADD: result_next = input.opr1 + input.opr2; break;
            case SUB: result_next = input.opr1 - input.opr2; break;
            case SLL: result_next = input.opr1 << (input.opr2 & 0b11111); break;
            case SLT: result_next = static_cast<signed int>(input.opr1) < static_cast<signed int>(input.opr2); break;
            case SLTU: result_next = input.opr1 < input.opr2; break;
            case XOR: result_next = input.opr1 ^ input.opr2; break;
            case SRL: result_next = input.opr1 >> (input.opr2 & 0b11111); break;
            case SRA: result_next = static_cast<signed int>(input.opr1) >> (input.opr2 & 0b11111); break;
            case OR: result_next = input.opr1 | input.opr2; break;
            case AND: result_next = input.opr1 & input.opr2; break;
        }
    }

    void ALU::flush() {
        result = result_next;
    }

} // riscv