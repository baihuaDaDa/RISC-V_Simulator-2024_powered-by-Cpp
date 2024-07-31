#ifndef RISC_V_SIMULATE_REGISTERFILE_HPP
#define RISC_V_SIMULATE_REGISTERFILE_HPP

#include "CommonDataBus.hpp"

namespace riscv {

    class RegisterFile {
    private:
        ui regs_next[32]{};
        int regStatus_next[32];
    public:
        ui regs[32]{};
        int regStatus[32];

    private:
        void store_reg(ui rd, ui value, ui robId);

        void set_dependency(ui rd, ui robId);

    public:
        RegisterFile();

        ui load_reg(ui rs, RoB2Reg &toReg);

        int get_dependency(ui rs, RoB2Reg &toReg, RoB2RegStatus &toRegSta);

        void execute(RoB2Reg &toReg, RoB2RegStatus &toRegSta);

        void next();

    };

} // riscv

#endif //RISC_V_SIMULATE_REGISTERFILE_HPP
