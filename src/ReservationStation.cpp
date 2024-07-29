#include "ReservationStation.hpp"

namespace riscv {

    ReservationStation::ReservationStation() = default;

    void ReservationStation::add(Decoder2RS &toRS) {
        if (station.full()) throw "RS full.";
        station_next.push_back(RSEntry{toRS, true});
    }

    void ReservationStation::execute(Decoder2RS &toRS) {
        if (toRS.ready) add(toRS);

    }

    void ReservationStation::flush() {
    }

    bool ReservationStation::full() const {
        return station.full();
    }

} // riscv