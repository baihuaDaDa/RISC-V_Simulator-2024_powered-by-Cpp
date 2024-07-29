#include "ReorderBuffer.hpp"

namespace riscv {

    bool ReorderBuffer::full() const {
        return buffer.full();
    }

    void ReorderBuffer::add(Decoder2RoB toRoB) {
        if (buffer.full()) throw "RoB full.";
        buffer_next.push_back(RoBEntry{toRoB, true});
    }

} // riscv