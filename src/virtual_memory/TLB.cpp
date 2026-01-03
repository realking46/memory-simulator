#include "TLB.h"
#include <iostream>

TLB::TLB(int size)
    : capacity(size), hits(0), misses(0) {}

bool TLB::lookup(int page_number, int &frame_number) {
    auto it = table.find(page_number);
    if (it != table.end()) {
        hits++;
        frame_number = it->second.first;

        // Move to front of LRU
        lru_list.erase(it->second.second);
        lru_list.push_front(page_number);
        it->second.second = lru_list.begin();
        return true;
    }

    misses++;
    return false;
}

void TLB::insert(int page_number, int frame_number) {
    if (table.size() >= capacity) {
        // Evict LRU
        int lru_page = lru_list.back();
        lru_list.pop_back();
        table.erase(lru_page);
    }

    lru_list.push_front(page_number);
    table[page_number] = {frame_number, lru_list.begin()};
}

void TLB::print_stats() const {
    std::cout << "TLB hits: " << hits << "\n";
    std::cout << "TLB misses: " << misses << "\n";

    int total = hits + misses;
    if (total > 0) {
        double ratio = (double)hits / total * 100.0;
        std::cout << "Hit ratio: " << ratio << "%\n";
    }
}
