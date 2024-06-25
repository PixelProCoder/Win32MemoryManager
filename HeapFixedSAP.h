#ifndef HEAP_FIXED_SAP_H
#define HEAP_FIXED_SAP_H

#include "HeapFixed.h"

namespace Azul
{
	class HeapFixedSAP final : public HeapFixed
	{
	public:
		HeapFixedSAP(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem);

		void* SAPHeapAlloc(size_t size, Mem::Align align, const char* name, size_t lineNum);
		void SAPHeapFree(void* ptr);
		void SAPHeapFormat(size_t numBlocks, size_t sizePerBlock);
	};
}

#endif