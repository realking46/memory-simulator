#include "VirtualMemory.h"
#include <iostream>

using namespace std;

VirtualMemory::VirtualMemory(int frames)
    : num_frames(frames),
      page_hits(0),
      page_faults(0),
      tlb(4) { 

    frame_to_page.resize(num_frames, -1);
}

int VirtualMemory::access(int virtual_address) {
    int page_number = virtual_address / PAGE_SIZE;
    int offset = virtual_address % PAGE_SIZE;

    int frame;

    //  TLB lookup
    if (tlb.lookup(page_number, frame)) {
        page_hits++;
        return frame * PAGE_SIZE + offset;
    }

    //  Page table lookup
    if (page_table.count(page_number) && page_table[page_number].valid) {
        page_hits++;
        frame = page_table[page_number].frame_number;
        tlb.insert(page_number, frame);

        // Update LRU for frame
        lru_frames.remove(frame);
        lru_frames.push_front(frame);

        return frame * PAGE_SIZE + offset;
    }

    //  Page fault
    page_faults++;
    handle_page_fault(page_number);

    frame = page_table[page_number].frame_number;
    tlb.insert(page_number, frame);

    return frame * PAGE_SIZE + offset;
}

void VirtualMemory::dump() const {
    std::cout << "===== Page Table =====\n";
    for (const auto& [page, entry] : page_table) {
        std::cout << "Page " << page << ": ";
        if (entry.valid)
            std::cout << "VALID  -> Frame " << entry.frame_number;
        else
            std::cout << "INVALID";
        std::cout << "\n";
    }

    std::cout << "\n===== Frame Table =====\n";
    for (int i = 0; i < num_frames; i++) {
        std::cout << "Frame " << i << ": ";
        if (frame_to_page[i] != -1)
            std::cout << "Page " << frame_to_page[i];
        else
            std::cout << "FREE";
        std::cout << "\n";
    }
}

void VirtualMemory::handle_page_fault(int page_number) {
    int frame = -1;

    // Find free frame
    for (int i = 0; i < num_frames; i++) {
        if (frame_to_page[i] == -1) {
            frame = i;
            break;
        }
    }

    // No free frame → LRU eviction
    if (frame == -1) {
        int victim_frame = lru_frames.back();
        lru_frames.pop_back();

        int victim_page = frame_to_page[victim_frame];
        page_table[victim_page].valid = false;
        frame = victim_frame;
    }

    // Load new page
    page_table[page_number] = PageTableEntry(true, frame);
    frame_to_page[frame] = page_number;

    // Update LRU
    lru_frames.push_front(frame);
}

void VirtualMemory::print_stats() const {
    std::cout << "Page hits: " << page_hits << "\n";
    std::cout << "Page faults: " << page_faults << "\n";

    int total = page_hits + page_faults;
    if (total > 0) {
        double rate = (double)page_faults / total * 100.0;
        std::cout << "Fault rate: " << rate << "%\n";
    }
}

void VirtualMemory::print_tlb_stats() const {
    tlb.print_stats();
}
