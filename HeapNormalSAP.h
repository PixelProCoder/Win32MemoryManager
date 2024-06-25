#ifndef HEAP_NORMAL_SAP_H
#define HEAP_NORMAL_SAP_H

#include "HeapNormal.h"
#include "BlockSAP.h"

namespace Azul
{
	class HeapNormalSAP final : public HeapNormal
	{
	public:
		HeapNormalSAP(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem);

		void* SAPHeapAlloc(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum);
		void SAPHeapFree(void* ptr);
	};
}

#endif