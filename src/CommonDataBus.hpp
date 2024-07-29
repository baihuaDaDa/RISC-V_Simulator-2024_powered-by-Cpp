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

    enum MemType {
        LB,
        LH,
        LW,
        LBU,
        LHU,
        SB,
        SH,
        SW
    };

    enum LoadType {
        LOAD_BYTE,
        LOAD_HALF,
        LOAD_WORD,
        LOAD_BYTE_UNSIGNED,
        LOAD_HALF_UNSIGNED
    };

    enum StoreType {
        STORE_BYTE,
        STORE_HALF,
        STORE_WORD
    };

    enum RoBType {
        RoB_REG,
        RoB_STORE_BYTE,
        RoB_STORE_HALF,
        RoB_STORE_WORD,
        RoB_JUMP,
        Rob_EXIT
    };

    // Decoder output
    struct Decoder2RoB {
        RoBType robType;
        ui dest, value;
        bool ready;
    };

    struct Decoder2RS {
        CalcType calcType;
        ui Qj, Qk, Vj, Vk, dest, imm;
        ui robId;
        bool ready;
    };

    struct Decoder2LSB {
        MemType memType;
        ui rs1, rs2, rd, imm;
        bool ready;
    };

    // RoB output
    struct RoB2Reg {
        ui rd, value, robId;
        bool ready;
    };

    struct RoB2RegStatus {
        ui rd, robId;
        bool ready;
    };

    struct RoB2Mem {
        StoreType storeType;
        ui addr, value;
        bool ready;
    };

    // RS output
    struct RS2ALU {
        CalcType calcType;
        ui opr1, opr2;
    };

    struct RS2LSB {

    };

    struct RS2RoB {

    };

    // LSB output
    struct LSB2RoB {

    };

    struct LSB2RS {

    };

    // ALU output
    struct ALUResult {

    };

}

#endif //RISC_V_SIMULATE_BUS_HPP
