#ifndef GLOBAL_NEW_H
#define GLOBAL_NEW_H

#include "Mem.h"
#include "Heap.h"
#include "HeapFixed.h"
#include "HeapNormal.h"

// For Normal Heap
void *operator new(size_t inSize, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum);
void operator delete(void *p, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum);

void *operator new[](size_t inSize, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum);
void operator delete[](void *p, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum);

// For Fixed Heap
void *operator new(size_t inSize, Azul::HeapFixed *const pHeap, const char *inName, size_t lineNum);
void operator delete(void *p, Azul::HeapFixed *const pHeap, const char *inName, size_t lineNum);

// -----------------------------------------------------------------
// The new system needs to coexist with existing new/delete()
// -----------------------------------------------------------------
void *operator new(size_t inSize);
void operator delete(void *p);

void *operator new[](size_t inSize);
void operator delete[](void *p);

#endif
