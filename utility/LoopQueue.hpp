#ifndef RISC_V_SIMULATE_QUEUE_HPP
#define RISC_V_SIMULATE_QUEUE_HPP

#include <algorithm>
#include "utils.hpp"

namespace riscv {

    const ui kMaxQueue = 100;

    template<class T>
    class LoopQueue {
    private:
        T data[kMaxQueue];
        ui head = 0, rear = 0, size_ = 0;

    public:
        LoopQueue() = default;

        void push_back(const T &t) {
            rear = (rear + 1) %kMaxQueue;
            data[rear] = t;
        }

        void pop_front() {
            head = (head + 1) % kMaxQueue;
        }

        T front() const {
            return data[(head + 1) % kMaxQueue];
        }

        T back() const {
            return data[rear];
        }

        T size() const {
            return size_;
        }

        bool empty() const {
            return size_ == 0;
        }

        bool full() const {
            return size_ == kMaxQueue - 1;
        }

        T &operator[](ui ind) const {
            return data[(ind + head + 1) % kMaxQueue];
        }
    };

}


#endif //RISC_V_SIMULATE_QUEUE_HPP
