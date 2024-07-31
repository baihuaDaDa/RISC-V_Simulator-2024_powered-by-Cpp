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
                if (loadBuffer_next[i].Qj == robId) {
                    loadBuffer_next[i].Vj = value;
                    loadBuffer_next[i].Qj = -1;
                }
            }
        }
        for (int i = 0; i < storeBuffer.size(); ++i) {
            if (storeBuffer_next[i].Qj == robId) {
                storeBuffer_next[i].Vj = value;
                storeBuffer_next[i].Qj = -1;
            }
            if (storeBuffer_next[i].Qk == robId) {
                storeBuffer_next[i].Vk = value;
                storeBuffer_next[i].Qk = -1;
            }
        }
    }

    void LoadStoreBuffer::execute(Decoder2LSB &fromDec, ALUResult &fromALU, MemResult &fromMem, RoB2SB &fromRoB,
                                  bool memBusy) {
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
                toRoB_next = {storeEntry.robId, storeEntry.Qk, true};
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

    void LoadStoreBuffer::flush() {}

} // riscv