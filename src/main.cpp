#include "CPU.hpp"

int main() {
    freopen("../testcases/naive.data", "r", stdin);
    riscv::Memory memory;
    riscv::CPU cpu(&memory);
    memory.store_program(std::cin);
    std::cout << cpu.run();
    return 0;
}