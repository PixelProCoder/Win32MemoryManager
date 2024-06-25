#include "Block.h"

namespace Azul
{
	Block::Block(Heap* heap, size_t size, const char* inName, const size_t lineNum)
		: pHeap{ heap }
		, pUserBlock{ nullptr }
		, mLineNum{ lineNum }
		, mAllocIndex{ 0 }
		, mAllocSize{ size }
		, mFileName{}
		, pSecret{ this }
	{
		const char* sep = strrchr(inName, '\\');
		const char* filename = sep + 1;
		strcpy_s(mFileName, Block::NameSize, filename);
	}

	Block* Block::GetHeapNext() const
	{
		return reinterpret_cast<Block*>(bLink.pNext);
	}

	Block* Block::GetHeapPrev() const
	{
		return reinterpret_cast<Block*>(bLink.pPrev);
	}

	Block* Block::GetGlobalNext() const
	{
		return reinterpret_cast<Block*>(gLink.pNext);
	}

	Block* Block::GetGlobalPrev() const
	{
		return reinterpret_cast<Block*>(gLink.pPrev);
	}

	const char* Block::GetFileName() const
	{
		return mFileName;
	}

	size_t Block::GetLineNum() const
	{
		return mLineNum;
	}

	size_t Block::GetAllocIndex() const
	{
		return mAllocIndex;
	}

	size_t Block::GetAllocSize() const
	{
		return mAllocSize;
	}

	Heap* Block::GetHeap() const
	{
		return pHeap;
	}

	void* Block::GetUserBlock() const
	{
		return pUserBlock;
	}

	void Block::SetHeapNext(Block* block)
	{
		bLink.pNext = reinterpret_cast<DLink*>(block);
	}

	void Block::SetHeapPrev(Block* block)
	{
		bLink.pPrev = reinterpret_cast<DLink*>(block);
	}

	void Block::SetGlobalNext(Block* block)
	{
		gLink.pNext = reinterpret_cast<DLink*>(block);
	}

	void Block::SetGlobalPrev(Block* block)
	{
		gLink.pPrev = reinterpret_cast<DLink*>(block);
	}

	void Block::SetAllocIndex(size_t index)
	{
		mAllocIndex = index;
	}

	void Block::SetAllocSize(size_t size)
	{
		mAllocSize = size;
	}

	void Block::SetHeap(Heap* heap)
	{
		pHeap = heap;
	}

	void Block::SetUserBlock(void* ptr)
	{
		pUserBlock = ptr;
	}
}
