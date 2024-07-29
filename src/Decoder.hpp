#ifndef RISC_V_SIMULATOR_DECODER_HPP
#define RISC_V_SIMULATOR_DECODER_HPP

#include "ReorderBuffer.hpp"
#include "RegisterFile.hpp"

namespace riscv {

    class Decoder {
    public:
        Decoder2RoB toRoB;
        Decoder2RS toRS;
        Decoder2LSB toLSB;
    private:
        Decoder2RoB toRoB_next;
        Decoder2RS toRS_next;
        Decoder2LSB toLSB_next;

    private:
        void func_lui(ui ir, ReorderBuffer &rob, RegisterFile &regFile);

        void func_auipc();

        void func_jal();

        void func_jalr();

        void func_branch();

        void func_load();

        void func_store();

        void func_calc();

        void func_calc_imm();

        void func_exit();

    public:
        Decoder();

        void flush();

        void execute(ui ir, ReorderBuffer &rob, RegisterFile &regFile);

    };

} // riscv

#endif //RISC_V_SIMULATOR_DECODER_HPP
