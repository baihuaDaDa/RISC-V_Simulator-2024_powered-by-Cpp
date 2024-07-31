#include "CPU.hpp"

namespace riscv {

    CPU::CPU() = default;

    CPU::~CPU() {
        delete mem;
    }

    void CPU::run() {
        ++clock;
        execute();
        next();
    }

    void CPU::execute() {
        cu.execute(*mem, rob.isFlush, rob.toCU, true);
        dec.execute(cu.toDec, rob, regFile, rs, lsb, alu.result, mem->result, lsb.toRoB, rob.isFlush);
        rob.execute(dec.toRoB, alu.result, mem->result, lsb.toRoB, rob.isFlush);
        rs.execute(dec.toRS, alu.result, mem->result, rob.isFlush);
        alu.execute(rs.toALU);
        lsb.execute(dec.toLSB, alu.result, mem->result, rob.toSB, mem->busy, rob.isFlush);
        mem->execute(rob.toMem, lsb.toMem, rob.isFlush);
        regFile.execute(rob.toReg, rob.toRegSta);
    }

    void CPU::next() {
        cu.next();
        dec.next();
        rob.next();
        rs.next();
        alu.next();
        lsb.next();
        mem->next();
        regFile.next();
    }

} // riscv