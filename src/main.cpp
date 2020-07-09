#include <iostream>
#include <fstream>
#include "memory.h"
#include "cpu.h"

using namespace std;
using namespace riscv;
//#define N_DEBUG
int main() {
    memory mem;
#ifndef N_DEBUG
    ifstream ifs("../riscv-data/testcases/pi.data");
    if(ifs.fail())throw std::runtime_error("File open failed");
    mem.open(ifs);
#else
    mem.open(cin);
#endif
    cpu instance(&mem);
    instance.run();
    cout << (instance.registers[10] & 255u) << endl;
//    cout<<instance.pre.tell()<<" "<<instance.pre.hit_cnt<<"/"<<instance.pre.total_cnt<<endl;
    return 0;
}
