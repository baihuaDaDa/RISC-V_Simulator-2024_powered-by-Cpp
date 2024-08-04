#include "ReorderBuffer.hpp"

namespace riscv {

    ReorderBuffer::ReorderBuffer() = default;

    bool ReorderBuffer::full(Decoder2RoB &toRoB) const {
//        std::cerr << buffer.size() << ' ' << toRoB.ready << std::endl;
        return buffer.size() + toRoB.ready >= kBufferSize;
    }

    void ReorderBuffer::add(Decoder2RoB &toRoB) {
        if (buffer.full()) throw "RoB lb_full.";
        buffer_next.push_back(RoBEntry(toRoB));
    }

    void ReorderBuffer::write_result_reg(ui value, ui robId) {
        buffer_next.at(robId).value = value;
        buffer_next.at(robId).state = WRITE_RESULT;
    }

    void ReorderBuffer::write_result_mem(ui dest, ui value, ui robId) {
        buffer_next.at(robId).dest = dest;
        buffer_next.at(robId).value = value;
        buffer_next.at(robId).state = WRITE_RESULT;
    }

    void ReorderBuffer::execute(Decoder2RoB &toRoB, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool memBusy, RegisterFile &regFile) {
        if (isFlush) {
            flush();
            return;
        }
        if (toRoB.ready) add(toRoB);
        if (fromALU.ready) write_result_reg(fromALU.value, fromALU.robId);
        if (fromMem.ready) write_result_reg(fromMem.value, fromMem.robId);
        if (fromSB.ready) write_result_mem(fromSB.dest, fromSB.value, fromSB.robId);
        if (buffer.empty()) return;
        auto &top = buffer.front();
        if (top.state == WRITE_RESULT) {
            switch (top.robType) {
                case RoB_REG:
                    toReg_next = {top.dest, top.value, buffer.front_identity(), true};
                    break;
                case RoB_JALR:
                    toReg_next = {top.dest, top.instrAddr + 4, buffer.front_identity(), true};
                    if (top.jumpAddr != top.value) {
                        isFlush_next = true;
                        toCU_next.jumpAddr = top.value;
                    }
                    break;
                case RoB_STORE_BYTE:
                    if (!memBusy) {
                        toMem_next = {STORE_BYTE, top.dest, top.value, true};
                        toSB_next = {buffer.front_identity(), true};
                    }
                    break;
                case RoB_STORE_HALF:
                    if (!memBusy) {
                        toMem_next = {STORE_HALF, top.dest, top.value, true};
                        toSB_next = {buffer.front_identity(), true};
                    }
                    break;
                case RoB_STORE_WORD:
                    if (!memBusy) {
                        toMem_next = {STORE_WORD, top.dest, top.value, true};
                        toSB_next = {buffer.front_identity(), true};
                    }
                    break;
                case RoB_BRANCH:
                    toPred_next = {top.instrAddr, bool(top.value), true};
                    ++branchCnt;
                    if (top.value != top.isJump) {
                        ++failureCnt;
                        isFlush_next = true;
                        toCU_next.jumpAddr = top.value ? top.jumpAddr : top.instrAddr + 4;
                    }
                    break;
                case Rob_EXIT:
                    exit = true;
                    break;
            }
            if (!memBusy || (top.robType != RoB_STORE_BYTE && top.robType != RoB_STORE_HALF && top.robType != RoB_STORE_WORD)) {
                ++commitCnt;
//                print(regFile);
//                std::cerr << "executing ins count: " << std::dec << commitCnt << " PC= " << std::hex << std::uppercase << top.instrAddr << std::endl;
//                std::cerr << std::endl;
                buffer_next.pop_front();
            }
        }
    }

    void ReorderBuffer::next() {
        buffer = buffer_next;
        toReg = toReg_next;
        toReg_next.ready = false;
        toMem = toMem_next;
        toMem_next.ready = false;
        toSB = toSB_next;
        toSB_next.ready = false;
        isFlush = isFlush_next;
        isFlush_next = false;
        toCU = toCU_next;
        toPred = toPred_next;
        toPred_next.ready = false;
    }

    ui ReorderBuffer::next_rob_id(Decoder2RoB &toRoB) const {
        return (buffer.back_identity() + 1 + toRoB.ready) & kBufferSize;
    }

    FindResult ReorderBuffer::find_value(ui robId, Decoder2RoB &fromDec, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) const {
        if (fromDec.ready && ((buffer.back_identity() + 1) & kBufferSize) == robId) return {fromDec.value, fromDec.state == WRITE_RESULT};
        if (fromALU.ready && fromALU.robId == robId) return {fromALU.value, true};
        if (fromMem.ready && fromMem.robId == robId) return {fromMem.value, true};
        if (fromSB.ready && fromSB.robId == robId) return {fromSB.value, true};
        return {buffer.at(robId).value, buffer.at(robId).state == WRITE_RESULT};
    }

    void ReorderBuffer::flush() {
        buffer_next.clear();
    }

} // riscv