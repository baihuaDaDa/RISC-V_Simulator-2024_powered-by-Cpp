#ifndef RISC_V_SIMULATOR_RESERVATIONSTATION_HPP
#define RISC_V_SIMULATOR_RESERVATIONSTATION_HPP

#include "../utility/LoopQueue.hpp"
#include "CommonDataBus.hpp"

namespace riscv {

    struct RSEntry {
        CalcType calcType;
        ui Qj, Qk, Vj, Vk, dest, imm;
        ui robId;
        bool busy;

        RSEntry() = default;

        RSEntry(const Decoder2RS &toRS, bool busy_) : calcType(toRS.calcType), Qj(toRS.Qj), Qk(toRS.Qk), Vj(toRS.Vj),
                                                      Vk(toRS.Vk), dest(toRS.dest), imm(toRS.imm), robId(toRS.robId),
                                                      busy(busy_) {}
    };

    class ReservationStation {
    private:
        LoopQueue<RSEntry> station_next;
        RS2ALU toALU_next;
    public:
        LoopQueue<RSEntry> station;
        RS2ALU toaLU;

    private:
        void add(Decoder2RS &toRS);

    public:
        ReservationStation();

        void execute(Decoder2RS &toRS);

        void flush();

        bool full() const;

    };

} // riscv

#endif //RISC_V_SIMULATOR_RESERVATIONSTATION_HPP
