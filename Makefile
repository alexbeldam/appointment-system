CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
EXEC = programa
BUILD_DIR = build

.PHONY: all clean $(BUILD_DIR)

all: $(EXEC)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@

build/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXEC)
