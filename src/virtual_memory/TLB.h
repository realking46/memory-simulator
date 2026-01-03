#ifndef TLB_H
#define TLB_H

#include <unordered_map>
#include <list>

class TLB {
public:
    TLB(int size);

    bool lookup(int page_number, int &frame_number);
    void insert(int page_number, int frame_number);
    void print_stats() const;

private:
    int capacity;

    std::unordered_map<int, std::pair<int, std::list<int>::iterator>> table;
    std::list<int> lru_list; // most recently used at front

    int hits;
    int misses;
};

#endif
