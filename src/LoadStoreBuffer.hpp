#ifndef RISC_V_SIMULATOR_LOADSTOREBUFFER_HPP
#define RISC_V_SIMULATOR_LOADSTOREBUFFER_HPP

#include <array>
#include "../utility/LoopQueue.hpp"
#include "CommonDataBus.hpp"

namespace riscv {

    struct LBEntry {
        LoadType loadType;
        int Qj, Qk;
        ui Vj, Vk, value;
        ui robId, age;
        bool busy = false;

        LBEntry() = default;

        LBEntry(const Decoder2LSB &toLSB, bool busy_) : Qj(toLSB.Qj), Qk(toLSB.Qk), Vj(toLSB.Vj), Vk(toLSB.Vk),
                                            robId(toLSB.robId), age(toLSB.age), value(0), busy(busy_) {
            switch (toLSB.memType) {
                case MEM_LB: loadType = LOAD_BYTE; break;
                case MEM_LH: loadType = LOAD_HALF; break;
                case MEM_LW: loadType = LOAD_WORD; break;
                case MEM_LBU: loadType = LOAD_BYTE_UNSIGNED; break;
                case MEM_LHU: loadType = LOAD_HALF_UNSIGNED; break;
                default: throw "memType wrong.";
            }
        }
    };

    struct SBEntry {
        StoreType storeType;
        int Qj, Qk;
        ui Vj, Vk, imm;
        ui robId, age;

        SBEntry() = default;

        SBEntry(const Decoder2LSB &toLSB) : Qj(toLSB.Qj), Qk(toLSB.Qk), Vj(toLSB.Vj), Vk(toLSB.Vk), imm(toLSB.imm),
                                            robId(toLSB.robId), age(toLSB.age) {
            switch (toLSB.memType) {
                case MEM_SB: storeType = STORE_BYTE; break;
                case MEM_SH: storeType = STORE_HALF; break;
                case MEM_SW: storeType = STORE_WORD; break;
                default: throw "memType wrong.";
            }
        }
    };

    class LoadStoreBuffer {
    private:
        static constexpr ui kBufferCapBin = 2;
        static constexpr ui kBufferSize = (1 << kBufferCapBin) - 1;
        LoopQueue<SBEntry, kBufferCapBin> storeBuffer_next;
        std::array<LBEntry, kBufferSize> loadBuffer_next;
        LB2Mem toMem_next;
        SB2RoB toRoB_next;

    public:
        LoopQueue<SBEntry, kBufferCapBin> storeBuffer;
        std::array<LBEntry, kBufferSize> loadBuffer;
        LB2Mem toMem;
        SB2RoB toRoB;

    private:
        void update_dependency(ui value, ui robId);

        void add(Decoder2LSB &fromDec);

    public:
        LoadStoreBuffer();

        void execute(Decoder2LSB &fromDec, ALUResult &fromALU, MemResult &fromMem, RoB2SB &fromRoB, bool memBusy, bool isFlush);

        void next();

        void flush();

        bool lb_full(Decoder2LSB &toLSB) const;

        bool sb_full(Decoder2LSB &fromDec, RoB2SB &fromRoB) const;

    };

} // riscv

#endif //RISC_V_SIMULATOR_LOADSTOREBUFFER_HPP
