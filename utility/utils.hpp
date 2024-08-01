#ifndef RISC_V_SIMULATE_UTILS_HPP
#define RISC_V_SIMULATE_UTILS_HPP

#include <iostream>

namespace riscv {
    using ui = unsigned int;
    const ui kWordLen = 4;
    constexpr ui sext(ui num, ui highest) {
        if (num & (1 << highest))
            return num + (((1 << (31 - highest)) - 1) << (highest + 1));
        return num;
    }
}

#endif //RISC_V_SIMULATE_UTILS_HPP
