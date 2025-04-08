# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = matrix

# Source files
SRCS = matrix.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files and executable
clean:
	rm -f $(OBJS) $(TARGET)
