#ifndef BLOCK_H
#define BLOCK_H

#include "DLink.h"

namespace Azul
{
	class Heap;

	class Block
	{
	public:
		static const uint32_t NameSize = 64;

		Block* GetHeapNext() const;
		Block* GetHeapPrev() const;
		Block* GetGlobalNext() const;
		Block* GetGlobalPrev() const;
		const char* GetFileName() const;
		size_t GetLineNum() const;
		size_t GetAllocIndex() const;
		size_t GetAllocSize() const;
		Heap* GetHeap() const;
		void* GetUserBlock() const;

	protected:
		Block(Heap* heap, size_t size, const char* inName, const size_t lineNum);

		Block() = delete;
		Block(const Block&) = delete;
		Block& operator = (const Block&) = delete;
		Block(Block&&) = delete;
		Block& operator = (Block&&) = delete;
		~Block() = default;

		void SetHeapNext(Block* block);
		void SetHeapPrev(Block* block);
		void SetGlobalNext(Block* block);
		void SetGlobalPrev(Block* block);
		void SetAllocIndex(size_t index);
		void SetAllocSize(size_t size);
		void SetHeap(Heap* heap);
		void SetUserBlock(void* ptr);

	private:
		// Tracking links 
		DLink bLink;   // Block links
		DLink gLink;   // Global links

		Heap* pHeap;
		void* pUserBlock;

		size_t	mLineNum;
		size_t	mAllocIndex;
		size_t	mAllocSize;

		char	mFileName[Block::NameSize];

		// Back pointer to the top of the trackingblock
		Block* pSecret;   
	};
}

#endif
