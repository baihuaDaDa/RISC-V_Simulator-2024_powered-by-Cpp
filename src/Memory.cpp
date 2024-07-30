#include "Memory.hpp"

namespace riscv {

    Memory::Memory() = default;

    void Memory::store_program(std::istream &is) {
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

    void Memory::load_instruction(ui pc) {
    }

    void Memory::load_byte(LSB2Mem &fromLSB) {
        result_next = {fromLSB.robId, sext(memory[fromLSB.addr], 7), true};
    }

    void Memory::load_half(LSB2Mem &fromLSB) {
        ui ret = memory[fromLSB.addr] + (memory[fromLSB.addr + 1] << 8);
        result_next = {fromLSB.robId, sext(ret, 15), true};
    }

    void Memory::load_word(LSB2Mem &fromLSB) {
        ui ret = 0;
        for (int i = 3; i >=0; --i)
            ret = (ret << 8) + memory[fromLSB.addr + i];
        result_next = {fromLSB.robId, ret, true};
    }

    void Memory::load_byte_unsigned(LSB2Mem &fromLSB) {
        result_next = {fromLSB.robId, ui(memory[fromLSB.addr]), true};
    }

    void Memory::load_half_unsigned(LSB2Mem &fromLSB) {
        result_next = {fromLSB.robId, ui(memory[fromLSB.addr] + (memory[fromLSB.addr + 1] << 8)), true};
    }

    void Memory::store_byte(RoB2Mem &fromRoB) {
        memory[fromRoB.addr] = (unsigned char)(fromRoB.value);
    }

    void Memory::store_half(RoB2Mem &fromRoB) {
        memory[fromRoB.addr] = fromRoB.value & 0x000000ff;
        memory[fromRoB.addr + 1] = fromRoB.value & 0x0000ff00;
    }

    void Memory::store_word(RoB2Mem &fromRoB) {
        memory[fromRoB.addr] = fromRoB.value & 0x000000ff;
        memory[fromRoB.addr + 1] = fromRoB.value & 0x0000ff00;
        memory[fromRoB.addr + 2] = fromRoB.value & 0x00ff0000;
        memory[fromRoB.addr + 3] = fromRoB.value & 0xff000000;
    }

    void Memory::execute(RoB2Mem &fromRoB, LSB2Mem &fromLSB) {
        ++clock;
        if (busy) return;
        busy = true;
        if (fromLSB.ready) {
            switch (fromLSB.loadType) {
                case LOAD_BYTE: load_byte(fromLSB); break;
                case LOAD_HALF: load_half(fromLSB); break;
                case LOAD_WORD: load_word(fromLSB); break;
                case LOAD_BYTE_UNSIGNED: load_byte_unsigned(fromLSB); break;
                case LOAD_HALF_UNSIGNED: load_half_unsigned(fromLSB); break;
            }
        }
        if (fromRoB.ready) {
            switch (fromRoB.storeType) {
                case STORE_BYTE: store_byte(fromRoB); break;
                case STORE_HALF: store_half(fromRoB); break;
                case STORE_WORD: store_word(fromRoB); break;
            }
        }
    }

    void Memory::flush() {
        result = result_next;
        result_next.ready = false;
        if (clock == 3) {
            clock = 0;
            busy = false;
        }
    }

}