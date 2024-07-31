#include "CPU.hpp"

int main() {
    freopen("../testcases/array_test1.data", "r", stdin);
    riscv::Memory memory;
    riscv::CPU cpu(&memory);
    memory.store_program(std::cin);
    std::cout << cpu.run();
    return 0;
}