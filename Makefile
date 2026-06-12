# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Target executable name
TARGET = program

# Source files
SRC = main.cpp Hospital_module.cpp pharmacy.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)
