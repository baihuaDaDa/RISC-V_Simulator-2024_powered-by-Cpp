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

    private:
        void load_byte(LSB2Mem &fromLSB);

        void load_half(LSB2Mem &fromLSB);

        void load_word(LSB2Mem &fromLSB);

        void load_byte_unsigned(LSB2Mem &fromLSB);

        void load_half_unsigned(LSB2Mem &fromLSB);

        void store_byte(RoB2Mem &fromRoB);

        void store_half(RoB2Mem &fromRoB);

        void store_word(RoB2Mem &fromRoB);

    public:
        Memory();

        void store_program(std::istream &is);

        void load_instruction(ui pc);

        void execute(RoB2Mem &fromRoB, LSB2Mem &fromLSB);

        void flush();

    };
}


#endif //RISC_V_SIMULATOR_MEMORY_HPP
