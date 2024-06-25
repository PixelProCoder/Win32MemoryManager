#ifndef BLOCK_SAP_H
#define BLOCK_SAP_H

#include "Block.h"

namespace Azul
{
	class BlockSAP final : public Block
	{
	public:
		BlockSAP(Heap* heap, size_t size, const char* inName, const size_t lineNum);

		void SAPSetAllocIndex(size_t index);
		void SAPSetAllocSize(size_t size);
		void SAPSetGlobalNext(Block* block);
		void SAPSetGlobalPrev(Block* block);
		void SAPSetHeap(Heap* heap);
		void SAPSetHeapNext(Block* block);
		void SAPSetHeapPrev(Block* block);
		void SAPSetUserBlock(void* ptr);
	};
}

#endif