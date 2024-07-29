#include "Decoder.hpp"

namespace riscv {
    
    Decoder::Decoder() {}

    void Decoder::flush() {}

    void Decoder::execute(ui ir) {
        if (ir == 0x0ff00513) {
            func_exit();
            return;
        }
        switch (ir & 1111111) {
            case 0110111:
                func_lui();
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

    void Decoder::func_lui() {
        if ()
    }

    void Decoder::func_auipc() {
    }

    void Decoder::func_jal() {}

    void Decoder::func_jalr() {}

    void Decoder::func_branch() {}

    void Decoder::func_load() {}

    void Decoder::func_store() {}

    void Decoder::func_calc() {}

    void Decoder::func_calc_imm() {}

    void Decoder::func_exit() {}

} // riscv