#include "LoadStoreBuffer.hpp"

namespace riscv {

    LoadStoreBuffer::LoadStoreBuffer() = default;

    void LoadStoreBuffer::add(riscv::Decoder2LSB &fromDec) {
        for (int i = 0; i < kBufferSize; ++i) {
            if (!loadBuffer[i].busy) {
                loadBuffer_next[i] = LBEntry(fromDec, true);
                break;
            }
        }
    }

    void LoadStoreBuffer::update_dependency(ui value, ui robId) {
        for (int i = 0; i < kBufferSize; ++i) {
            if (loadBuffer[i].busy) {
                if (loadBuffer[i].Qj == robId) {
                    loadBuffer_next[i].Vj = value;
                    loadBuffer_next[i].Qj = -1;
                    loadBuffer_next[i].dest = loadBuffer_next[i].Vj + loadBuffer_next[i].Vk;
                }
            }
        }
        for (int i = 0; i < storeBuffer.size(); ++i) {
            if (storeBuffer[i].Qj == robId) {
                storeBuffer_next[i].Vj = value;
                storeBuffer_next[i].Qj = -1;
                storeBuffer_next[i].dest = storeBuffer_next[i].Vj + storeBuffer_next[i].imm;
            }
            if (storeBuffer[i].Qk == robId) {
                storeBuffer_next[i].Vk = value;
                storeBuffer_next[i].Qk = -1;
            }
        }
    }

    void LoadStoreBuffer::execute(Decoder2LSB &fromDec, ALUResult &fromALU, MemResult &fromMem, RoB2SB &fromRoB,
                                  bool memBusy, bool isFlush) {
        if (isFlush) {
            flush();
            return;
        }
        if (fromDec.ready) add(fromDec);
        if (fromALU.ready) update_dependency(fromALU.value, fromALU.robId);
        if (fromMem.ready) update_dependency(fromMem.value, fromMem.robId);
        if (fromRoB.ready) {
            auto &storeEntry = storeBuffer.front();
            if (storeEntry.robId != fromRoB.robId) throw "wrong store entry.";
            storeBuffer_next.pop_front();
        }
        if (!storeBuffer.empty()) {
            auto &storeEntry = storeBuffer.front();
            if (storeEntry.Qj == -1 && storeEntry.Qk == -1) {
                toRoB_next = {storeEntry.robId, storeEntry.Vk, true};
            }
        }
        if (!memBusy) {
            for (int i = 0; i < kBufferSize; ++i) {
                auto &loadEntry = loadBuffer[i];
                if (loadEntry.busy && loadEntry.Qj == -1 && loadEntry.age < storeBuffer.front().age) {
                    toMem_next = {loadEntry.loadType, loadEntry.dest, loadEntry.robId, true};
                    loadBuffer_next[i].busy = false;
                }
            }
        }
    }

    void LoadStoreBuffer::next() {
        loadBuffer = loadBuffer_next;
        storeBuffer = storeBuffer_next;
        toMem = toMem_next;
        toMem_next.ready = false;
        toRoB = toRoB_next;
        toRoB_next.ready = false;
    }

    void LoadStoreBuffer::flush() {
        for (int i = 0; i < kBufferSize; ++i) {
            loadBuffer_next[i].busy = false;
        }
        storeBuffer_next.clear();
    }

    bool LoadStoreBuffer::lb_full() const {
        bool full = true;
        for (int i = 0; i < kBufferSize; ++i) {
            if (!loadBuffer[i].busy) {
                full = false;
                break;
            }
        }
        return full;
    }

    bool LoadStoreBuffer::sb_full() const {
        return storeBuffer.full();
    }

} // riscv