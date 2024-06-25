#ifndef MEM_H
#define MEM_H

namespace Azul
{
	class Heap;
	class HeapFixed;
	class HeapNormal;
	class Block;
	class Registration;

	// Singleton
	class Mem
	{
	public:
		static const uint32_t MAJOR_VERSION = 7;
		static const uint32_t MINOR_VERSION = 0;

		// Alignment 
		enum class Align : uint32_t
		{
			Byte_4,
			Byte_8,
			Byte_16,
			Byte_32,
			Byte_64,
			Byte_128,
			Byte_256,
			Default = Byte_4
		};

		// Return codes
		enum class Code : uint32_t
		{
			OK,
			ERROR_Heap_Create,
			ERROR_Illegal_Heap,
			ERROR_Mem_Not_Initialized,
			ERROR_Invalid_Addr,
		};

		struct Info
		{
			size_t CurrAllocIndex;
			size_t PeakHeapCount;
			size_t CurrHeapCount;
			size_t PeakNumAlloc;
			size_t CurrNumAlloc;
			size_t PeakBytesUsed;
			size_t CurrBytesUsed;
		};

	public:

		// Initialize the mem system
		static Code Create();
		static Code Destroy();

		// Create an Heap from the heap system
		static Code NormalHeap(HeapNormal*& newHeap, size_t heapSize, const char* const Name);
		static Code FixedHeap(HeapFixed*& newHeap, size_t numBlocks, size_t sizePerBlock, const char* const Name);

		// Create an Heap from the heap system
		static Code RemoveHeap(Heap* inHeap);

		// Information
		static Code GetInfo(Info& info);
		static Code GetHeapByAddr(Heap*& pHeap, void* p);
		static Block* DebugGetGlobalTrackingHead(void);

		static Mem* GetInstance();
		static int GetAlignment(Mem::Align align);

	protected:
		Mem();
		Mem(const Mem& in) = delete;
		Mem& operator = (const Mem& in) = delete;
		Mem(Mem&&) = delete;
		Mem& operator = (Mem&&) = delete;
		~Mem();

		void SetGlobalHead(Block* block);
		void RegisterPtr(void* ptr);
		void UnRegisterPtr(void* ptr);

		static Mem* psMem;

		Heap* poHead;
		Block* pGlobalHead;
		Registration* poRegistration;  // Pointer registration
		Info mInfo;
		bool mInitialized;
	};
}

#endif
