#include "memory_manager.h"
#include <iostream>
#include <iomanip>
using namespace std;

MemoryManager::MemoryManager():
      total_memory(0),
      next_block_id(1),
      allocator_type(AllocatorType::FIRST_FIT),
      total_requests(0),
      successful_allocs(0),
      failed_allocs(0),
      buddy(0) 
{}

void MemoryManager::init_memory(size_t total_size) {
    blocks.clear();
    total_memory = total_size;

    Block initial;
    initial.start = 0;
    initial.size = total_size;
    initial.free = true;
    initial.block_id = -1;
    total_requests = 0;
    successful_allocs = 0;
    failed_allocs = 0;
    next_block_id = 1;

    buddy.init(total_size);
    blocks.push_back(initial);
}

void MemoryManager::set_allocator(AllocatorType type) {
    allocator_type = type;
}

void MemoryManager::dump_memory() const {
    cout << "Allocator = " << (int)allocator_type << "\n";

    if (allocator_type == AllocatorType::BUDDY) {
        size_t used = buddy.get_used_memory();
        size_t total = buddy.get_total_memory();

        cout << "Allocation requests: "
            << buddy.get_successful_allocs() + buddy.get_failed_allocs() << "\n";
        cout << "Successful allocations: "
            << buddy.get_successful_allocs() << "\n";
        cout << "Failed allocations: "
            << buddy.get_failed_allocs() << "\n";

        cout << "Total memory: " << total << " bytes\n";
        cout << "Used memory: " << used << " bytes\n";
        cout << "Free memory: " << (total - used) << " bytes\n";

        double util = (double)used / total * 100.0;
        cout << "Utilization: " << util << "%\n";
        return;
    }

    size_t free_blocks = 0;
    size_t used_blocks = 0;
    size_t largest_free = 0;

    cout << "===== Memory Layout =====\n";

    for (const auto& block : blocks) {
        size_t end = block.start + block.size - 1;

        cout << "[0x"
             << setw(4) << setfill('0') << hex << block.start
             << " - 0x"
             << setw(4) << setfill('0') << hex << end
             << "] ";

        cout << dec;

        if (block.free) {
            cout << "FREE   ";
            free_blocks++;
            largest_free = max(largest_free, block.size);
        } else {
            cout << "USED   id=" << block.block_id << " ";
            used_blocks++;
        }

        cout << "size=" << block.size << " bytes\n";
    }

    cout << "-----------------------------\n";
    cout << "Total blocks: " << blocks.size() << "\n";
    cout << "Used blocks : " << used_blocks << "\n";
    cout << "Free blocks : " << free_blocks << "\n";
    cout << "Largest free block: " << largest_free << " bytes\n";
}

bool MemoryManager::free_block(int block_id) {
    if (allocator_type == AllocatorType::BUDDY) {
        bool success = buddy.free_block(block_id);
        return success;
    }
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {

        if (!it->free && it->block_id == block_id) {

            // Step 1: mark as free
            it->free = true;
            it->block_id = -1;

            // Step 2: coalesce with previous
            if (it != blocks.begin()) {
                auto prev = std::prev(it);
                if (prev->free) {
                    prev->size += it->size;
                    it = blocks.erase(it);
                    it = prev;
                }
            }

            // Step 3: coalesce with next
            auto next = std::next(it);
            if (next != blocks.end() && next->free) {
                it->size += next->size;
                blocks.erase(next);
            }

            return true;
        }
    }

    return false; // block id not found
}

int MemoryManager::malloc_block(size_t size) {

    auto selected = blocks.end();
    total_requests++;

    if (allocator_type == AllocatorType::BUDDY) {
            int addr = buddy.malloc_block(size);
            if (addr == -1) {
                failed_allocs++;
                return -1;
            } else {
                successful_allocs++;
                return addr; // in Buddy, addr acts as block id
            }
        }

    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        if (!it->free || it->size < size)
            continue;

        if (allocator_type == AllocatorType::FIRST_FIT) {
            selected = it;
            break;
        }
        
        if (selected == blocks.end()) {
            selected = it;
        } else if (allocator_type == AllocatorType::BEST_FIT &&
                   it->size < selected->size) {
            selected = it;
        } else if (allocator_type == AllocatorType::WORST_FIT &&
                   it->size > selected->size) {
            selected = it;
        }
    }

    if (selected == blocks.end()){
        failed_allocs++;
        return -1;
    }
        
    int allocated_id = next_block_id++;

    if (selected->size == size) {
        selected->free = false;
        selected->block_id = allocated_id;
        selected->requested = size;  
    } else {
        Block allocated {
            selected->start,
            size,
            size,
            false,
            allocated_id
        };

        selected->start += size;
        selected->size -= size;

        blocks.insert(selected, allocated);
    }

    successful_allocs++;
    return allocated_id;
}

void MemoryManager::print_stats() const {

    if (allocator_type == AllocatorType::BUDDY) {
        buddy.print_stats();

        return;  
    }
    
    size_t used = 0;
    size_t free = 0;
    size_t largest_free = 0;
    size_t internal_frag = 0;

    cout << "Allocation requests: " << total_requests << "\n";
    cout << "Successful allocations: " << successful_allocs << "\n";
    cout << "Failed allocations: " << failed_allocs << "\n";

    if (total_requests > 0) {
        double success_rate =
            (double)successful_allocs / total_requests * 100.0;
        cout << "Allocation success rate: "
                << success_rate << "%\n";
    }

    for (const auto& block : blocks) {
        if (block.free) {
            free += block.size;
            if (block.size > largest_free)
                largest_free = block.size;
        } else {
            used += block.size;
            internal_frag += (block.size - block.requested);
            cout << "Internal fragmentation: "
            << internal_frag << " bytes\n";

        }
    }

    double failure_rate = 0;
    if (total_requests > 0) {
        failure_rate =
            (double)failed_allocs / total_requests * 100.0;
    }

    cout << "Allocation failure rate: "
        << failure_rate << "%\n";


    double utilization = (double)used / total_memory * 100.0;

    double external_fragmentation = 0.0;
    if (free > 0) {
        external_fragmentation =
            (1.0 - (double)largest_free / free) * 100.0;
    }

    cout << "Total memory: " << total_memory << " bytes\n";
    cout << "Used memory: " << used << " bytes\n";
    cout << "Free memory: " << free << " bytes\n";
    cout << "Utilization: " << utilization << "%\n";
    cout << "External fragmentation: "
              << external_fragmentation << "%\n";
}
