#include "HeapFixedSAP.h"

Azul::HeapFixedSAP::HeapFixedSAP(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem)
	: HeapFixed(winHeapHandle, heapSize, name, hType, pMem)
{
}

void* Azul::HeapFixedSAP::SAPHeapAlloc(size_t size, Mem::Align align, const char* name, size_t lineNum)
{
	return Allocate(size, align, name, lineNum);
}

void Azul::HeapFixedSAP::SAPHeapFree(void* ptr)
{
	Deallocate(ptr);
}

void Azul::HeapFixedSAP::SAPHeapFormat(size_t numBlocks, size_t sizePerBlock)
{
	FormatHeap(numBlocks, sizePerBlock);
}
