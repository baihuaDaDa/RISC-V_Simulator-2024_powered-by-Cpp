#ifndef RISC_V_SIMULATE_QUEUE_HPP
#define RISC_V_SIMULATE_QUEUE_HPP

#include <algorithm>
#include "utils.hpp"

namespace riscv {

    template<class T, ui kMaxQueueBin>
    class LoopQueue {
    public:
        static constexpr ui kMaxQueue = 1 << kMaxQueueBin;
    private:
        T data[kMaxQueue]{};
        ui head = 0, rear = 0, size_ = 0;

    public:
        LoopQueue() = default;

        LoopQueue &operator=(LoopQueue &rhs) {
            if (this == &rhs) return *this;
            for (ui i = (rhs.head + 1) >> kMaxQueueBin; rhs.head != rhs.rear; i = (i + 1) >> kMaxQueueBin) {
                data[i] = rhs.data[i];
                if (i == rhs.rear) break;
            }
            head = rhs.head;
            rear = rhs.rear;
            size_ = rhs.size_;
            return *this;
        }

        void push_back(const T &t) {
            rear = (rear + 1) >> kMaxQueueBin;
            data[rear] = t;
            ++size_;
        }

        void pop_front() {
            head = (head + 1) >> kMaxQueueBin;
            --size_;
        }

        void clear() {
            head = rear = size_ = 0;
        }

        T &front() {
            return data[(head + 1) >> kMaxQueueBin];
        }

        const T &front() const {
            return data[(head + 1) >> kMaxQueueBin];
        }

        T &back() {
            return data[rear];
        }

        const T &back() const {
            return data[rear];
        }

        ui size() const {
            return size_;
        }

        bool empty() const {
            return size_ == 0;
        }

        bool full() const {
            return size_ == kMaxQueue - 1;
        }

        T &operator[](ui ind) {
            return data[(ind + head + 1) >> kMaxQueueBin];
        }

        T &at(ui id) {
            return data[id];
        }

        const T &at(ui id) const {
            return data[id];
        }

        ui front_identity() const {
            return (head + 1) >> kMaxQueueBin;
        }

        ui back_identity() const {
            return rear;
        }

    };

}


#endif //RISC_V_SIMULATE_QUEUE_HPP
