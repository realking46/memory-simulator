# Compiler
CXX = g++
CXXFLAGS = -std=gnu++20 -Wall -Wextra -O2 \
           -Iinclude \
           -Isrc

# Target
TARGET = memsim

# Source files
SRCS = src/main.cpp \
       src/allocator/memory_manager.cpp \
       src/buddy/buddy_allocator.cpp \
       src/cache/cache.cpp \
       src/virtual_memory/VirtualMemory.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run simulator
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
