#ifndef RISC_V_SIMULATOR_MEMORY_HPP
#define RISC_V_SIMULATOR_MEMORY_HPP

#include "CommonDataBus.hpp"
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>

namespace riscv {
    class Memory {
    public:
        MemResult result;
        bool busy = false;
    private:
        ui clock = 0;
        std::unordered_map<ui, unsigned char> memory;
        MemResult result_next;
        RoB2Mem fromRoB_tmp;
        LB2Mem fromLSB_tmp;

    private:
        void load_byte(LB2Mem &fromLSB);

        void load_half(LB2Mem &fromLSB);

        void load_word(LB2Mem &fromLSB);

        void load_byte_unsigned(LB2Mem &fromLSB);

        void load_half_unsigned(LB2Mem &fromLSB);

        void store_byte(RoB2Mem &fromRoB);

        void store_half(RoB2Mem &fromRoB);

        void store_word(RoB2Mem &fromRoB);

    public:
        Memory();

        void store_program(std::istream &is);

        ui load_instruction(ui pc);

        void execute(RoB2Mem &fromRoB, LB2Mem &fromLSB, bool isFlush);

        void next();

        void flush();

        bool is_busy(RoB2Mem &fromRoB, LB2Mem &fromLSB) const;

    };
}


#endif //RISC_V_SIMULATOR_MEMORY_HPP
