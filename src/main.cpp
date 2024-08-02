#include "CPU.hpp"

int main() {
//    freopen("../testcases/basicopt1.data", "r", stdin);
//    freopen("../basicopt1.out", "w", stderr);
//    std::cerr << std::hex;
    riscv::Memory memory;
    riscv::CPU cpu(&memory);
    memory.store_program(std::cin);
    std::cout << cpu.run();
    return 0;
}