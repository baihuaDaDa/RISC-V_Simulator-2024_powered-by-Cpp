#ifndef RISC_V_SIMULATOR_DECODER_HPP
#define RISC_V_SIMULATOR_DECODER_HPP

#include "CommonDataBus.hpp"
#include "ControlUnit.hpp"
#include "ReorderBuffer.hpp"
#include "ReservationStation.hpp"

namespace riscv {

    class Decoder {
    public:
        Decoder2RoB toRoB;
        bool isRoB;
        Decoder2RS toRS;
        bool isRS;
        Decoder2LSB toLSB;
        bool isLSB;

    private:
        void func_lui();

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

        void execute(ui ir);

    };

} // riscv

#endif //RISC_V_SIMULATOR_DECODER_HPP
