#ifndef HEAP_H
#define HEAP_H

#include "Mem.h"
#include "DLink.h"

namespace Azul
{
	class Block;

	class Heap
	{
	public:
		// Heap Types
		enum class Type : size_t
		{
			NORMAL,
			FIXED
		};

		struct Info
		{
			static const uint32_t NameSize = 64;

			// Data
			size_t    TotalSize;
			size_t	  PeakNumAlloc;
			size_t	  CurrNumAlloc;
			size_t	  PeakBytesUsed;
			size_t	  CurrBytesUsed;
			char Name[Heap::Info::NameSize];
		};

	public:
		void GetInfo(Info& info) const;

		// Tracking block 
		Block* GetTrackingHead() const;
		Heap::Type GetHeapType() const;

		Heap* GetNextHeap() const;
		Heap* GetPrevHeap() const;

	protected:
		Heap(HANDLE winHeapHandle, unsigned int heapSize, const char* const name, Type hType, Mem* pMem);

		Heap() = delete;
		Heap(const Heap&) = delete;
		Heap& operator = (const Heap& tmp) = delete;
		Heap(Heap&&) = delete;
		Heap& operator = (Heap&&) = delete;
		virtual ~Heap() = default;

		void SetNextHeap(Heap* next);
		void SetPrevHeap(Heap* prev);

		void RemoveBlockFromList(Block* block);

		virtual void* Allocate(size_t size, Azul::Mem::Align align, const char* name, size_t lineNum) = 0;
		virtual void Deallocate(void* ptr) = 0;
		virtual void Destroy();

		// Data:
		Block* pBlkHead;
		Mem* pMem;		// Back link to the memory system
		DLink   hLink;  // Heap Link
		Type	mType;
		HANDLE  mWinHeapHandle;
		Info	mInfo;
	};
}

#endif
