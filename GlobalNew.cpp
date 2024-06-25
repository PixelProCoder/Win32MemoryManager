#include "Mem.h"
#include "GlobalNew.h"
#include "HeapNormalSAP.h"
#include "HeapFixedSAP.h"

// --------------------------------------------
// For Normal Heap
// --------------------------------------------

void *operator new(size_t inSize, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum)
{
	void* ptr = static_cast<Azul::HeapNormalSAP*>(pHeap)->SAPHeapAlloc(inSize, align, inName, lineNum);
	return ptr;
}

void operator delete(void *p, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum)
{
	AZUL_REPLACE_ME(p);
	AZUL_REPLACE_ME(pHeap);
	AZUL_REPLACE_ME(align);
	AZUL_REPLACE_ME(inName);
	AZUL_REPLACE_ME(lineNum);
}

void *operator new[](size_t inSize, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum)
{
	void* ptr = static_cast<Azul::HeapNormalSAP*>(pHeap)->SAPHeapAlloc(inSize, align, inName, lineNum);
	return ptr;
}

void operator delete[](void *p, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum)
{
	AZUL_REPLACE_ME(p);
	AZUL_REPLACE_ME(pHeap);
	AZUL_REPLACE_ME(align);
	AZUL_REPLACE_ME(inName);
	AZUL_REPLACE_ME(lineNum);
}

// --------------------------------------------
// For Fixed Heap
// --------------------------------------------

void *operator new(size_t inSize, Azul::HeapFixed *const pHeap, const char *inName, size_t lineNum)
{
	void* ptr = static_cast<Azul::HeapFixedSAP*>(pHeap)->SAPHeapAlloc(inSize, Azul::Mem::Align::Byte_4, inName, lineNum);
	if (!ptr)
	{
		Azul::HeapNormalSAP* heap = static_cast<Azul::HeapNormalSAP*>(pHeap->GetOverflowHeap());
		if (heap)
		{
			ptr = heap->SAPHeapAlloc(inSize, Azul::Mem::Align::Byte_4, inName, lineNum);
		}
	}

	return ptr;
}

void operator delete(void *p, Azul::HeapFixed *const pHeap, const char *inName, size_t lineNum)
{
	AZUL_REPLACE_ME(p);
	AZUL_REPLACE_ME(pHeap);
	AZUL_REPLACE_ME(inName);
	AZUL_REPLACE_ME(lineNum);
}

// --------------------------------------------
// For Global Standard new/delete
// --------------------------------------------

void *operator new(size_t inSize)
{
	void *p = malloc(inSize);
	return p;
}

void operator delete(void *p)
{
	Azul::Heap* heap = nullptr;
	Azul::Mem::GetHeapByAddr(heap, p);

	if (heap)
	{
		// If heap isn't null, then it's a custom allocated block.
		// Call the appropriate free method.
		if (heap->GetHeapType() == Azul::Heap::Type::NORMAL)
		{
			static_cast<Azul::HeapNormalSAP*>(heap)->SAPHeapFree(p);
		}
		else if (heap->GetHeapType() == Azul::Heap::Type::FIXED)
		{
			static_cast<Azul::HeapFixedSAP*>(heap)->SAPHeapFree(p);
		}
	}
	else
	{
		// Otherwise, it's a globally allocated block.
		free(p);
	}
}

void *operator new[](size_t inSize)
{
	void *p = malloc(inSize);
	return p;
}

void operator delete[](void *p)
{
	Azul::Heap* heap = nullptr;
	Azul::Mem::GetHeapByAddr(heap, p);

	if (heap)
	{
		// If heap isn't null, then it's a custom allocated block.
		// Call the appropriate free method.
		if (heap->GetHeapType() == Azul::Heap::Type::NORMAL)
		{
			static_cast<Azul::HeapNormalSAP*>(heap)->SAPHeapFree(p);
		}
		else if (heap->GetHeapType() == Azul::Heap::Type::FIXED)
		{
			static_cast<Azul::HeapFixedSAP*>(heap)->SAPHeapFree(p);
		}
	}
	else
	{
		// Otherwise, it's a globally allocated block.
		free(p);
	}
}
