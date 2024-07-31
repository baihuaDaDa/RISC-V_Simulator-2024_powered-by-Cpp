#include "ReorderBuffer.hpp"

namespace riscv {

    ReorderBuffer::ReorderBuffer() = default;

    bool ReorderBuffer::full() const {
        return buffer.full();
    }

    void ReorderBuffer::add(Decoder2RoB &toRoB) {
        if (buffer.full()) throw "RoB full.";
        buffer_next.push_back(RoBEntry{toRoB, EXECUTE});
        if (toRoB.robType == RoB_REG) toRegSta_next = {toRoB.dest, buffer_next.back_identity(), true};
    }

    void ReorderBuffer::write_result(ui value, ui robId) {
        buffer_next.at(robId).value = value;
        buffer_next.at(robId).state = WRITE_RESULT;
    }

    void ReorderBuffer::execute(Decoder2RoB &toRoB, ALUResult &fromALU, MemResult &fromMem, SB2RoB &fromSB, bool memBusy) {
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
                case RoB_JUMP:
                    break;
                case Rob_EXIT:
                    break;
            }
            if (!memBusy) buffer_next.pop_front();
        }
    }

    void ReorderBuffer::flush() {
        buffer = buffer_next;
        toReg = toReg_next;
        toReg_next.ready = false;
        toRegSta = toRegSta_next;
        toRegSta_next.ready = false;
        toMem = toMem_next;
        toMem_next.ready = false;
        toSB = toSB_next;
        toSB_next.ready = false;
    }

} // riscv