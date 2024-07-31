#include "RegisterFile.hpp"

namespace riscv {

    RegisterFile::RegisterFile() {
        for (int i = 0; i < 32; ++i)
            regStatus_next[i] = regStatus[i] = -1;
    }

    void RegisterFile::store_reg(ui rd, ui value, ui robId) {
        regs_next[rd] = value;
        if (regStatus[rd] == robId) regStatus_next[rd] = -1;
    }

    void RegisterFile::set_dependency(ui rd, ui robId) {
        regStatus_next[rd] = robId;
    }

    ui RegisterFile::load_reg(ui rs, RoB2Reg &toReg) {
        if (toReg.ready && toReg.rd == rs) return toReg.value;
        return regs[rs];
    }

    int RegisterFile::get_dependency(ui rs, RoB2Reg &toReg, RoB2RegStatus &toRegSta) {
        if (toReg.ready && toReg.rd == rs && regStatus[toReg.rd] == toReg.robId) return -1;
        if (toRegSta.ready && toRegSta.rd == rs) return toRegSta.robId;
        return regStatus[rs];
    }

    void RegisterFile::execute(RoB2Reg &toReg, RoB2RegStatus &toRegSta) {
        if (toReg.ready) {
            store_reg(toReg.rd, toReg.value, toReg.robId);
        }
        if (toRegSta.ready) {
            set_dependency(toRegSta.rd, toRegSta.robId);
        }
    }

    void RegisterFile::next() {
        for (int i = 0; i < 32; ++i) {
            regs[i] = regs_next[i];
            regStatus[i] = regStatus_next[i];
        }
    }

} // riscv