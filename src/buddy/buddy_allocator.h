#pragma once
#include <map>
#include <vector>
#include <cstddef>
#include <unordered_map>

struct BuddyBlock {
    size_t start;
    size_t size;
    bool free;
    
};

class BuddyAllocator {
public:
    BuddyAllocator();
    BuddyAllocator(size_t size);

    // void init_memory(size_t total_size);
    int malloc_block(size_t size);
    void init(size_t size); // initialize free lists
    bool free_block(size_t address);
    void dump() const;
    void print_stats() const;
    size_t get_used_memory() const { return used_memory; }
    size_t get_total_memory() const { return total_memory; }
    size_t get_failed_allocs() const { return failed_allocs; }
    size_t get_successful_allocs() const { return successful_allocs; }
    

private:
    size_t total_memory;
    size_t min_block_size;
    size_t successful_allocs = 0;
    size_t failed_allocs = 0;
    size_t internal_fragmentation = 0;
    size_t used_memory = 0;
    size_t total_requests = 0;

    std::unordered_map<size_t, std::pair<size_t, size_t>> allocated_blocks;
    // address → {block_size, requested_size}

    // free_lists[size] → list of free blocks of that size
    std::map<size_t, std::vector<BuddyBlock>> free_lists;

    size_t next_power_of_two(size_t n) const;
    size_t get_buddy_address(size_t addr, size_t size) const;
    void split_block(size_t size);
};
