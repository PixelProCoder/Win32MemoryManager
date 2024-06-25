#include "HeapNormalSAP.h"

Azul::HeapNormalSAP::HeapNormalSAP(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem)
	: HeapNormal(winHeapHandle, heapSize, name, hType, pMem)
{
}

void* Azul::HeapNormalSAP::SAPHeapAlloc(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum)
{
	return Allocate(size, align, name, lineNum);
}

void Azul::HeapNormalSAP::SAPHeapFree(void* ptr)
{
	Deallocate(ptr);
}
