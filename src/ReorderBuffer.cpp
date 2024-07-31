#include "ReorderBuffer.hpp"

namespace riscv {

    ReorderBuffer::ReorderBuffer() = default;

    bool ReorderBuffer::full() const {
        return buffer.full();
    }

    void ReorderBuffer::add(Decoder2RoB &toRoB) {
        if (buffer.full()) throw "RoB lb_full.";
        buffer_next.push_back(RoBEntry{toRoB, EXECUTE});
        if (toRoB.robType == RoB_REG) toRegSta_next = {toRoB.dest, buffer_next.back_identity(), true};
    }

    void ReorderBuffer::write_result(ui value, ui robId) {
        buffer_next.at(robId).value = value;
        buffer_next.at(robId).state = WRITE_RESULT;
    }

    void ReorderBuffer::execute(Decoder2RoB &toRoB, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool memBusy) {
        if (isFlush) {
            flush();
            return;
        }
        if (toRoB.ready) add(toRoB);
        if (fromALU.ready) write_result(fromALU.value, fromALU.robId);
        if (fromMem.ready) write_result(fromMem.value, fromMem.robId);
        if (fromSB.ready) write_result(fromSB.value, fromSB.robId);
        auto &top = buffer.front();
        if (top.state == WRITE_RESULT) {
            switch (top.robType) {
                case RoB_REG:
                    toReg_next = {top.dest, top.value, buffer.front_identity(), true};
                    break;
                case RoB_JALR:
                    toReg_next = {top.dest, top.value, buffer.front_identity(), true};
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
                    if (top.value != top.isJump) {
                        isFlush_next = true;
                        toCU_next.jumpAddr = top.value ? top.instrAddr + 4 : top.jumpAddr;
                    }
                    break;
                case Rob_EXIT:
                    exit = true;
                    break;
            }
            if (!memBusy) buffer_next.pop_front();
        }
    }

    void ReorderBuffer::next() {
        buffer = buffer_next;
        toReg = toReg_next;
        toReg_next.ready = false;
        toRegSta = toRegSta_next;
        toRegSta_next.ready = false;
        toMem = toMem_next;
        toMem_next.ready = false;
        toSB = toSB_next;
        toSB_next.ready = false;
        isFlush = isFlush_next;
        isFlush_next = false;
        toCU = toCU_next;
    }

    ui ReorderBuffer::next_rob_id() const {
        return (buffer.back_identity() + 1) >> kBufferSizeBin;
    }

    FindResult ReorderBuffer::find_value(ui robId,ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB) const {
        if (fromALU.ready && fromALU.robId == robId) return {fromALU.value, true};
        if (fromMem.ready && fromMem.robId == robId) return {fromMem.value, true};
        if (fromSB.ready && fromSB.robId == robId) return {fromSB.value, true};
        return {buffer.at(robId).value, buffer.at(robId).state == WRITE_RESULT};
    }

    void ReorderBuffer::flush() {
        buffer_next.clear();
    }

} // riscv