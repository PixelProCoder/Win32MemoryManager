#include "HeapSAP.h"

void Azul::HeapSAP::SAPSetBlockHead(Block* block)
{
	pBlkHead = block;
}

void Azul::HeapSAP::SAPSetNextHeap(Heap* heap)
{
	SetNextHeap(heap);
}

void Azul::HeapSAP::SAPSetPrevHeap(Heap* heap)
{
	SetPrevHeap(heap);
}

void Azul::HeapSAP::SAPDestroyHeap()
{
	Destroy();
}
