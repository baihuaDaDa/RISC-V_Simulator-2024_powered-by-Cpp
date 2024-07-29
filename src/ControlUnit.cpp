#include "ControlUnit.hpp"

namespace riscv {
    void ControlUnit::execute(Memory &mem) {
        if (!ready) return;
        ir_next = mem.load_word(pc);
        pc_next += 4;
    }

    void ControlUnit::flush() {
        pc = pc_next;
        ir = ir_next;
        ready = ready_next;
    }

    ControlUnit::ControlUnit() {
        pc_next = 0x00000000;
        ir_next = 0;
        ready_next = false;
    }
} // riscv