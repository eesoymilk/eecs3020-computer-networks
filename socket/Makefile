# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -pedantic

# All targets (without extension)
TARGETS = server client

# Default rule
all: $(TARGETS)

# Rule for server and client
$(TARGETS): %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Phony rule for clean
.PHONY: clean

# Rule for clean
clean:
	rm -f $(TARGETS)

