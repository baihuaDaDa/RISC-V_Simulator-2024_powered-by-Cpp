#ifndef RISC_V_SIMULATE_ALU_HPP
#define RISC_V_SIMULATE_ALU_HPP

#include "CommonDataBus.hpp"
#include "ControlUnit.hpp"

namespace riscv {

    class ALU {
    public:
        ui result;
    private:
        ui result_next;

    public:
        ALU();

        void execute(RS2ALU &input);

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATE_ALU_HPP
