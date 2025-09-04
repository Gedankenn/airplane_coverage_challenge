# Makefile for task.c
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lm
TARGET = task
SOURCE = task.c
TEST_DIR = test
OUTPUT_FILE = OUTPUT.txt

# Find all INPUT_*.txt files in the test directory
TEST_FILES = $(wildcard $(TEST_DIR)/INPUT_*.txt)

# Default target - builds the executable
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)

# Clean - removes compiled files
clean:
	rm -f $(TARGET) $(OUTPUT_FILE)

# Test - runs the compiled program on all INPUT_*.txt files
test: $(TARGET)
	@echo "Running tests on all INPUT_*.txt files in $(TEST_DIR)/"
	@echo "=========================================="
	@if [ -z "$(TEST_FILES)" ]; then \
		echo "No INPUT_*.txt files found in $(TEST_DIR)/ directory"; \
		echo "Please create test files with pattern INPUT_*.txt"; \
	else \
		for test_file in $(TEST_FILES); do \
			echo ""; \
			echo "Testing: $$test_file"; \
			echo "----------------------------------------"; \
			./$(TARGET) $$test_file; \
			echo "Output:"; \
			cat $(OUTPUT_FILE) 2>/dev/null || echo "No output file generated"; \
			echo "========================================"; \
		done; \
		echo ""; \
		echo "All tests completed!"; \
	fi

# Test a specific file
test-file: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make test-file FILE=test/INPUT_1.txt"; \
	else \
		echo "Testing: $(FILE)"; \
		echo "----------------------------------------"; \
		./$(TARGET) $(FILE); \
		echo "Output:"; \
		cat $(OUTPUT_FILE) 2>/dev/null || echo "No output file generated"; \
	fi

# List all available test files
list-tests:
	@echo "Available test files:"
	@if [ -z "$(TEST_FILES)" ]; then \
		echo "No INPUT_*.txt files found in $(TEST_DIR)/ directory"; \
	else \
		for test_file in $(TEST_FILES); do \
			echo "  $$test_file"; \
		done; \
	fi

# Create test directory structure
setup-tests:
	@mkdir -p $(TEST_DIR)
	@echo "Test directory created: $(TEST_DIR)/"
	@echo "You can now add INPUT_*.txt files to this directory"

# Verbose test - shows more detailed output
test-verbose: $(TARGET)
	@echo "Running verbose tests on all INPUT_*.txt files in $(TEST_DIR)/"
	@echo "=============================================================="
	@if [ -z "$(TEST_FILES)" ]; then \
		echo "No INPUT_*.txt files found in $(TEST_DIR)/ directory"; \
		echo "Please create test files with pattern INPUT_*.txt"; \
	else \
		for test_file in $(TEST_FILES); do \
			echo ""; \
			echo "Testing: $$test_file"; \
			echo "----------------------------------------"; \
			echo "Input file contents:"; \
			cat $$test_file; \
			echo ""; \
			echo "Running program..."; \
			./$(TARGET) $$test_file; \
			echo ""; \
			echo "Program output:"; \
			cat $(OUTPUT_FILE) 2>/dev/null || echo "No output file generated"; \
			echo "============================================================"; \
		done; \
		echo ""; \
		echo "All verbose tests completed!"; \
	fi

# Rebuild - clean and build
rebuild: clean all

# Help - shows available targets
help:
	@echo "Available targets:"
	@echo "  all          - Build the executable (default)"
	@echo "  clean        - Remove compiled files and output"
	@echo "  test         - Run the program on all INPUT_*.txt files in $(TEST_DIR)/"
	@echo "  test-file    - Run the program on a specific file (Usage: make test-file FILE=path/to/file.txt)"
	@echo "  test-verbose - Run tests with detailed input/output display"
	@echo "  list-tests   - List all available test files"
	@echo "  setup-tests  - Create test directory structure"
	@echo "  rebuild      - Clean and build"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make test                              # Run all tests"
	@echo "  make test-file FILE=test/INPUT_1.txt   # Test specific file"
	@echo "  make test-verbose                      # Verbose test output"

# Declare phony targets (targets that don't create files)
.PHONY: all clean test test-file test-verbose list-tests setup-tests rebuild help
