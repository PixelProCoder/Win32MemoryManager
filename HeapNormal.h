#ifndef HEAP_NORMAL_H
#define HEAP_NORMAL_H

#include "Heap.h"

namespace Azul
{
	class HeapNormal : public Heap
	{
	public:

	protected:
		HeapNormal(HANDLE winHeapHandle, unsigned int heapSize, const char *const name, Type hType, Mem *pMem);
	
		HeapNormal() = delete;
		HeapNormal(const HeapNormal &) = delete;
		HeapNormal &operator = (const HeapNormal &tmp) = delete;
		HeapNormal(HeapNormal &&) = delete;
		HeapNormal &operator = (HeapNormal &&) = delete;
		virtual ~HeapNormal() = default;

		void FreeBlock(Block* block);

		void* Allocate(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum) override;
		void Deallocate(void* ptr);
		void Destroy() override;
	};
}

#endif
