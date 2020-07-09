//
// Created by Vortox Oganesson on 7/9/20.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H
#include "constant.h"
#include "typedef.h"
#include <memory>
namespace riscv{
    struct predictor{
        uint8_t two_bit[4096]; // low 12+2 bits
        int total_cnt=0;
        int hit_cnt=0;
        bool predict(dword_t cur_pc){
            dword_t index=(cur_pc>>2)&0xfff;
            return two_bit[index]>=2;
        }
        void set(dword_t pc,bool hit){
            dword_t index=(pc>>2)&0xfff;
            ++total_cnt;
            if(hit){
                two_bit[index]=std::min(4,two_bit[index]+1);
                ++hit_cnt;
            }
            else {
                two_bit[index]=std::max(0,two_bit[index]-1);
            }
        }
        double tell()const {return total_cnt?(double )hit_cnt/(double )total_cnt:0.0;}
        predictor(){
            memset(two_bit,0,sizeof(two_bit));
        }
    };
}
#endif //RISC_V_SIMULATOR_PREDICTOR_H
