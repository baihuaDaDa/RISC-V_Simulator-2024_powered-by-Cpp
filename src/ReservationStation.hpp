#ifndef RISC_V_SIMULATOR_RESERVATIONSTATION_HPP
#define RISC_V_SIMULATOR_RESERVATIONSTATION_HPP

#include <array>
#include "../utility/LoopQueue.hpp"
#include "CommonDataBus.hpp"

namespace riscv {

    struct RSEntry {
        CalcType calcType;
        int Qj, Qk;
        ui Vj, Vk;
        ui robId;
        bool busy = false;

        RSEntry() = default;

        RSEntry(const Decoder2RS &toRS, bool busy_) : calcType(toRS.calcType), Qj(toRS.Qj), Qk(toRS.Qk), Vj(toRS.Vj),
                                                      Vk(toRS.Vk), robId(toRS.robId), busy(busy_) {}
    };

    class ReservationStation {
    private:
        static constexpr ui kStationSize = 16;
        std::array<RSEntry, kStationSize> station_next;
        RS2ALU toALU_next;
    public:
        std::array<RSEntry, kStationSize> station;
        RS2ALU toALU;

    private:
        void add(Decoder2RS &toRS);

        void update_dependency(ui value, ui robId);

    public:
        ReservationStation();

        void execute(Decoder2RS &fromDec, ALUResult &fromALU, MemResult &fromMem, bool isFlush);

        void next();

        void flush();

        bool full() const;

    };

} // riscv

#endif //RISC_V_SIMULATOR_RESERVATIONSTATION_HPP
