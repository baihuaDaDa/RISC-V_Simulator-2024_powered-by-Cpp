#include "LoadStoreBuffer.hpp"

namespace riscv {

    LoadStoreBuffer::LoadStoreBuffer() = default;

    void LoadStoreBuffer::add(Decoder2LSB &fromDec) {
        if (fromDec.memType <= 4) {
            for (int i = 0; i < kBufferSize; ++i) {
                if (!loadBuffer[i].busy) {
                    loadBuffer_next[i] = LBEntry(fromDec, true);
                    break;
                }
            }
        } else {
            if (storeBuffer.full()) throw "store buffer full.";
            storeBuffer_next.push_back(SBEntry(fromDec));
        }
    }

    void LoadStoreBuffer::update_dependency(ui value, ui robId) {
        for (int i = 0; i < kBufferSize; ++i) {
            if (loadBuffer_next[i].busy) {
                if (loadBuffer_next[i].Qj == robId) {
                    loadBuffer_next[i].Vj = value;
                    loadBuffer_next[i].Qj = -1;
                }
            }
        }
        for (int i = 0; i < storeBuffer_next.size(); ++i) {
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
        if (!storeBuffer.empty() && !fromRoB.ready) {
            auto &storeEntry = storeBuffer.front();
            if (storeEntry.Qj == -1 && storeEntry.Qk == -1) {
                toRoB_next = {storeEntry.robId, storeEntry.Vj + storeEntry.imm, storeEntry.Vk, true};
            }
        }
        if (!memBusy) {
            for (int i = 0; i < kBufferSize; ++i) {
                auto &loadEntry = loadBuffer[i];
//                std::cerr << loadEntry.busy << " " << loadEntry.Qj << " " << storeBuffer.empty() << std::endl;
                if (loadEntry.busy && loadEntry.Qj == -1 && (storeBuffer.empty() || (!storeBuffer.empty() && loadEntry.age < storeBuffer.front().age))) {
                    toMem_next = {loadEntry.loadType, loadEntry.Vj + loadEntry.Vk, loadEntry.robId, true};
                    loadBuffer_next[i].busy = false;
                    break;
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

    bool LoadStoreBuffer::lb_full(Decoder2LSB &fromDec) const {
        int cnt = 0;
        for (int i = 0; i < kBufferSize; ++i) {
            if (!loadBuffer[i].busy) ++cnt;
        }
        return cnt <= (fromDec.ready && fromDec.memType <= 4);
    }

    bool LoadStoreBuffer::sb_full(Decoder2LSB &fromDec, RoB2SB &fromRoB) const {
        return storeBuffer.size() + (fromDec.ready && fromDec.memType > 4) - fromRoB.ready >= kBufferSize;
    }

} // riscv