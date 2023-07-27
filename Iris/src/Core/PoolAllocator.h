#ifndef POOL_ALLOCATOR_H

#define POOL_ALLOCATOR_H

#include <list>

class PoolAllocator
{
public:
	PoolAllocator(size_t chunksPerBlock);
	~PoolAllocator();

	PoolAllocator(const PoolAllocator&) = delete;
	PoolAllocator(PoolAllocator&&) = delete;

	PoolAllocator& operator=(const PoolAllocator&) = delete;
	PoolAllocator& operator=(PoolAllocator&&) = delete;

	void transferOwnership(PoolAllocator& other);

	void* allocate(size_t size);
	void deallocate(void* memory);
	void freeAllocatedMemory();

private:
	struct Chunk { Chunk* next; };

private:
	size_t m_ChunksPerBlock;
	Chunk* m_AllocatedMemory;
	std::list<void*> m_AllocatedBlocks;

	void* allocateBlock(size_t size);
};

#endif // !POOL_ALLOCATOR_H
