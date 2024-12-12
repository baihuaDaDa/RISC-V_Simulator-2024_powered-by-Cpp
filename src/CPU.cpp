#include "CPU.hpp"

namespace riscv {

    CPU::CPU(Memory *mem_) : mem(mem_) {}

    ui CPU::run() {
        while (true) {
            if (rob.exit) {
//                std::cout << "Clock: " << clock << "; CommitCnt: " << rob.commitCnt << std::endl;
//                std::cout << "Failure rate of branch prediction: " << double(rob.failureCnt) / double(rob.branchCnt) << std::endl;
                return (std::uint8_t)(regFile.load_reg(10, rob.toReg));
            }
            ++clock;
//            std::cerr << "Clock: " << std::dec << clock << std::hex << std::endl;
            execute();
            next();
//            std::cerr << "Commit cnt: " << std::dec << rob.commitCnt << std::hex << std::endl << std::endl;
        }
    }

    void CPU::execute() {
        /* one single module */
        pred.update(rob.toPred);
//        std::cerr << "isFull: " << dec.isFull << std::endl;
        cu.execute(*mem, regFile, rob.isFlush, rob.toCU, pred.predict(cu.pc), dec.isFull);
//        std::cerr << "CU: " << cu.pc << " " << cu.toDec.op << std::endl;
        dec.execute(cu.toDec, rob, regFile, rs, lsb, alu.result, mem->result, lsb.toRoB, rob.isFlush);
        /* one single module */
//        std::cerr << "toRoB: " << dec.toRoB.robType << " " << dec.toRoB.instrAddr << " " << dec.toRoB.ready << std::endl;
        rob.execute(dec.toRoB, alu.result, mem->result, lsb.toRoB, mem->is_busy(rob.toMem, lsb.toMem));
        rs.execute(dec.toRS, alu.result, mem->result, rob.isFlush);
        alu.execute(rs.toALU, rob.isFlush);
        lsb.execute(dec.toLSB, alu.result, mem->result, rob.toSB, mem->is_busy(rob.toMem, lsb.toMem), rob.isFlush);
        mem->execute(rob.toMem, lsb.toMem, rob.isFlush);
        regFile.execute(rob.toReg, dec.toRegSta, rob.isFlush);
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