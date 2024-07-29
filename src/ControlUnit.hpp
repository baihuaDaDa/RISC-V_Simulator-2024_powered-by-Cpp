#ifndef RISC_V_SIMULATE_CONTROLUNIT_HPP
#define RISC_V_SIMULATE_CONTROLUNIT_HPP

#include "../utility/utils.hpp"
#include "Memory.hpp"

namespace riscv {

    class ControlUnit {
    public:
        ui pc;
        ui ir;
        bool ready;
    private:
        ui pc_next;
        ui ir_next;
        bool ready_next;

    public:
        ControlUnit();

        void flush();

        void execute(Memory &mem);
    };

} // riscv

#endif //RISC_V_SIMULATE_CONTROLUNIT_HPP
