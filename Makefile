CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
EXEC = programa

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(EXEC)
