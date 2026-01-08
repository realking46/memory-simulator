#pragma once
#include "../buddy/buddy_allocator.h" 
#include <list>
#include <cstddef>
using namespace std;

struct Block {
    size_t start;
    size_t size;
    size_t requested;   
    bool free;
    int block_id;
};

enum class AllocatorType {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    BUDDY 
};

class MemoryManager {
public:
    MemoryManager();

    void init_memory(size_t total_size);
    void set_allocator(AllocatorType type);

    int malloc_block(size_t size);
    bool free_block(int block_id);

    void dump_memory() const;
    void print_stats() const;

private:
    size_t total_memory;
    int next_block_id;
    AllocatorType allocator_type;
    list<Block> blocks;
    size_t total_requests;
    size_t successful_allocs;
    size_t failed_allocs;
    BuddyAllocator buddy;

};


