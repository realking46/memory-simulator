#include "cache.h"
#include <iostream>

Cache::Cache(size_t csize, size_t bsize, int latency)
    : cache_size(csize),
      block_size(bsize),
      access_latency(latency),
      time_counter(0),
      hits(0),
      misses(0) {

    num_lines = cache_size / block_size;
    lines.resize(num_lines, {false, 0, 0});
}

bool Cache::access(size_t address) {
    size_t block_addr = address / block_size;
    time_counter++;

    // Check for HIT
    for (auto& line : lines) {
        if (line.valid && line.tag == block_addr) {
            hits++;
            return true;
        }
    }

    // MISS
    misses++;

    // Find empty line
    for (auto& line : lines) {
        if (!line.valid) {
            line.valid = true;
            line.tag = block_addr;
            line.arrival_time = time_counter;
            return false;
        }
    }

    // LRU/FIFO replacement
    auto victim = lines.begin();
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        if (it->arrival_time < victim->arrival_time) {
            victim = it;
        }
    }

    victim->tag = block_addr;
    victim->arrival_time = time_counter;

    return false;
}

int Cache::get_latency() const {
    return access_latency;
}

void Cache::print_stats() const {
    std::cout << "Cache hits: " << hits << "\n";
    std::cout << "Cache misses: " << misses << "\n";

    if (hits + misses > 0) {
        double hit_ratio =
            (double)hits / (hits + misses) * 100.0;
        std::cout << "Hit ratio: " << hit_ratio << "%\n";
    }
}

void Cache::dump() const {
    std::cout << "===== Cache Dump =====\n";

    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "Line " << i << ": ";

        if (lines[i].valid) {
            std::cout << "VALID  tag=" << lines[i].tag;
        } else {
            std::cout << "INVALID";
        }

        std::cout << "\n";
    }
}
