#include "PoolAllocator.h"

#include <cassert>

PoolAllocator::PoolAllocator(size_t chunksPerBlock)
    : m_ChunksPerBlock(chunksPerBlock), m_AllocatedMemory(nullptr)
{
    assert(chunksPerBlock > 0);
}

PoolAllocator::~PoolAllocator()
{
    freeAllocatedMemory();
}

void PoolAllocator::transferOwnership(PoolAllocator& other)
{
    Chunk* chunk = other.m_AllocatedMemory;
    while (chunk->next)
        chunk = chunk->next;

    chunk->next = m_AllocatedMemory;
    m_AllocatedMemory = nullptr;

    other.m_AllocatedBlocks.splice(other.m_AllocatedBlocks.end(), m_AllocatedBlocks);
}

void* PoolAllocator::allocate(size_t size)
{
    if (m_AllocatedMemory == nullptr)
        m_AllocatedMemory = reinterpret_cast<Chunk*>(allocateBlock(size));

    Chunk* memoryToAllocate = m_AllocatedMemory;
    m_AllocatedMemory = m_AllocatedMemory->next;

    return reinterpret_cast<void*>(memoryToAllocate);
}

void PoolAllocator::deallocate(void* memory)
{
    Chunk* allocatedMemory = reinterpret_cast<Chunk*>(memory);
    allocatedMemory->next = m_AllocatedMemory;
    m_AllocatedMemory = allocatedMemory;
}

void PoolAllocator::freeAllocatedMemory()
{
    for (void* block : m_AllocatedBlocks)
        ::operator delete(block);

    m_AllocatedMemory = nullptr;
    m_AllocatedBlocks.clear();
}

void* PoolAllocator::allocateBlock(size_t size)
{
    size_t blockSize = m_ChunksPerBlock * size;

    void* startOfBlock = ::operator new(blockSize);
    Chunk* allocatedMemory = reinterpret_cast<Chunk*>(startOfBlock);

    for (int i = 0; i < m_ChunksPerBlock - 1; i++)
    {
        char* endOfBlock = reinterpret_cast<char*>(allocatedMemory) + size;
        allocatedMemory->next = reinterpret_cast<Chunk*>(endOfBlock);
        allocatedMemory = allocatedMemory->next;
    }

    allocatedMemory->next = nullptr;

    m_AllocatedBlocks.push_back(startOfBlock);
    return startOfBlock;
}
