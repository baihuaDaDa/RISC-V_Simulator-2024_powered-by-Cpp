#include "CPU.hpp"

int main() {
//    freopen("../testcases/bulgarian.data", "r", stdin);
    freopen("../bulgarian.out", "w", stderr);
    std::cerr << std::hex;
    riscv::Memory memory;
    riscv::CPU cpu(&memory);
    memory.store_program(std::cin);
    std::cout << cpu.run();
    return 0;
}