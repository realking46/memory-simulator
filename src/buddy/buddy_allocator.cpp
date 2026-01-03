#include "buddy_allocator.h"
#include <iostream>
#include <algorithm>
using namespace std;

BuddyAllocator::BuddyAllocator()
    : total_memory(0), min_block_size(4) {}

// constructor with size
BuddyAllocator::BuddyAllocator(size_t size)
    : total_memory(size), min_block_size(4) {
    init(size);
}

void BuddyAllocator::init(size_t size) {
    total_memory = size;
    min_block_size = 8;

    free_lists.clear();
    allocated_blocks.clear();

    successful_allocs = 0;
    failed_allocs = 0;
    internal_fragmentation = 0;
    used_memory = 0;
    total_requests = 0;

    // Entire memory is one free block initially
    BuddyBlock block {0, size, true};
    free_lists[size].push_back(block);
}
size_t BuddyAllocator::next_power_of_two(size_t n) const {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}
size_t BuddyAllocator::get_buddy_address(size_t addr, size_t size) const {
    return addr ^ size;
}
void BuddyAllocator::split_block(size_t size) {
    size_t larger = size << 1;

    if (larger > total_memory || free_lists[larger].empty())
        return;

    BuddyBlock block = free_lists[larger].back();
    free_lists[larger].pop_back();

    BuddyBlock left  {block.start, size, true};
    BuddyBlock right {block.start + size, size, true};

    free_lists[size].push_back(left);
    free_lists[size].push_back(right);
}
int BuddyAllocator::malloc_block(size_t request_size) {

    total_requests++;

    size_t block_size = max(next_power_of_two(request_size), min_block_size);


    if (block_size > total_memory) {
        failed_allocs++;
        return -1;
    }

    if (free_lists[block_size].empty()) {
        size_t bigger = block_size << 1;
        while (bigger <= total_memory && free_lists[bigger].empty())
            bigger <<= 1;

        if (bigger > total_memory) {
            failed_allocs++;
            return -1;
        }

        while (bigger > block_size) {
            split_block(bigger >> 1);
            bigger >>= 1;
        }
    }

    BuddyBlock block = free_lists[block_size].back();
    free_lists[block_size].pop_back();

    allocated_blocks[block.start] = {block_size, request_size};


    successful_allocs++;
    used_memory += block_size;
    internal_fragmentation += (block_size - request_size);

    return block.start;
}

bool BuddyAllocator::free_block(size_t address) {

    if (allocated_blocks.find(address) == allocated_blocks.end())
        return false;

    auto [size, request_size] = allocated_blocks[address];
    allocated_blocks.erase(address);

    used_memory -= size;
    internal_fragmentation -= (size - request_size);


    while (size < total_memory) {
        size_t buddy_addr = get_buddy_address(address, size);

        auto &list = free_lists[size];
        auto it = find_if(list.begin(), list.end(),
            [&](const BuddyBlock &b) {
                return b.start == buddy_addr && b.free;
            });

        if (it == list.end())
            break;

        list.erase(it);
        address = min(address, buddy_addr);
        size <<= 1;
    }

    BuddyBlock merged {address, size, true};
    free_lists[size].push_back(merged);
    return true;
}

void BuddyAllocator::dump() const {
    cout << "Buddy Free Lists:\n";
    for (const auto &[size, list] : free_lists) {
        cout << "Size " << size << " : ";
        auto sorted = list;
        sort(sorted.begin(), sorted.end(),
            [](auto &a, auto &b) { return a.start < b.start; });

        for (auto &b : sorted)
            cout << "[" << b.start << "] ";

        cout << "\n";
    }
}

void BuddyAllocator::print_stats() const {
    cout << "Allocation requests: " << total_requests << "\n";
    cout << "Successful allocations: " << successful_allocs << "\n";
    cout << "Failed allocations: " << failed_allocs << "\n";

    if (total_requests > 0) {
        double failure_rate =
            (double)failed_allocs / total_requests * 100.0;
        cout << "Failure rate: " << failure_rate << "%\n";
    }

    double utilization =
        (double)used_memory / total_memory * 100.0;

    cout << "Used memory: " << used_memory << " bytes\n";
    cout << "Internal fragmentation: "
         << internal_fragmentation << " bytes\n";
    cout << "External fragmentation: 0%\n";
    cout << "Utilization: " << utilization << "%\n";
}