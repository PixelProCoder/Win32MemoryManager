#include "Heap.h"
#include "Block.h"
#include "BlockSAP.h"
#include "MemSAP.h"

namespace Azul
{
	Heap::Heap(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem)
		: pBlkHead{ nullptr }
		, pMem{ pMem }
		, mType{ hType }
		, mWinHeapHandle{ winHeapHandle }
		, mInfo{}
	{
		strcpy_s(mInfo.Name, Heap::Info::NameSize, name);
		mInfo.TotalSize = heapSize;
	}

	void Heap::GetInfo(Info& retInfo) const
	{
		retInfo = mInfo;
	};

	Block* Heap::GetTrackingHead() const
	{
		return pBlkHead;
	}

	Heap::Type Heap::GetHeapType() const
	{
		return mType;
	}

	Heap* Heap::GetNextHeap() const
	{
		return reinterpret_cast<Heap*>(hLink.pNext);
	}

	Heap* Heap::GetPrevHeap() const
	{
		return reinterpret_cast<Heap*>(hLink.pPrev);
	}

	void Heap::SetNextHeap(Heap* next)
	{
		hLink.pNext = reinterpret_cast<DLink*>(next);
	}

	void Heap::SetPrevHeap(Heap* prev)
	{
		hLink.pPrev = reinterpret_cast<DLink*>(prev);
	}

	void Heap::Destroy()
	{
		// Update mem info
		Mem::Info info;
		Mem::GetInfo(info);

		info.CurrHeapCount--;

		static_cast<MemSAP*>(pMem)->SAPSetInfo(info);

		HANDLE heapHandle = mWinHeapHandle;
		HeapFree(heapHandle, 0, this);

		HeapDestroy(heapHandle);
	}

	void Heap::RemoveBlockFromList(Block* block)
	{
		BlockSAP* nextBlock = static_cast<BlockSAP*>(block->GetHeapNext());
		BlockSAP* prevBlock = static_cast<BlockSAP*>(block->GetHeapPrev());

		if (nextBlock)
		{
			nextBlock->SAPSetHeapPrev(prevBlock);
		}

		if (pBlkHead == block)
		{
			pBlkHead = nextBlock;
		}
		else
		{
			prevBlock->SAPSetHeapNext(nextBlock);
		}

		BlockSAP* nextGlobalBlock = static_cast<BlockSAP*>(block->GetGlobalNext());
		BlockSAP* prevGlobalBlock = static_cast<BlockSAP*>(block->GetGlobalPrev());

		if (nextGlobalBlock)
		{
			nextGlobalBlock->SAPSetGlobalPrev(prevGlobalBlock);
		}

		Block* globalHead = Mem::DebugGetGlobalTrackingHead();

		if (globalHead == block)
		{
			// Add the next block to the front of the global tracking list
			static_cast<MemSAP*>(pMem)->SAPSetGlobalHead(nextGlobalBlock);
		}
		else
		{
			prevGlobalBlock->SAPSetGlobalNext(nextGlobalBlock);
		}
	}
}
