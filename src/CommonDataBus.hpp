#ifndef RISC_V_SIMULATE_BUS_HPP
#define RISC_V_SIMULATE_BUS_HPP

#include "utils.hpp"

namespace riscv {

    enum Operation {
        LUI,
        AUIPC,
        JAL,
        JALR,
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,
        LB,
        LH,
        LW,
        LBU,
        LHU,
        SB,
        SH,
        SW,
        ADDI,
        SLTI,
        SLTIU,
        XORI,
        ORI,
        ANDI,
        SLLI,
        SRLI,
        SRAI,
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND,
        EXIT
    };

    enum CalcType {
        CALC_ADD,
        CALC_SUB,
        CALC_SLL,
        CALC_XOR,
        CALC_SRL,
        CALC_SRA,
        CALC_OR,
        CALC_AND,
        CALC_SEQ,
        CALC_SNE,
        CALC_SLT,
        CALC_SLTU,
        CALC_SGE,
        CALC_SGEU
    };

    enum MemType {
        MEM_LB,
        MEM_LH,
        MEM_LW,
        MEM_LBU,
        MEM_LHU,
        MEM_SB,
        MEM_SH,
        MEM_SW
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

    // CU output
    struct CU2Decoder {
        Operation op;
        ui rs1, rs2, rd, imm, instrAddr;
        bool ready = false;
    };

    // Decoder output
    struct Decoder2RoB {
        RoBType robType;
        ui dest, value, instrAddr, jumpAddr;
        bool ready = false;
    };

    struct Decoder2RS {
        CalcType calcType;
        ui Qj, Qk, Vj, Vk;
        ui robId;
        bool ready = false;
    };

    struct Decoder2LSB {
        MemType memType;
        ui Qj, Qk, Vj, Vk, dest;
        ui robId, age;
        bool ready = false;
    };

    // RoB output
    struct RoB2Reg {
        ui rd, value, robId;
        bool ready = false;
    };

    struct RoB2RegStatus {
        ui rd, robId;
        bool ready = false;
    };

    struct RoB2Mem {
        StoreType storeType;
        ui addr, value;
        bool ready = false;
    };

    // RS output
    struct RS2ALU {
        CalcType calcType;
        ui opr1, opr2, robId;
        bool ready = false;
    };

    // LSB output
    struct LB2RoB {
        ui robId, value;
        bool ready = false;
    };

    struct SB2RoB {
        ui robId, value;
        bool ready = false;
    };

    struct LSB2RS {
        ui robId, value;
        bool ready = false;
    };

    struct LSB2Mem {
        LoadType loadType;
        ui addr, robId;
        bool ready = false;
    };

    // ALU output
    struct ALUResult {
        ui robId, value;
        bool ready = false;
    };

    // Memory output
    struct MemResult {
        ui robId, value;
        bool ready = false;
    };

}

#endif //RISC_V_SIMULATE_BUS_HPP
