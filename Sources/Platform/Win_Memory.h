
namespace  Core {
namespace  Mem {

void        Virtual_Free (void* Ptr);
void*       Virtual_Get (u64 Size);

void*       Heap_Get (u64 Size);
void        Heap_Free (void* Ptr);

}

void        Heap_Init();
void        Heap_Free();

}
