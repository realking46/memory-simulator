Memory Management Simulator

## Overview:

This project simulates operating system memory management at a user-space level. It models:
Dynamic memory allocation using:
   - First Fit
   - Best Fit
   - Worst Fit
   - Buddy allocator

Multilevel CPU caches (L1 and L2) with configurable block size and associativity.
Virtual memory using paging with page table and simple page replacement.
Memory statistics and metrics, including fragmentation, utilization, and allocation success/failure rates.
This is not an actual OS kernel, but a detailed simulation for educational and testing purposes.

## Project Structure

memory-simulator/
├── src/
│   ├── allocator/
│   │   └── memory_manager.cpp / .h
│   ├── buddy/
│   │   └── buddy_allocator.cpp / .h
│   ├── cache/
│   │   └── cache.cpp / .h
│   ├── virtual_memory/
│   │   ├── VirtualMemory.cpp / .h
│   │   └── TLB.cpp / .h
│   └── main.cpp
├── tests/
│   ├── first_fit_basic.txt
│   ├── best_fit_fragmentation.txt
│   ├── worst_fit_behavior.txt
│   ├── buddy_basic.txt
│   ├── buddy_merge.txt
│   ├── buddy_internal_fragmentation.txt
│   ├── cache_log.txt
│   ├── cache_11_12.txt
│   ├── virtual_memory_basic.txt
│   ├── full_pipeline.txt
│   └── outputs/         # Stores test outputs
├── include/
├── docs/
├── run_tests.sh
├── Makefile
├── memsim.exe
└── README.md

## Setup & Compilation
🔹 Option 1: Automated Setup (Recommended)
- Use the provided setup script (works in Git Bash / Linux / WSL):
```bash
chmod +x setup.sh
./setup.sh
```

This will:

- Compile the project
- Generate memsim  (use this to test the scripts  - memsim has already been added in case installation problem)
- Automatically create run_tests.sh
- Outputs are saved in tests/outputs/ folder.

## Running Tests

All test workloads are stored in the tests/ directory.
Run the full test suite:
```bash
./run_tests.sh
```
✔ Outputs are saved in:
```bash
tests/output/
```

## Test Artifacts Included

The project includes the following test artifacts:

1. **Memory Allocation Tests**

    - buddy_basic.txt
    - buddy_merge.txt
    - buddy_internal_fragmentation.txt
    - first_fit_basic.txt
    - best_fit_fragmentation.txt
    - worst_fit_behavior.txt

2. **Cache Tests**

    - cache_log.txt
    - cache_11_12.txt

3. **Virtual Memory Tests**

    - virtual_memory_basic.txt

4. **End-to-End Test**

    - full_pipeline.txt

5. **Each test validates:**
 
    - Allocation correctness
    - Fragmentation behavior
    - Cache hit/miss tracking
    - Page faults and TLB behavior

## Supported Features

1. **Memory Allocation Algorithms:**

    - First Fit
    - Best Fit
    - Worst Fit
    - Buddy Allocator (splitting and merging)

2. **Memory Metrics:**

   - Total memory
   - Used memory
   - Free memory
   - Internal fragmentation
   - External fragmentation
   - Allocation success/failure rate
   - Memory utilization

3. **Cache Simulation:**

   - L1 and L2 caches
   - FIFO replacement (LRU optional)
   - Tracks hits and misses

4. **Virtual Memory Simulation:**

   - Paging
   - Page table
   - Page hits and faults
   - Optional disk latency simulation

5. **Integration:**

    Virtual address → Page Table → Physical Memory → Cache → RAM

## Statistics Reported

The simulator reports:

1. **Memory**

   - Allocation requests
   - Successful / failed allocations
   - Internal & external fragmentation
   - Memory utilization

2. **Cache**

   - L1 cache hits & misses
   - L2 cache hits & misses

3. **Virtual Memory**

   - Page hits
   - Page faults

## Authors
Nishant Singh










