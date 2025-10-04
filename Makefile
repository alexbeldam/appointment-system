CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Wextra -pedantic -Iinclude -MMD -MP
LDFLAGS     :=
BUILD_DIR   := build
SRC_DIR     := src
INCLUDE_DIR := include
TARGET      := programa
SRC_ROOT    := $(wildcard $(SRC_DIR)/*.cpp)
SRC_HELPER  := $(wildcard $(SRC_DIR)/helper/*.cpp)
SOURCES     := $(SRC_ROOT) $(SRC_HELPER)
OBJECTS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS        := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.d, $(SOURCES))

all: $(EXEC)

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