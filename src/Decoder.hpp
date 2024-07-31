#ifndef RISC_V_SIMULATOR_DECODER_HPP
#define RISC_V_SIMULATOR_DECODER_HPP

#include "ReorderBuffer.hpp"
#include "RegisterFile.hpp"
#include "ReservationStation.hpp"
#include "LoadStoreBuffer.hpp"

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
        void func_lui(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_auipc(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_jal(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_jalr(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_branch(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_load(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_store(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_calc(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_calc_imm(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        void func_exit(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB);

        static CalcType op_to_calc(Operation op);

    public:
        Decoder();

        void next();

        void execute(CU2Decoder &toDecoder, ReorderBuffer &rob, RegisterFile &regFile, ReservationStation &rs, LoadStoreBuffer &lsb, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool isFlush);

    };

} // riscv

#endif //RISC_V_SIMULATOR_DECODER_HPP
