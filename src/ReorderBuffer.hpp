#ifndef RISC_V_SIMULATOR_REORDERBUFFER_HPP
#define RISC_V_SIMULATOR_REORDERBUFFER_HPP

#include <iomanip>
#include "../utility/LoopQueue.hpp"
#include "CommonDataBus.hpp"
#include "RegisterFile.hpp"

namespace riscv {

    struct RoBEntry {
        RoBType robType;
        ui dest, value, instrAddr, jumpAddr;
        RoBState state;
        bool isJump;

        RoBEntry() : robType(Rob_EXIT), dest(0), value(0), instrAddr(0), jumpAddr(0), state(COMMIT) {}

        RoBEntry(const Decoder2RoB &toRoB) : robType(toRoB.robType), dest(toRoB.dest),
                                                              value(toRoB.value), instrAddr(toRoB.instrAddr),
                                                              jumpAddr(toRoB.jumpAddr), state(toRoB.state),
                                                              isJump(toRoB.isJump) {}
    };

    struct FindResult {
        ui value;
        bool found;
    };

    class ReorderBuffer {
    private:
        static constexpr ui kBufferCapBin = 6;
        static constexpr ui kBufferSize = (1 << kBufferCapBin) - 1; // 循环队列capacity比实际空间少1
    public:
        LoopQueue<RoBEntry, kBufferCapBin> buffer;
        RoB2Reg toReg;
        RoB2Mem toMem;
        RoB2SB toSB;
        RoB2CU toCU;
        bool isFlush = false;
        bool exit = false;
        ui commitCnt = 0;
    private:
        LoopQueue<RoBEntry, kBufferCapBin> buffer_next;
        RoB2Reg toReg_next;
        RoB2Mem toMem_next;
        RoB2SB toSB_next;
        RoB2CU toCU_next;
        bool isFlush_next = false;

    private:
        void add(Decoder2RoB &toRoB);

        void write_result_reg(ui value, ui robId);

        void write_result_mem(ui dest, ui value, ui robId);

    public:
        ReorderBuffer();

        bool full(Decoder2RoB &toRoB) const;

        ui next_rob_id(Decoder2RoB &toRoB) const;

        FindResult find_value(ui robId, Decoder2RoB &fromDec, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) const;

        void execute(Decoder2RoB &toRoB, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool memBusy, RegisterFile &regFile);

        void next();

        void flush();

        void print(RegisterFile &regFile) {
            for (int i = 0; i < 32; i++) {
                std::cerr << "x" << i << "=" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << regFile.load_reg(i, toReg)
                           << std::endl;
            }
        }

    };

} // riscv

#endif //RISC_V_SIMULATOR_REORDERBUFFER_HPP
