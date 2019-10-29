#include "stdafx.h"


namespace  Core {

HANDLE    BaseHeap = 0;

void    Heap_Init()
{
    BaseHeap   = HeapCreate(0, 0x010000, 0); // Min 64kb page
}

void    Heap_Free()
{
    HeapDestroy(BaseHeap);
}


namespace  Mem {

void     Virtual_Free(void* Ptr) 
{
    if (!Ptr) return;
    VirtualFree(Ptr, 0, MEM_RELEASE | MEM_DECOMMIT);
}


void*   Virtual_Get(u64 Size) 
{
    Size = (((Size)+65535) & ~65535);
    return VirtualAlloc(NULL, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}


void*   Heap_Get(u64 Size)
{
    //
    if (BaseHeap == 0) Heap_Init();
    //
    return HeapAlloc(BaseHeap, 0, Size);
}

void    Heap_Free(void* Ptr)
{
    HeapFree(BaseHeap, 0, Ptr);
}



}
}
