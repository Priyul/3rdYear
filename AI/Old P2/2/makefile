# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# Executable name
TARGET = main

# Directories
SRCDIR = ./
OBJDIR = ./obj
BINDIR = ./

# Files
SOURCES = $(wildcard $(SRCDIR)*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)%.cpp=$(OBJDIR)%.o)

# Build rules
all: $(BINDIR)$(TARGET)

$(BINDIR)$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)$(TARGET)
