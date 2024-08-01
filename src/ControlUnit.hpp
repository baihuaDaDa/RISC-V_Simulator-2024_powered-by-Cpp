#ifndef RISC_V_SIMULATE_CONTROLUNIT_HPP
#define RISC_V_SIMULATE_CONTROLUNIT_HPP

#include "../utility/utils.hpp"
#include "Memory.hpp"

namespace riscv {

    class ControlUnit {
    public:
        ui pc = 0x00000000;
        CU2Decoder toDec;
    private:
        static constexpr ui kSubOpcode = 0x7000;
        static constexpr ui kImm_31_12 = 0xFFFFF000;
        static constexpr ui kImm_31_20 = 0xFFF00000;
        static constexpr ui kImm_31 = 0x80000000;
        static constexpr ui kImm_30_21 = 0x7FE00000;
        static constexpr ui kImm_20 = 0x100000;
        static constexpr ui kImm_19_12 = 0xFF000;
        static constexpr ui kImm_31_25 = 0xFE000000;
        static constexpr ui kImm_30_25 = 0x7E000000;
        static constexpr ui kImm_11_7 = 0xF80;
        static constexpr ui kImm_11_8 = 0xF00;
        static constexpr ui kImm_7 = 0x80;
        ui pc_next = 0x00000000;
        ui ir = 0;
        ui memInstrCnt = 0;
        bool exit = false;

    public:
        ControlUnit();

        void next();

        void execute(Memory &mem, bool isFlush, RoB2CU &fromRoB, bool isJump, bool isFull);

    };

} // riscv

#endif //RISC_V_SIMULATE_CONTROLUNIT_HPP
