#ifndef HEAP_SAP_H
#define HEAP_SAP_H

#include "Heap.h"

namespace Azul
{
	class HeapSAP : public Heap
	{
	public:
		HeapSAP() = delete;

		void SAPSetBlockHead(Block* block);
		void SAPSetNextHeap(Heap* heap);
		void SAPSetPrevHeap(Heap* heap);
		void SAPDestroyHeap();
	};
}

#endif