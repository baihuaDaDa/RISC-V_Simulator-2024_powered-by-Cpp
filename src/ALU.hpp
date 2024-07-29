#ifndef RISC_V_SIMULATE_ALU_HPP
#define RISC_V_SIMULATE_ALU_HPP

#include "CommonDataBus.hpp"
#include "ControlUnit.hpp"

namespace riscv {

    class ALU {
    public:
        ALU();

        void execute(ui ir, RS2ALU &input);

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATE_ALU_HPP
