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
        void func_lui(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_auipc(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_jal(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_jalr(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_branch(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_load(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_store(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_calc(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_calc_imm(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

        void func_exit(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

    public:
        Decoder();

        void flush();

        void execute(ui pc, CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile);

    };

} // riscv

#endif //RISC_V_SIMULATOR_DECODER_HPP
