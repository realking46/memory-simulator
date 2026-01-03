#include <iostream>
#include <sstream>
#include "allocator/memory_manager.h"
#include "cache/cache.h"
#include "virtual_memory/VirtualMemory.h"
#include "buddy/buddy_allocator.h"
using namespace std;

int main() {
    MemoryManager mem;
    string line;

    Cache l1(128, 16, 1);   // 1 cycle per access
    Cache l2(512, 16, 5);   // 5 cycles per access
    VirtualMemory vm(4); // 4 physical frames

    cout << "Memory Simulator\n";

    while (true) {
        cout << "> ";
        getline(cin, line);

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "init") {
            string type;
            size_t size;
            ss >> type >> size;
            if (type == "memory") {
                mem.init_memory(size);
                cout << "Initialized memory with size " << size << " bytes\n";
            }
        } 
        else if (cmd == "dump") {
            string what, level;
            ss >> what >> level;

            if (what == "memory") {
                mem.dump_memory();
            }
            if (what == "cache") {
                if (level == "l1") l1.dump();
                else if (level == "l2") l2.dump();
            }
            if (what == "vm") {
                vm.dump();
            }
        }
        else if (cmd == "exit") {
            break;
        }
        else if (cmd == "malloc") {
            size_t size;
            ss >> size;

            int id = mem.malloc_block(size);
            if (id == -1) {
                cout << "Allocation failed\n";
            } else {
                cout << "Allocated block id=" << id << "\n";
            }
        }
        else if (cmd == "free") {
            int id;
            ss >> id;

            if (mem.free_block(id)) {
                cout << "Block " << id << " freed and merged\n";
            } else {
                cout << "Invalid block id\n";
            }
        }
        else if (cmd == "stats") {
            string what;
            ss >> what;

            if (what == "all") {
                cout << "\n===== MEMORY STATS =====\n";
                mem.print_stats();

                cout << "\n===== L1 CACHE STATS =====\n";
                l1.print_stats();

                cout << "\n===== L2 CACHE STATS =====\n";
                l2.print_stats();

                cout << "\n===== VIRTUAL MEMORY STATS =====\n";
                vm.print_stats();
            }
        }
        else if (cmd == "set") {
            std::string what, which;
            ss >> what >> which;

            if (what == "allocator") {
                if (which == "first_fit")
                    mem.set_allocator(AllocatorType::FIRST_FIT);
                else if (which == "best_fit")
                    mem.set_allocator(AllocatorType::BEST_FIT);
                else if (which == "worst_fit")
                    mem.set_allocator(AllocatorType::WORST_FIT);
                else if (which == "buddy") {
                    mem.set_allocator(AllocatorType::BUDDY);
                }
                std::cout << "Allocator set to " << which << "\n";
            }
        }
        else if (cmd == "access") {
            size_t virtual_addr;
            ss >> virtual_addr;
            int total_latency = 0;

            // 1️⃣ Virtual Memory
            int physical_addr = vm.access(virtual_addr);
            total_latency += 1; // assume 1 cycle for page table/TLB access

            // 2️⃣ L1 Cache
            if (l1.access(physical_addr)) {
                std::cout << "L1 HIT\n";
                total_latency += l1.get_latency();
            } else {
                std::cout << "L1 MISS -> ";
                total_latency += l1.get_latency();

                // 3️⃣ L2 Cache
                if (l2.access(physical_addr)) {
                    std::cout << "L2 HIT\n";
                    total_latency += l2.get_latency();
                } else {
                    std::cout << "L2 MISS -> Memory Access\n";
                    total_latency += l2.get_latency() + 50; // 50 cycles for RAM
                }
            }
            std::cout << "Total access latency: " << total_latency << " cycles\n";

        }
        else if (cmd == "cache") {
            string what, level;
            ss >> what >> level;

            if (what == "stats") {
                if (level == "l1") {
                    cout << "L1 Cache Stats\n";
                    l1.print_stats();
                }
                else if (level == "l2") {
                    cout << "L2 Cache Stats\n";
                    l2.print_stats();
                }
            }
        }
        else if (cmd == "vm") {
            std::string what;
            ss >> what;
            if (what == "stats") {
                vm.print_stats();
            }
        }
        else if (cmd == "tlb") {
            std::string what;
            ss >> what;
            if (what == "stats") {
                vm.print_tlb_stats();
            }
        }
        
        else {
            cout << "Unknown command\n";
        }
    }

    return 0;
}
