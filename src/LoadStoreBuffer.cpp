#include "LoadStoreBuffer.hpp"

namespace riscv {

    LoadStoreBuffer::LoadStoreBuffer() = default;

    void LoadStoreBuffer::add(riscv::Decoder2LSB &fromDec) {
        for (int i = 0; i < kBufferSize; ++i) {
            if (!loadBuffer[i].busy) {
                auto &loadEntry = loadBuffer[i];
                loadEntry = LBEntry(fromDec, true);
                if (fromDec.Qj == -1 && fromDec.Qk == -1 && !storeBuffer.empty()) {
                    for (int j = storeBuffer.size() - 1; j >= 0; --j) {
                        auto &storeEntry = storeBuffer[j];
                        if (storeEntry.Qj == -1 && storeEntry.Qk == -1 && storeEntry.age < loadEntry.age) {
                            loadEntry.value = storeEntry.Vk;
                            break;
                        }
                    }
                }
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

    void LoadStoreBuffer::execute(Decoder2LSB &fromDec, ALUResult &fromALU, MemResult &fromMem) {
        if (fromDec.ready) add(fromDec);
        if (fromALU.ready) update_dependency(fromALU.value, fromALU.robId);
        if (fromMem.ready) update_dependency(fromMem.value, fromMem.robId);
    }

    void LoadStoreBuffer::flush() {}

} // riscv