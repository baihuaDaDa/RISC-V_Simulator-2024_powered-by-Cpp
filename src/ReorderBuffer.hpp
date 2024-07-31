#ifndef RISC_V_SIMULATOR_REORDERBUFFER_HPP
#define RISC_V_SIMULATOR_REORDERBUFFER_HPP

#include "../utility/LoopQueue.hpp"
#include "CommonDataBus.hpp"

namespace riscv {

    enum RoBState {
        EXECUTE,
        WRITE_RESULT,
        COMMIT
    };

    struct RoBEntry {
        RoBType robType;
        ui dest, value, instrAddr, jumpAddr;
        RoBState state;

        RoBEntry() : robType(Rob_EXIT), dest(0), value(0), instrAddr(0), jumpAddr(0), state(COMMIT) {}

        RoBEntry(const Decoder2RoB &toRoB, RoBState state_) : robType(toRoB.robType), dest(toRoB.dest),
                                                              value(toRoB.value), instrAddr(toRoB.instrAddr),
                                                              jumpAddr(toRoB.jumpAddr), state(state_) {}
    };

    class ReorderBuffer {
    private:
        static constexpr ui kBufferSizeBin = 5;
    public:
        LoopQueue<RoBEntry, kBufferSizeBin> buffer;
        RoB2Reg toReg;
        RoB2RegStatus toRegSta;
        RoB2Mem toMem;
        RoB2SB toSB;
    private:
        LoopQueue<RoBEntry, kBufferSizeBin> buffer_next;
        RoB2Reg toReg_next;
        RoB2RegStatus toRegSta_next;
        RoB2Mem toMem_next;
        RoB2SB toSB_next;

    private:
        void add(Decoder2RoB &toRoB);

        void write_result(ui value, ui robId);

    public:
        ReorderBuffer();

        bool full() const;

        void execute(Decoder2RoB &toRoB, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool memBusy);

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATOR_REORDERBUFFER_HPP
