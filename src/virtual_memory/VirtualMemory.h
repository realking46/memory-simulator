#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <unordered_map>
#include <vector>
#include <queue>
#include "TLB.h"
#include <list>  

// Page Table Entry
struct PageTableEntry {
    bool valid;
    int frame_number;

    PageTableEntry() : valid(false), frame_number(-1) {}

    PageTableEntry(bool v, int f)
        : valid(v), frame_number(f) {}
};

class VirtualMemory {
public:
    // frames = number of physical frames
    VirtualMemory(int frames);

    // Takes virtual address, returns physical address
    int access(int virtual_address);
    void print_stats() const;
    void print_tlb_stats() const;  
    void dump() const;

private:
    static const int PAGE_SIZE = 16; // bytes

    int num_frames;

    // frame_number -> page_number
    std::vector<int> frame_to_page;

    // page_number -> page table entry
    std::unordered_map<int, PageTableEntry> page_table;

    // FIFO page replacement queue
    std::queue<int> fifo_queue;

    std::list<int> lru_frames; // MRU at front

    int page_hits;
    int page_faults;
    TLB tlb;  

    void handle_page_fault(int page_number);

};

#endif
