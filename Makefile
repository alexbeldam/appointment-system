ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
    EXECUTABLE_EXT := .exe
    RM_CMD := del /Q
    RMDIR_CMD := rmdir /S /Q
    LINK_MSG := Linking $(EXECUTABLE)...
    COMPILE_MSG := Compiling $<...
    CLEAN_MSG := Cleaning up build directory and target...
    CLEAN_SUCCESS_MSG := Clean finished.
    DOXYGEN_CONFIG_MSG := Generating documentation...
else
    OS_NAME := $(shell uname -s)
    EXECUTABLE_EXT := 
    RM_CMD := rm -f
    RMDIR_CMD := rm -rf
    LINK_MSG := 🛠️ Linking $(EXECUTABLE)...
    COMPILE_MSG := ⚙️ Compiling $<...
    CLEAN_MSG := 🧹 Cleaning up build directory and target...
    CLEAN_SUCCESS_MSG := ✅ Clean finished.
    DOXYGEN_CONFIG_MSG := 📄 Generating documentation...
endif

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Wno-unused-parameter -Iinclude -MMD -MP
LDFLAGS :=

BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include
TARGET := programa
EXECUTABLE := $(TARGET)$(EXECUTABLE_EXT)

DOC_DIR := docs/html_doc
DOXYGEN_CMD := doxygen Doxyfile

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.d, $(SOURCES))

.PHONY: all clean rebuild doc

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

doc:
	@echo "$(DOXYGEN_CONFIG_MSG)"
ifeq ($(OS_NAME),Windows)
	@if not exist $(subst /,\,,$(DOC_DIR)) mkdir $(subst /,\,,$(DOC_DIR))
else
	@mkdir -p $(DOC_DIR)
endif
	@$(DOXYGEN_CMD)

clean:
	@echo "$(CLEAN_MSG)"
ifeq ($(OS_NAME),Windows)
	-@$(RMDIR_CMD) $(BUILD_DIR) 2>NUL
	-@$(RM_CMD) $(EXECUTABLE) 2>NUL
	-@$(RMDIR_CMD) $(DOC_DIR) 2>NUL
else
	-@$(RMDIR_CMD) $(BUILD_DIR) 2>/dev/null || true
	-@$(RM_CMD) $(EXECUTABLE) 2>/dev/null || true
	-@$(RMDIR_CMD) $(DOC_DIR) 2>/dev/null || true
endif
	@echo "$(CLEAN_SUCCESS_MSG)"

rebuild: clean all

-include $(DEPS)