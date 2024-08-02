#ifndef RISC_V_SIMULATE_PREDICTOR_HPP
#define RISC_V_SIMULATE_PREDICTOR_HPP

#include "CommonDataBus.hpp"

namespace riscv {

    // 直接连接CU（组合逻辑），与其他模块之间是时序逻辑
    class Predictor {
    private:
        static constexpr ui kPredictorSizeBin = 7;
        static constexpr ui kMod = (1 << kPredictorSizeBin) - 1;
        ui counter[1 << kPredictorSizeBin]{};

    public:
        Predictor() = default;

        void update(RoB2Predictor &toPred) {
            if (toPred.ready) {
                ui ind = toPred.instrAddr & kMod;
                if (toPred.isJump) {
                    ++counter[ind];
                    counter[ind] &= 3;
                } else {
                    if (counter[ind] == 0) counter[ind] = 3;
                    else --counter[ind];
                }
            }
        }

        bool predict(ui pc) {
            return counter[pc & kMod] > 1;
        }

    };

}

#endif //RISC_V_SIMULATE_PREDICTOR_HPP
