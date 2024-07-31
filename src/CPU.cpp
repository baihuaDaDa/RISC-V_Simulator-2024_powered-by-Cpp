#include "CPU.hpp"

namespace riscv {

    CPU::CPU(Memory *mem_) : mem(mem_) {}

    ui CPU::run() {
        while (true) {
            if (rob.exit) return regFile.load_reg(10, rob.toReg);
            ++clock;
            execute();
            next();
        }
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