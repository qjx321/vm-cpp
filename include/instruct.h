#ifndef __INSTRUCT__
#define __INSTRUCT__

#include<cstdint>

struct Instruct
{
    int32_t opcode;
    int32_t left;
    int32_t right;
};


#endif