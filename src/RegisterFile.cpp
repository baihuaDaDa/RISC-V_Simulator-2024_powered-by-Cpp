#include "RegisterFile.hpp"

namespace riscv {

    RegisterFile::RegisterFile() = default;

    void RegisterFile::store_reg(ui rd, ui value, ui robId) {
        regs_next[rd] = value;
        if (regStatus[rd] == robId) regStatus_next[rd] = 0;
    }

    void RegisterFile::set_dependency(ui rd, ui robId) {
        regStatus_next[rd] = robId;
    }

    void RegisterFile::execute(RoB2Reg &toReg, RoB2RegStatus &toRegSta) {
        if (toReg.ready) {
            store_reg(toReg.rd, toReg.value, toReg.robId);
        }
        if (toRegSta.ready) {
            set_dependency(toRegSta.rd, toRegSta.robId);
        }
    }

    void RegisterFile::flush() {
        for (int i = 0; i < 32; ++i) {
            regs[i] = regs_next[i];
            regStatus[i] = regStatus_next[i];
        }
    }

} // riscv