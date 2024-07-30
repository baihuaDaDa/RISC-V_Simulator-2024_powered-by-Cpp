#ifndef RISC_V_SIMULATE_ALU_HPP
#define RISC_V_SIMULATE_ALU_HPP

#include "CommonDataBus.hpp"

namespace riscv {

    class ALU {
    public:
        ALUResult result;
    private:
        ALUResult result_next;

    public:
        ALU();

        void execute(RS2ALU &fromRS);

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATE_ALU_HPP
