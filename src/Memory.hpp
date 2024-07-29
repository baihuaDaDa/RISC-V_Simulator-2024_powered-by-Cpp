#ifndef RISC_V_SIMULATOR_MEMORY_HPP
#define RISC_V_SIMULATOR_MEMORY_HPP

#include "CommonDataBus.hpp"
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>

namespace riscv {
    class Memory {
    private:
        std::unordered_map<ui, unsigned char> memory;

    public:
        Memory();

        void load_program(std::istream &is);

        ui load_byte(ui addr);

        ui load_half(ui addr);

        ui load_word(ui addr);

        ui load_byte_unsigned(ui addr);

        ui load_half_unsigned(ui addr);

        void store_byte(ui addr, ui operand);

        void store_half(ui addr, ui operand);

        void store_word(ui addr, ui operand);
    };
}


#endif //RISC_V_SIMULATOR_MEMORY_HPP
