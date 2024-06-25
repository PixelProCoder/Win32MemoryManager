#include "BlockSAP.h"

Azul::BlockSAP::BlockSAP(Heap* heap, size_t size, const char* name, const size_t lineNum)
	: Block(heap, size, name, lineNum)
{
}

void Azul::BlockSAP::SAPSetAllocIndex(size_t index)
{
	SetAllocIndex(index);
}

void Azul::BlockSAP::SAPSetAllocSize(size_t size)
{
	SetAllocSize(size);
}

void Azul::BlockSAP::SAPSetGlobalNext(Block* block)
{
	SetGlobalNext(block);
}

void Azul::BlockSAP::SAPSetGlobalPrev(Block* block)
{
	SetGlobalPrev(block);
}

void Azul::BlockSAP::SAPSetHeap(Heap* heap)
{
	SetHeap(heap);
}

void Azul::BlockSAP::SAPSetHeapNext(Block* block)
{
	SetHeapNext(block);
}

void Azul::BlockSAP::SAPSetHeapPrev(Block* block)
{
	SetHeapPrev(block);
}

void Azul::BlockSAP::SAPSetUserBlock(void* ptr)
{
	SetUserBlock(ptr);
}
