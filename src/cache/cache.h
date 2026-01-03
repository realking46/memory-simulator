#include <bits/stdc++.h>
using namespace std;

struct CacheLine {
    bool valid;
    size_t tag;
    size_t arrival_time;
};

class Cache {
public:
    Cache(size_t cache_size, size_t block_size, int latency);

    bool access(size_t address);  // returns HIT or MISS
    void print_stats() const;
    int get_latency() const;      // returns last access latency
    void dump() const;
    void reset_stats();

private:
    size_t cache_size;
    size_t block_size;
    size_t num_lines;
    int access_latency;           // cycles per access

    size_t time_counter;
    size_t hits;
    size_t misses;

    std::vector<CacheLine> lines;
};