#include "Mem.h"
#include "Block.h"
#include "Heap.h"
#include "HeapFixedSAP.h"
#include "HeapNormalSAP.h"
#include "Registration.h"
#include "HeapSAP.h"

namespace Azul
{
	Mem* Mem::psMem = nullptr;

	Mem::Mem()
		: poHead{ nullptr }
		, pGlobalHead{ nullptr }
		, poRegistration{ new Registration() }
		, mInfo{0}
		, mInitialized{ true }
	{
	}

	Mem::~Mem()
	{
		mInitialized = false;
		while (poHead)
		{
			Mem::RemoveHeap(poHead);
		}

		psMem = nullptr;
		delete poRegistration;
	}

	Mem* Mem::GetInstance()
	{
		return psMem;
	}

	int Mem::GetAlignment(Mem::Align align)
	{
		int alignmentSize = 0;

		switch (align)
		{
		case Mem::Align::Byte_4:
			alignmentSize = 4u;
			break;
		case Mem::Align::Byte_8:
			alignmentSize = 8u;
			break;
		case Mem::Align::Byte_16:
			alignmentSize = 16u;
			break;
		case Mem::Align::Byte_32:
			alignmentSize = 32u;
			break;
		case Mem::Align::Byte_64:
			alignmentSize = 64u;
			break;
		case Mem::Align::Byte_128:
			alignmentSize = 128u;
			break;
		case Mem::Align::Byte_256:
			alignmentSize = 256u;
			break;
		default:
			// TODO
			break;
		}

		return alignmentSize;
	}

	void Mem::SetGlobalHead(Block* block)
	{
		pGlobalHead = block;
	}

	void Mem::RegisterPtr(void* ptr)
	{
		Mem* mem = GetInstance();

		mem->poRegistration->Register(ptr);
	}

	void Mem::UnRegisterPtr(void* ptr)
	{
		Mem* mem = GetInstance();

		mem->poRegistration->UnRegister(ptr);
	}

	Mem::Code Mem::NormalHeap(HeapNormal*& newHeap, size_t heapSize, const char* const Name)
	{
		newHeap = nullptr;

		Mem* mem = GetInstance();
		if (!mem)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		HANDLE heapHandle = HeapCreate(0, heapSize, 0);
		assert(heapHandle);

		void* ptr = HeapAlloc(heapHandle, 0, sizeof(Heap));
		assert(ptr);

		if (ptr)
		{
			newHeap = new(ptr) HeapNormalSAP(heapHandle, heapSize, Name, Heap::Type::NORMAL, psMem);

			if (mem->poHead)
			{
				static_cast<HeapSAP*>(mem->poHead)->SAPSetPrevHeap(newHeap);
				reinterpret_cast<HeapSAP*>(newHeap)->SAPSetNextHeap(mem->poHead);
			}

			mem->poHead = newHeap;

			mem->mInfo.CurrHeapCount++;
			if (mem->mInfo.CurrHeapCount > mem->mInfo.PeakHeapCount)
			{
				mem->mInfo.PeakHeapCount = mem->mInfo.CurrHeapCount;
			}
		}
		else
		{
			return Mem::Code::ERROR_Heap_Create;
		}

		return Code::OK;
	}

	Mem::Code Mem::FixedHeap(HeapFixed*& newHeap, size_t numBlocks, size_t sizePerBlock, const char* const Name)
	{
		Mem* mem = GetInstance();
		if (!mem)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		size_t totalSize = numBlocks * (sizePerBlock + sizeof(Block));
		size_t heapSize = totalSize + sizeof(HeapFixed) + HeapFixed::FreeHeapExtra;

		HANDLE heapHandle = HeapCreate(0, heapSize, 0);
		assert(heapHandle);

		// Allocate memory for the heap header.
		void* ptr = HeapAlloc(heapHandle, 0, sizeof(HeapFixed));
		assert(ptr);

		if (ptr)
		{
			// Initialize the heap header at the newly allocated memory address.
			newHeap = new(ptr) HeapFixedSAP(heapHandle, heapSize, Name, Heap::Type::FIXED, psMem);

			// Allocate and subdivide the rest of the heap into fixed blocks.
			static_cast<HeapFixedSAP*>(newHeap)->SAPHeapFormat(numBlocks, sizePerBlock);

			if (mem->poHead)
			{
				static_cast<HeapSAP*>(mem->poHead)->SAPSetPrevHeap(newHeap);
				reinterpret_cast<HeapSAP*>(newHeap)->SAPSetNextHeap(mem->poHead);
			}

			mem->poHead = newHeap;

			mem->mInfo.CurrHeapCount++;
			if (mem->mInfo.CurrHeapCount > mem->mInfo.PeakHeapCount)
			{
				mem->mInfo.PeakHeapCount = mem->mInfo.CurrHeapCount;
			}
		}
		else
		{
			return Mem::Code::ERROR_Heap_Create;
		}

		return Code::OK;
	}

	// Initialize the memory system  (Public method)
	Mem::Code Mem::Create(void)
	{
		if (!psMem)
		{
			psMem = new Mem();
		}

		return Code::OK;
	}

	Mem::Code Mem::Destroy(void)
	{
		if (!psMem)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		delete psMem;

		return Code::OK;
	}

	Mem::Code Mem::GetInfo(Mem::Info& retInfo)
	{
		Mem* mem = GetInstance();
		if (!mem)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		retInfo = mem->mInfo;
		return Code::OK;
	}

	Block* Mem::DebugGetGlobalTrackingHead()
	{
		Mem* mem = GetInstance();

		return mem->pGlobalHead;
	}

	Mem::Code Mem::RemoveHeap(Heap* pInHeap)
	{
		Mem* mem = GetInstance();

		if (!mem)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		if (!pInHeap)
		{
			return Code::ERROR_Illegal_Heap;
		}

		Heap* currHeap = mem->poHead;
		while (currHeap && currHeap != pInHeap)
		{
			currHeap = currHeap->GetNextHeap();
		}

		if (!currHeap)
		{
			return Code::ERROR_Illegal_Heap;
		}

		if (pInHeap->GetNextHeap())
		{
			static_cast<HeapSAP*>(pInHeap->GetNextHeap())->SAPSetPrevHeap(pInHeap->GetPrevHeap());
		}

		// If input heap is the head, its previous link should be null.
		// So, we set the next heap as the head.
		// If it's not the head, we make the prev heap point to the next heap of the input heap.
		if (pInHeap == mem->poHead)
		{
			mem->poHead = mem->poHead->GetNextHeap();
		}
		else
		{
			static_cast<HeapSAP*>(pInHeap->GetPrevHeap())->SAPSetNextHeap(pInHeap->GetNextHeap());
		}

		static_cast<HeapSAP*>(pInHeap)->SAPDestroyHeap();

		return Code::OK;
	}

	Mem::Code Mem::GetHeapByAddr(Heap*& pHeap, void* p)
	{
		Mem* mem = GetInstance();

		Mem::Code status = Mem::Code::ERROR_Mem_Not_Initialized;
		if (!mem)
		{
			return status;
		}

		if (mem->poRegistration->Find(p))
		{
			Block* secretPtr = *reinterpret_cast<Block**>(reinterpret_cast<size_t>(p) - sizeof(Block*));
			pHeap = secretPtr->GetHeap();
			status = Mem::Code::OK;
		}
		else
		{
			status = Mem::Code::ERROR_Invalid_Addr;
		}

		return status;
	}
}
