#include "Memory.hpp"

namespace riscv {

    Memory::Memory() = default;

    void Memory::load_program(std::istream &is) {
        std::string token;
        ui lc = 0;
        while (std::getline(is, token)) {
            if (token.empty()) continue;
            if (token[0] == '@') {
                lc = std::stoi(token.substr(1), nullptr, 16);
                continue;
            }
            std::istringstream iss(token);
            iss >> std::hex;
            ui byte;
            while (iss >> byte)
                memory[lc++] = (unsigned char)(byte);
        }
    }

    ui Memory::load_byte(ui addr) {
        return sext(memory[addr], 7);
    }

    ui Memory::load_half(ui addr) {
        ui ret = memory[addr] + (memory[addr + 1] << 8);
        return sext(ret, 15);
    }

    ui Memory::load_word(ui addr) {
        ui ret = 0;
        for (int i = 3; i >=0; --i)
            ret = (ret << 8) + memory[addr + i];
        return ret;
    }

    ui Memory::load_byte_unsigned(ui addr) {
        return memory[addr];
    }

    ui Memory::load_half_unsigned(ui addr) {
        return memory[addr] + (memory[addr + 1] << 8);
    }

    void Memory::store_byte(ui addr, ui operand) {
        memory[addr] = (unsigned char)(operand);
    }

    void Memory::store_half(ui addr, ui operand) {
        memory[addr] = operand & 0x000000ff;
        memory[addr + 1] = operand & 0x0000ff00;
    }

    void Memory::store_word(ui addr, ui operand) {
        memory[addr] = operand & 0x000000ff;
        memory[addr + 1] = operand & 0x0000ff00;
        memory[addr + 2] = operand & 0x00ff0000;
        memory[addr + 3] = operand & 0xff000000;
    }

}