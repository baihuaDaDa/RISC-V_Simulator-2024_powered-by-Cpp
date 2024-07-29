#ifndef RISC_V_SIMULATE_REGISTERFILE_HPP
#define RISC_V_SIMULATE_REGISTERFILE_HPP

#include "CommonDataBus.hpp"

namespace riscv {

    class RegisterFile {
    private:
        ui regs_next[32]{};
        ui regStatus_next[32]{};
    public:
        ui regs[32]{};
        ui regStatus[32]{};

    private:
        void store_reg(ui rd, ui value, ui robId);

        void set_dependency(ui rd, ui robId);

    public:
        RegisterFile();

        void execute(RoB2Reg &toReg, RoB2RegStatus &toRegSta);

        void flush();

    };

} // riscv

#endif //RISC_V_SIMULATE_REGISTERFILE_HPP
