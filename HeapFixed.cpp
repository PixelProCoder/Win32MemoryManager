#include "HeapFixed.h"
#include "Block.h"
#include "BlockSAP.h"
#include "Registration.h"
#include "MemSAP.h"

namespace Azul
{
	HeapFixed::HeapFixed(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem)
		: Heap(winHeapHandle, heapSize, name, hType, pMem)
		, heapOffset{ nullptr }
		, pFixedFreeHead{ nullptr }
		, pOverflow{ nullptr }
	{
	}

	void* HeapFixed::Allocate(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum)
	{
		Block* freeBlock = pFixedFreeHead;
		if (!freeBlock)
		{
			// No more free blocks.
			return freeBlock;
		}

		pFixedFreeHead = freeBlock->GetHeapNext();

		// BlockSAP is derived from Block and has no data members.
		BlockSAP* blockHdr = new(freeBlock) BlockSAP(this, size, name, lineNum);

		int alignmentSize = Mem::GetAlignment(align);

		const size_t offset = reinterpret_cast<size_t>(blockHdr + 1);
		const int padding = -static_cast<int>(offset) & (alignmentSize - 1);
		const size_t aligned = offset + padding;

		if (padding > 0)
		{
			Block** secretPtr = reinterpret_cast<Block**>(aligned - sizeof(Block*));
			*secretPtr = blockHdr;
		}

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

		// Update heap info
		mInfo.CurrNumAlloc++;
		if (mInfo.CurrNumAlloc > mInfo.PeakNumAlloc)
		{
			mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
		}

		mInfo.CurrBytesUsed += size;

		if (mInfo.CurrBytesUsed > mInfo.PeakBytesUsed)
		{
			mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
		}

		blockHdr->SAPSetAllocIndex(info.CurrAllocIndex);

		if (pBlkHead)
		{
			// Add the new block to the front of the tracking list
			static_cast<BlockSAP*>(pBlkHead)->SAPSetHeapPrev(blockHdr);
			blockHdr->SAPSetHeapNext(pBlkHead);
		}

		pBlkHead = blockHdr;

		BlockSAP* globalHead = static_cast<BlockSAP*>(Mem::DebugGetGlobalTrackingHead());
		if (globalHead)
		{
			// Add the new block to the front of the tracking list
			globalHead->SAPSetGlobalPrev(blockHdr);
			blockHdr->SAPSetGlobalNext(globalHead);
		}

		static_cast<MemSAP*>(pMem)->SAPSetGlobalHead(blockHdr);

		// Add the aligned address to the tracking list.
		void* alignedPtr = reinterpret_cast<void*>(aligned);
		static_cast<MemSAP*>(pMem)->SAPRegisterPtr(alignedPtr);

		blockHdr->SAPSetUserBlock(alignedPtr);

		return alignedPtr;
	}

	void HeapFixed::Deallocate(void* ptr)
	{
		BlockSAP* blockHdr = *reinterpret_cast<BlockSAP**>(reinterpret_cast<size_t>(ptr) - sizeof(Block*));

		RemoveBlockFromList(blockHdr);

		// Update the free list
		if (pFixedFreeHead)
		{
			static_cast<BlockSAP*>(pFixedFreeHead)->SAPSetHeapPrev(blockHdr);
		}

		blockHdr->SAPSetHeapNext(pFixedFreeHead);
		//blockHdr->SAPSetHeapPrev(nullptr);

		pFixedFreeHead = blockHdr;

		// Remove pointer registration
		static_cast<MemSAP*>(pMem)->SAPUnRegisterPtr(ptr);

		// Update mem info
		Mem::Info info;
		Mem::GetInfo(info);

		info.CurrNumAlloc--;
		info.CurrBytesUsed -= blockHdr->GetAllocSize();

		static_cast<MemSAP*>(pMem)->SAPSetInfo(info);

		mInfo.CurrNumAlloc--;
		mInfo.CurrBytesUsed -= blockHdr->GetAllocSize();
	}

	void HeapFixed::SetOverflowHeap(Heap* pHeap)
	{
		pOverflow = pHeap;
	}

	Heap* HeapFixed::GetOverflowHeap() const
	{
		return pOverflow;
	}

	void HeapFixed::Destroy()
	{
		HeapFree(mWinHeapHandle, 0, heapOffset);

		Mem::Info info;
		Mem::GetInfo(info);

		info.CurrNumAlloc -= mInfo.CurrNumAlloc;
		info.CurrBytesUsed -= mInfo.CurrBytesUsed;

		static_cast<MemSAP*>(pMem)->SAPSetInfo(info);

		Heap::Destroy();
	}

	void HeapFixed::FormatHeap(size_t numBlocks, size_t sizePerBlock)
	{
		size_t totalSize = numBlocks * (sizePerBlock + sizeof(Block));

		// Allocate memory for the fixed-sized blocks
		void* ptr = HeapAlloc(mWinHeapHandle, 0, totalSize);
		assert(ptr);

		heapOffset = pFixedFreeHead = reinterpret_cast<Block*>(ptr);

		Block* curr = pFixedFreeHead;
		Block* next = nullptr;

		// Subdivide allocated memory into blocks and link them.
		for (unsigned int i = 0; i < numBlocks - 1; ++i)
		{
			next = reinterpret_cast<Block*>(reinterpret_cast<size_t>((curr + 1)) + sizePerBlock);
			static_cast<BlockSAP*>(curr)->SAPSetHeapNext(next);
			curr = next;
		}

		// Set the next linkage of last block to null
		static_cast<BlockSAP*>(curr)->SAPSetHeapNext(nullptr);
	}
}
