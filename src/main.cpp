#include "CPU.hpp"

int main() {
//    freopen("../testcases/hanoi.data", "r", stdin);
    std::cerr << std::hex;
    riscv::Memory memory;
    riscv::CPU cpu(&memory);
    memory.store_program(std::cin);
    std::cout << std::dec << cpu.run();
    return 0;
}