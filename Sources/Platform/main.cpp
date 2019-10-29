#include "stdafx.h"

namespace Core {

    bool        Init(){
        Core::Heap_Init();
        Core::Time_Init();
        FindPaths ();
        if (!Core::Debug_Init()) return false;
        //..
        return true;
    }

    void        Free(){
        //..
        Core::Debug_Free();
        Core::Time_Free();
        Core::Heap_Free();
    }

}



uint64_t state0 = 1;
uint64_t state1 = 2;
uint64_t xorshift128plus() {
   uint64_t s1 = state0;
   uint64_t s0 = state1;
   state0 = s0;
   s1 ^= s1 << 23;
   s1 ^= s1 >> 17;
   s1 ^= s0;
   s1 ^= s0 >> 26;
   state1 = s1;
   return state0 + state1;
}