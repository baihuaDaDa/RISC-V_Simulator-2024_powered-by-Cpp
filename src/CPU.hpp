#ifndef RISC_V_SIMULATE_CPU_HPP
#define RISC_V_SIMULATE_CPU_HPP

#include <cstdint>
#include "ControlUnit.hpp"
#include "Decoder.hpp"
#include "ALU.hpp"

namespace riscv {

    class CPU {
    private:
        ALU alu;
        ControlUnit cu;
        Decoder dec;
        Memory *mem = new(Memory);
        RegisterFile regFile;
        ReservationStation rs;
        ReorderBuffer rob;
        LoadStoreBuffer lsb;
        ui clock = 0;

        void execute();

        void next();

    public:
        CPU(Memory *mem_);

        ui run();

    };

} // riscv

#endif //RISC_V_SIMULATE_CPU_HPP
