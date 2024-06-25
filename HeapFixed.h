#ifndef HEAP_FIXED_H
#define HEAP_FIXED_H

#include "Heap.h"

namespace Azul
{
	class Block;

	class HeapFixed : public Heap
	{
	public:
		static const uint32_t FreeHeapExtra = 4 * 1024;

		void SetOverflowHeap(Heap*);
		Heap* GetOverflowHeap() const;

	protected:
		HeapFixed(HANDLE winHeapHandle, unsigned int HeapSize, const char* const Name, Type hType, Mem* pMem);

		HeapFixed() = delete;
		HeapFixed(const HeapFixed&) = delete;
		HeapFixed& operator = (const HeapFixed& tmp) = delete;
		HeapFixed(HeapFixed&&) = delete;
		HeapFixed& operator = (HeapFixed&&) = delete;
		virtual ~HeapFixed() = default;

		void FormatHeap(size_t numBlocks, size_t sizePerBlock);

		void* Allocate(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum) override;
		void Deallocate(void* ptr) override;
		void Destroy() override;

	private:
		Block* heapOffset;
		Block* pFixedFreeHead;
		Heap* pOverflow;
	};
}

#endif
