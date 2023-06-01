# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# All targets (without extension)
TARGETS = server client

# Object files
OBJS = common.o

# Default rule
all: $(TARGETS)

server: server.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $< $(OBJS) -o $@

client: client.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $< $(OBJS) -o $@

common.o: common.cpp common.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Phony rule for clean
.PHONY: clean

# Rule for clean
clean:
	rm -f $(TARGETS) $(OBJS)
