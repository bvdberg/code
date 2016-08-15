#include "Context.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>  // for printf

struct MemBlock {
    uint8_t* memory;
    uint8_t* end;
    uint8_t* ptr;
    MemBlock* next;

    MemBlock(size_t size) : next(0) {
        memory = (uint8_t*)malloc(size);
        end = memory + size;
        ptr = memory;
    }
    ~MemBlock() {
        free(memory);
    }
};

Context::Context(uint32_t blockSize_)
    : blocks(0)
    , lastBlock(0)
    , blockSize(blockSize_)
{
    allocateMemory();
}

Context::~Context() {
    MemBlock* block = blocks;
    while (block) {
        MemBlock* next = block->next;
        delete block;
        block = next;
    }
}

void Context::dump() const {
    MemBlock* cur = blocks;
    unsigned numBlocks = 0;
    while (cur) {
        cur = cur->next;
        numBlocks++;
    }
    printf("Context: allocated %d blocks x %d Kb (%d Kb total)\n", numBlocks, BLOCK_SIZE / 1024, numBlocks * BLOCK_SIZE / 1024);
}

size_t Context::getAllocatedMemory() const {
    MemBlock* cur = blocks;
    size_t total = 0;
    while (cur) {
        cur = cur->next;
        total += BLOCK_SIZE;
    }
    return total;
}

void Context::allocateMemory() const {
    MemBlock* newBlock = new MemBlock(BLOCK_SIZE);
    if (lastBlock) {
        lastBlock->next = newBlock;
    } else {
        blocks = newBlock;
    }
    lastBlock = newBlock;
}

void* Context::Allocate(size_t Size, unsigned Align) const {
    assert(Size <= BLOCK_SIZE);
    // TODO handle Align
    size_t avail = lastBlock->end - lastBlock->ptr;
    printf("Allocate %lu / %lu\n", Size, avail);
    if (avail < Size) {
        printf("no more space, creating new MemBlock\n");
        allocateMemory();
    }
    void* result = lastBlock->ptr;
    lastBlock->ptr += Size;
    return result;
}

