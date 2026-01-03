#!/bin/bash

echo "========================================"
echo " Memory Simulator Setup Script"
echo "========================================"

set -e

# -------------------------------------------------
# Step 1: Create run_tests.sh
# -------------------------------------------------
echo "[1/3] Creating run_tests.sh..."

cat << 'EOF' > run_tests.sh
#!/bin/bash

echo "========================================"
echo " Running Memory Simulator Test Suite"
echo "========================================"

mkdir -p tests/output

run_test () {
    local input_file=$1
    local output_file=$2

    echo "Running test: $input_file"
    ./memsim < tests/$input_file > tests/output/$output_file
    echo "✔ Output saved to tests/output/$output_file"
    echo "----------------------------------------"
}

run_test buddy_basic.txt buddy_basic.out
run_test buddy_merge.txt buddy_merge.out
run_test buddy_internal_fragmentation.txt buddy_internal_fragmentation.out

run_test first_fit_basic.txt first_fit_basic.out
run_test best_fit_fragmentation.txt best_fit_fragmentation.out
run_test worst_fit_behavior.txt worst_fit_behavior.out

run_test cache_log.txt cache_log.out
run_test cache_11_12.txt cache_11_12.out

run_test virtual_memory_basic.txt virtual_memory_basic.out

run_test full_pipeline.txt full_pipeline.out

echo "========================================"
echo " All tests completed successfully"
echo " Outputs are in tests/output/"
echo "========================================"
EOF

chmod +x run_tests.sh
echo "✔ run_tests.sh created"

# -------------------------------------------------
# Step 2: Compile memsim
# -------------------------------------------------
echo "[2/3] Compiling Memory Simulator..."

g++ -std=gnu++20 \
    -Isrc \
    -Iinclude \
    src/main.cpp \
    src/allocator/memory_manager.cpp \
    src/cache/cache.cpp \
    src/virtual_memory/VirtualMemory.cpp \
    src/virtual_memory/TLB.cpp \
    src/buddy/buddy_allocator.cpp \
    -o memsim

echo "✔ Compilation successful: memsim created"

# -------------------------------------------------
# Step 3: Final message
# -------------------------------------------------
echo "[3/3] Setup complete"
echo "----------------------------------------"
echo "Run simulator manually:"
echo "  ./memsim"
echo ""
echo "Run all tests:"
echo "  ./run_tests.sh"
echo "----------------------------------------"
