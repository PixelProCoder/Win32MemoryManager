#include "HeapNormal.h"
#include "BlockSAP.h"
#include "Registration.h"
#include "MemSAP.h"

namespace Azul
{
	HeapNormal::HeapNormal(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem)
		: Heap(winHeapHandle, heapSize, name, hType, pMem)
	{
	}

	void* HeapNormal::Allocate(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum)
	{
		int alignmentSize = Mem::GetAlignment(align);

		void* const ptr = HeapAlloc(mWinHeapHandle, 0, sizeof(Block) + size + alignmentSize);

		BlockSAP* const blockHdr = new(ptr) BlockSAP(this, size, name, lineNum);

		// Update mem info
		Mem::Info info;
		Mem::GetInfo(info);

		info.CurrAllocIndex++;
		info.CurrNumAlloc++;
		if (info.CurrNumAlloc > info.PeakNumAlloc)
		{
			info.PeakNumAlloc = info.CurrNumAlloc;
		}

		info.CurrBytesUsed += size;
		if (info.CurrBytesUsed > info.PeakBytesUsed)
		{
			info.PeakBytesUsed = info.CurrBytesUsed;
		}

		static_cast<MemSAP*>(pMem)->SAPSetInfo(info);

		// Set block info
		blockHdr->SAPSetAllocIndex(info.CurrAllocIndex);

		BlockSAP* globalHead = static_cast<BlockSAP*>(Mem::DebugGetGlobalTrackingHead());
		if (globalHead)
		{
			// Add the new block to the front of the tracking list
			globalHead->SAPSetGlobalPrev(blockHdr);
			blockHdr->SAPSetGlobalNext(globalHead);
			//blockHdr->gLink.pNext = (DLink*)pMem->pGlobalHead;
		}

		static_cast<MemSAP*>(pMem)->SAPSetGlobalHead(blockHdr);

		// Update heap info
		mInfo.CurrNumAlloc++;
		if (mInfo.CurrNumAlloc >= mInfo.PeakNumAlloc)
		{
			mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
		}

		mInfo.CurrBytesUsed += size;

		if (mInfo.CurrBytesUsed >= mInfo.PeakBytesUsed)
		{
			mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
		}

		if (pBlkHead)
		{
			// Add the new block to the front of the tracking list
			static_cast<BlockSAP*>(pBlkHead)->SAPSetHeapPrev(blockHdr);
			blockHdr->SAPSetHeapNext(pBlkHead);
		}

		// Update pBlkHead to point to the new blockHdr

		pBlkHead = blockHdr;

		const size_t offset = reinterpret_cast<size_t>(blockHdr + 1);

		// Compute the padding required to align the address correctly.
		const int padding = -static_cast<int>(offset) & (alignmentSize - 1);

		// Compute the aligned address.
		const size_t aligned = offset + padding;

		if (padding > 0)
		{
			// If offset is not aligned, go back up the size of Block* and set the secret pointer.
			Block** secretPtr = reinterpret_cast<Block**>(aligned - sizeof(Block*));
			*secretPtr = blockHdr;
		}

		// Add the aligned address to the tracking list.
		void* alignedPtr = reinterpret_cast<void*>(aligned);
		static_cast<MemSAP*>(pMem)->SAPRegisterPtr(alignedPtr);
		blockHdr->SAPSetUserBlock(alignedPtr);

		return alignedPtr;
	}

	void HeapNormal::Deallocate(void* ptr)
	{
		Block* blockHdr = *reinterpret_cast<Block**>(reinterpret_cast<size_t>(ptr) - sizeof(Block*));

		RemoveBlockFromList(blockHdr);

		// Remove pointer registration
		static_cast<MemSAP*>(pMem)->SAPUnRegisterPtr(ptr);

		FreeBlock(blockHdr);
	}

	void HeapNormal::FreeBlock(Block* block)
	{
		HeapFree(mWinHeapHandle, 0, block);

		// Update heap info
		mInfo.CurrNumAlloc--;
		mInfo.CurrBytesUsed -= block->GetAllocSize();

		// Update mem info
		Mem::Info info;
		Mem::GetInfo(info);

		info.CurrNumAlloc--;
		info.CurrBytesUsed -= block->GetAllocSize();

		static_cast<MemSAP*>(pMem)->SAPSetInfo(info);
	}

	void HeapNormal::Destroy()
	{
		// Traverse the block list and remove each block one by one.
		Block* currBlock = pBlkHead;
		while (currBlock)
		{
			Block* next = currBlock->GetHeapNext();
			FreeBlock(currBlock);
			currBlock = next;
		}

		Heap::Destroy();
	}
}
