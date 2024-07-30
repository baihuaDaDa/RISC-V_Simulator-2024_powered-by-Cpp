#ifndef RISC_V_SIMULATE_UTILS_HPP
#define RISC_V_SIMULATE_UTILS_HPP

namespace riscv {
    using ui = unsigned int;
    const ui kWordLen = 4;
    constexpr ui sext(ui num, ui highest) {
        if (num & (1 << highest)) return num + (((1 << (31 - highest)) - 1) << highest);
        return num;
    }
    constexpr ui upper_sext(ui num, ui lowest) {
        return num << lowest;
    }
}

#endif //RISC_V_SIMULATE_UTILS_HPP
