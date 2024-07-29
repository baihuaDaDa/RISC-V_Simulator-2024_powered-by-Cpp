#include "Decoder.hpp"

namespace riscv {
    
    Decoder::Decoder() = default;

    void Decoder::flush() {}

    void Decoder::execute(ui ir, ReorderBuffer &rob, RegisterFile &regFile) {
        if (ir == 0x0ff00513) {
            func_exit();
            return;
        }
        switch (ir & 1111111) {
            case 0110111:
                func_lui(ir, rob, regFile);
                break;
            case 0010111:
                func_auipc();
                break;
            case 1101111:
                func_jal();
                break;
            case 1100111:
                func_jalr();
                break;
            case 1100011:
                func_branch();
                break;
            case 0000011:
                func_load();
                break;
            case 0100011:
                func_store();
                break;
            case 0010011:
                func_calc_imm();
                break;
            case 0110011:
                func_calc();
                break;
        }
    }

    void Decoder::func_lui(ui ir, ReorderBuffer &rob, RegisterFile &regFile) {
        toRoB_next
    }

} // riscv