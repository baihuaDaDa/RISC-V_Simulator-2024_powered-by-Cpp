#include "ReservationStation.hpp"

namespace riscv {

    ReservationStation::ReservationStation() = default;

    void ReservationStation::add(Decoder2RS &toRS) {
        int tar = -1;
        for (int i = 0; i < kStationSize; i++) {
            if (!station[i].busy) {
                tar = i;
                break;
            }
        }
        if (tar == -1) throw "RS full.";
        station_next[tar] = RSEntry{toRS, true};
    }

    void ReservationStation::update_dependency(ui value, ui robId) {
        for (int i = 0; i < kStationSize; i++) {
            if (station[i].busy) {
                if (station[i].Qj == robId) {
                    station_next[i].Vj = value;
                    station_next[i].Qj = -1;
                }
                if (station[i].Qk == robId) {
                    station_next[i].Vk = value;
                    station_next[i].Qk = -1;
                }
            }
        }
    }

    void ReservationStation::execute(Decoder2RS &fromDec, ALUResult &fromALU, MemResult &fromMem) {
        if (fromDec.ready) add(fromDec);
        if (fromMem.ready) update_dependency(fromMem.value, fromMem.robId);
        if (fromALU.ready) update_dependency(fromALU.value, fromALU.robId);
        for (int i = 0; i < kStationSize; i++) {
            if (station[i].busy) {
                if (station[i].Qj == -1 && station[i].Qk == -1) {
                    station_next[i].busy = false;
                    toALU_next = RS2ALU{station[i].calcType, station[i].Vj, station[i].Vk, station[i].robId, true};
                    return;
                }
            }
        }
    }

    void ReservationStation::flush() {
        station = station_next;
        toALU = toALU_next;
        toALU_next.ready = false;
    }

    bool ReservationStation::full() const {
        bool full = true;
        for (int i = 0; i < kStationSize; i++)
            if (!station[i].busy) full = false;
        return full;
    }

} // riscv