#include <iostream>
#include <fstream>
#include "memory.h"
#include "cpu.h"
using namespace std;
using namespace riscv;
//#define DEBUG
int main() {
    memory mem;
#ifdef DEBUG
    ifstream ifs("../riscv-data/pi.data");
    mem.open(ifs);
#else
    mem.open(cin);
#endif
    cpu instance(&mem);
    try{
        instance.run();
    } catch (terminal_exception& e) {
        cout<<(instance.registers[10]&255u)<<endl;
    }

    return 0;
}
