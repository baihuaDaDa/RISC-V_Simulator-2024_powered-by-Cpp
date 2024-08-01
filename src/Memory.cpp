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

    ui Memory::load_instruction(ui pc) {
        ui ret = 0;
        for (int i = 3; i >=0; --i) {
            if (memory.count(pc + i) == 0) ret <<= 8;
            else ret = (ret << 8) + memory[pc + i];
        }
        return ret;
    }

    void Memory::load_byte(LB2Mem &fromLSB) {
        if (memory.count(fromLSB.addr) == 0) result_next = {fromLSB.robId, 0, true};
        else result_next = {fromLSB.robId, sext(memory[fromLSB.addr], 7), true};
    }

    void Memory::load_half(LB2Mem &fromLSB) {
        ui ret = (memory.count(fromLSB.addr) == 0 ? 0 : memory[fromLSB.addr])
                + ((memory.count(fromLSB.addr + 1) == 0 ? 0 : memory[fromLSB.addr + 1]) << 8);
        result_next = {fromLSB.robId, sext(ret, 15), true};
    }

    void Memory::load_word(LB2Mem &fromLSB) {
        ui ret = 0;
        for (int i = 3; i >=0; --i) {
            if (memory.count(fromLSB.addr + i) == 0) ret <<= 8;
            else ret = (ret << 8) + memory[fromLSB.addr + i];
        }
        result_next = {fromLSB.robId, ret, true};
    }

    void Memory::load_byte_unsigned(LB2Mem &fromLSB) {
        result_next = {fromLSB.robId, ui(memory[fromLSB.addr]), true};
    }

    void Memory::load_half_unsigned(LB2Mem &fromLSB) {
        result_next = {fromLSB.robId, ui(memory[fromLSB.addr] + (memory[fromLSB.addr + 1] << 8)), true};
    }

    void Memory::store_byte(RoB2Mem &fromRoB) {
        memory[fromRoB.addr] = (unsigned char)(fromRoB.value);
    }

    void Memory::store_half(RoB2Mem &fromRoB) {
        memory[fromRoB.addr] = fromRoB.value & 0x000000ff;
        memory[fromRoB.addr + 1] = (fromRoB.value >> 8) & 0x000000ff;
    }

    void Memory::store_word(RoB2Mem &fromRoB) {
        memory[fromRoB.addr] = fromRoB.value & 0x000000ff;
//        std::cerr << ui(memory[fromRoB.addr]) << ' ';
        memory[fromRoB.addr + 1] = (fromRoB.value >> 8) & 0x000000ff;
//        std::cerr << ui(memory[fromRoB.addr + 1]) << ' ';
        memory[fromRoB.addr + 2] = (fromRoB.value >> 16) & 0x000000ff;
//        std::cerr << ui(memory[fromRoB.addr + 2]) << ' ';
        memory[fromRoB.addr + 3] = (fromRoB.value >> 24) & 0x000000ff;
//        std::cerr << ui(memory[fromRoB.addr + 3]) << std::endl;
    }

    void Memory::execute(RoB2Mem &fromRoB, LB2Mem &fromLSB, bool isFlush) {
//        if (memory.count(0x106c) != 0)
//            std::cerr << "Memory[0x106c]: " << memory[0x106c] << std::endl;
        if (isFlush) {
            flush();
            return;
        }
        if (!busy && (fromLSB.ready || fromRoB.ready)) {
            fromRoB_tmp = fromRoB;
            fromLSB_tmp = fromLSB;
            busy = true;
        }
        if (busy) {
            if (clock < 3) {
                ++clock;
                return;
            }
            if (clock == 3) {
                if (fromLSB_tmp.ready) {
                    switch (fromLSB_tmp.loadType) {
                        case LOAD_BYTE: load_byte(fromLSB_tmp); break;
                        case LOAD_HALF: load_half(fromLSB_tmp); break;
                        case LOAD_WORD: load_word(fromLSB_tmp); break;
                        case LOAD_BYTE_UNSIGNED: load_byte_unsigned(fromLSB_tmp); break;
                        case LOAD_HALF_UNSIGNED: load_half_unsigned(fromLSB_tmp); break;
                    }
                }
                if (fromRoB_tmp.ready) {
                    switch (fromRoB_tmp.storeType) {
                        case STORE_BYTE: store_byte(fromRoB_tmp); break;
                        case STORE_HALF: store_half(fromRoB_tmp); break;
                        case STORE_WORD: store_word(fromRoB_tmp); break;
                    }
                }
                busy = false;
                clock = 0;
            }
        }
    }

    void Memory::next() {
        result = result_next;
        result_next.ready = false;
    }

    void Memory::flush() {
        if (fromLSB_tmp.ready && !fromRoB_tmp.ready) {
            clock = 0;
            busy = false;
        }
    }

    bool Memory::is_busy(RoB2Mem &fromRoB, LB2Mem &fromLSB) const {
        return busy || fromRoB.ready || fromLSB.ready;
    }

}