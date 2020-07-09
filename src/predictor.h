//
// Created by Vortox Oganesson on 7/9/20.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H
#include "constant.h"
#include "typedef.h"
#include <memory>
#include <cmath>
namespace riscv{
    struct predictor{
        // up to 32
        template<uint32_t size>
        struct history{
            uint8_t two_bit[1u<<size];
            uint32_t pattern=0;
            const static uint32_t mask=(1u<<size)-1;
            bool hit(){return two_bit[pattern&mask]>=2;}
            bool learn(bool bit){
                bool rt=!((two_bit[pattern&mask]>=2)^bit);
                auto& ref=two_bit[pattern&mask];
                ref=bit?std::min(4,ref+1):std::max(0,ref-1);
                pattern<<=1u;pattern|=bit;
                return rt;
            }
            history():pattern(0){
                for(int i=0;i<1u<<size;++i)two_bit[i]=2;
            }
        };
        history<4> slots[4096]; // low 12+2 bits
        int total_cnt=0;
        int hit_cnt=0;
        bool predict(dword_t cur_pc){
            return slots[(cur_pc>>2u)&0xfff].hit();
        }
        void set(dword_t pc,bool hit){
            ++total_cnt;
            if(slots[(pc>>2u)&0xfff].learn(hit))++hit_cnt;
        }
        double tell()const {return total_cnt?(double )hit_cnt/(double )total_cnt:0.0;}
    };
}
#endif //RISC_V_SIMULATOR_PREDICTOR_H
