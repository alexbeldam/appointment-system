ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
    EXECUTABLE_EXT := .exe
    RM_CMD := del /Q
    RMDIR_CMD := rmdir /S /Q
    LINK_MSG := Linking $(EXECUTABLE)...
    COMPILE_MSG := Compiling $<...
    CLEAN_MSG := Cleaning up build directory and target...
    CLEAN_SUCCESS_MSG := Clean finished.
else
    OS_NAME := $(shell uname -s)
    EXECUTABLE_EXT := 
    RM_CMD := rm -f
    RMDIR_CMD := rm -rf
    LINK_MSG := 🛠️ Linking $(EXECUTABLE)...
    COMPILE_MSG := ⚙️ Compiling $<...
    CLEAN_MSG := 🧹 Cleaning up build directory and target...
    CLEAN_SUCCESS_MSG := ✅ Clean finished.
endif

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Wno-unused-parameter -Iinclude -MMD -MP
LDFLAGS :=
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include
TARGET := programa
EXECUTABLE := $(TARGET)$(EXECUTABLE_EXT)
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.d, $(SOURCES))

.PHONY: all clean rebuild

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "$(LINK_MSG)"
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS_NAME),Windows)
	@if not exist $(dir $@) mkdir $(subst /,\,,$(dir $@))
else
	@mkdir -p $(dir $@)
endif
	@echo "$(COMPILE_MSG)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(CLEAN_MSG)"
ifeq ($(OS_NAME),Windows)
	-@$(RMDIR_CMD) $(BUILD_DIR) 2>NUL
	-@$(RM_CMD) $(EXECUTABLE) 2>NUL
else
	-@$(RMDIR_CMD) $(BUILD_DIR) 2>/dev/null || true
	-@$(RM_CMD) $(EXECUTABLE) 2>/dev/null || true
endif
	@echo "$(CLEAN_SUCCESS_MSG)"

rebuild: clean all

-include $(DEPS)