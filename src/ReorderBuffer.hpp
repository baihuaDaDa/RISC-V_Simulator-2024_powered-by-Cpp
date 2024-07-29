#ifndef RISC_V_SIMULATOR_REORDERBUFFER_HPP
#define RISC_V_SIMULATOR_REORDERBUFFER_HPP

#include "../utility/LoopQueue.hpp"
#include "CommonDataBus.hpp"

namespace riscv {

    struct RoBEntry : Decoder2RoB {
        bool busy;
    };

    class ReorderBuffer {
    public:
        LoopQueue<RoBEntry> buffer;

    private:
        LoopQueue<RoBEntry> buffer_next;

    public:
        ReorderBuffer();

        bool full() const;

        void add(Decoder2RoB toRoB);

        void execute();

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATOR_REORDERBUFFER_HPP
