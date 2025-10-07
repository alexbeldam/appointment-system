CXX           := g++
CXXFLAGS      := -std=c++17 -Wall -Wextra -pedantic -Wno-unused-parameter -Iinclude -MMD -MP
LDFLAGS       :=
BUILD_DIR     := build
SRC_DIR       := src
INCLUDE_DIR   := include
TARGET        := programa
SOURCES       := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS       := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS          := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.d, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "🛠️  Linking $(TARGET)..."
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "⚙️  Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "🧹 Cleaning up build directory and target..."
	-@rm -rf $(BUILD_DIR) $(TARGET) 2>/dev/null || ( \
	  echo "Falling back to Windows commands..." ; \
	  -@del /Q $(subst /,\,$^)\*.o $(subst /,\,$^)\*.d >NUL 2>NUL ; \
	  -@rmdir /S /Q $(BUILD_DIR) >NUL 2>NUL ; \
	  -@del $(TARGET).exe $(TARGET) >NUL 2>NUL \
	)
	@echo "✅ Clean finished."

.PHONY: rebuild
rebuild: clean all

-include $(DEPS)