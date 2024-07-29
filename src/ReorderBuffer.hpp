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
        ui dest, value;
        RoBState state;

        RoBEntry() : robType(Rob_EXIT), dest(0), value(0), state(COMMIT) {}

        RoBEntry(const Decoder2RoB &toRoB, RoBState state_) : robType(toRoB.robType), dest(toRoB.dest),
                                                              value(toRoB.value), state(state_) {}
    };

    class ReorderBuffer {
    public:
        LoopQueue<RoBEntry> buffer;
        RoB2Reg toReg;
        RoB2RegStatus toRegSta;
        RoB2Mem toMem;
    private:
        LoopQueue<RoBEntry> buffer_next;
        RoB2Reg toReg_next;
        RoB2RegStatus toRegSta_next;
        RoB2Mem toMem_next;

    private:
        void add(Decoder2RoB &toRoB);

    public:
        ReorderBuffer();

        bool full() const;

        void execute(Decoder2RoB &toRoB);

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATOR_REORDERBUFFER_HPP
