#### **Expected Outputs / Correctness Criteria**



Each test workload is evaluated using the following correctness conditions:



**Allocation Correctness**

* Allocated block sizes are rounded to the nearest power of two
* Returned block IDs correspond to valid memory addresses
* Allocation fails gracefully when insufficient memory is available



**Deallocation Correctness**

* Freed blocks are returned to the appropriate free list
* Buddy blocks merge recursively when possible
* No memory leaks occur



**Fragmentation Metrics**

* Internal fragmentation is computed as:   allocated\_block\_size − requested\_size
* External fragmentation is always 0% for buddy allocation



**Statistics Validation**

* Allocation request count matches number of malloc calls
* Successful and failed allocations are tracked correctly
* Memory utilization matches:   used\_memory / total\_memory × 100
