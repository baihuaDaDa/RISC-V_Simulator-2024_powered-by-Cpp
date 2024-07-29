#ifndef RISC_V_SIMULATE_BUS_HPP
#define RISC_V_SIMULATE_BUS_HPP

#include "utils.hpp"

namespace riscv {

    enum Operation {
        lui,
        auipc,
        jal,
        jalr,
        beq,
        bne,
        blt,
        bge,
        bltu,
        bgeu,
        lb,
        lh,
        lw,
        lbu,
        lhu,
        sb,
        sh,
        sw,
        addi,
        slti,
        sltiu,
        xori,
        ori,
        andi,
        slli,
        srli,
        srai,
        add,
        sub,
        sll,
        slt,
        sltu,
        xor_,
        srl,
        sra,
        or_,
        and_
    };

    enum Opcode {
        LUI,
        AUIPC,
        JAL,
        JALR,
        BRANCH,
        LOAD,
        STORE,
        CALC,
        CALC_IMM,
        EXIT
    };

    enum CalcType {
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND
    };

    enum RoBType {
        RoB_REG,
        RoB_MEM,
        RoB_JUMP,
        Rob_EXIT
    };

    struct Decoder2RoB {

    };

    struct Decoder2RS {

    };

    struct Decoder2LSB {

    };

    struct RS2ALU {
        ui calcCode : 3;
        ui opr1, opr2;

    };

    struct ALU2RoB {

    };

}

#endif //RISC_V_SIMULATE_BUS_HPP
