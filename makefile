# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++17

# Directories
SRCDIR = src
BUILDDIR = build
INCDIR = include

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.hpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))
TARGET = program

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Rule to compile source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean up build files
clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean