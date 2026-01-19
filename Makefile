# GRC Scanner Makefile
# Complete build system for HIPAA compliance scanner

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include
LDFLAGS = -lyaml

# Directories
SRC_DIR = src
OBJ_DIR = src
INC_DIR = include
HIPAA_DIR = $(SRC_DIR)/frameworks/hipaa
PARSER_DIR = $(SRC_DIR)/parsers

# Target executables
TARGET = grc-scan
TARGET_TEST = grc-scan-hipaa

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
MAIN_TEST_SRC = $(SRC_DIR)/main_hipaa_test.c
CORE_SRC = $(SRC_DIR)/scanner_core.c
HIPAA_LOADER_SRC = $(HIPAA_DIR)/hipaa_loader.c
HIPAA_CHECKS_SRC = $(HIPAA_DIR)/hipaa_checks.c
HIPAA_SCANNER_SRC = $(HIPAA_DIR)/hipaa_scanner.c

# Parser source files
PARSER_UTILS_SRC = $(PARSER_DIR)/file_parser_utils.c
MD_PARSER_SRC = $(PARSER_DIR)/md_parser.c
JSON_PARSER_SRC = $(PARSER_DIR)/json_parser.c
PDF_PARSER_SRC = $(PARSER_DIR)/pdf_parser.c

# Object files
MAIN_OBJ = $(SRC_DIR)/main.o
MAIN_TEST_OBJ = $(SRC_DIR)/main_hipaa_test.o
CORE_OBJ = $(SRC_DIR)/scanner_core.o
HIPAA_LOADER_OBJ = $(HIPAA_DIR)/hipaa_loader.o
HIPAA_CHECKS_OBJ = $(HIPAA_DIR)/hipaa_checks.o
HIPAA_SCANNER_OBJ = $(HIPAA_DIR)/hipaa_scanner.o

# Parser object files
PARSER_UTILS_OBJ = $(PARSER_DIR)/file_parser_utils.o
MD_PARSER_OBJ = $(PARSER_DIR)/md_parser.o
JSON_PARSER_OBJ = $(PARSER_DIR)/json_parser.o
PDF_PARSER_OBJ = $(PARSER_DIR)/pdf_parser.o

# All object files for main program
PARSER_OBJS = $(PARSER_UTILS_OBJ) $(MD_PARSER_OBJ) $(JSON_PARSER_OBJ) $(PDF_PARSER_OBJ)
COMMON_OBJS = $(CORE_OBJ) $(HIPAA_LOADER_OBJ) $(HIPAA_CHECKS_OBJ) $(HIPAA_SCANNER_OBJ)
OBJS = $(MAIN_OBJ) $(COMMON_OBJS) $(PARSER_OBJS)
TEST_OBJS = $(MAIN_TEST_OBJ) $(COMMON_OBJS)

# Header files
HEADERS = $(INC_DIR)/grc_scanner.h $(INC_DIR)/frameworks/hipaa.h $(INC_DIR)/parsers/file_parsers.h

# Default target
.PHONY: all
all: check-deps $(TARGET) $(TARGET_TEST)

# Link main target with file parsers
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Build successful! Run with: ./$(TARGET) <file>"

# Link test target
$(TARGET_TEST): $(TEST_OBJS)
	@echo "Linking $(TARGET_TEST)..."
	$(CC) $(TEST_OBJS) -o $(TARGET_TEST) $(LDFLAGS)
	@echo "✅ Test build successful! Run with: ./$(TARGET_TEST)"

# Compile main program
$(MAIN_OBJ): $(MAIN_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test main program
$(MAIN_TEST_OBJ): $(MAIN_TEST_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile core scanner
$(CORE_OBJ): $(CORE_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile HIPAA loader
$(HIPAA_LOADER_OBJ): $(HIPAA_LOADER_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile HIPAA checks
$(HIPAA_CHECKS_OBJ): $(HIPAA_CHECKS_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile HIPAA scanner
$(HIPAA_SCANNER_OBJ): $(HIPAA_SCANNER_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile parser utilities
$(PARSER_UTILS_OBJ): $(PARSER_UTILS_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile MD parser
$(MD_PARSER_OBJ): $(MD_PARSER_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile JSON parser
$(JSON_PARSER_OBJ): $(JSON_PARSER_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile PDF parser
$(PDF_PARSER_OBJ): $(PDF_PARSER_SRC) $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Check dependencies
.PHONY: check-deps
check-deps:
	@echo "Checking dependencies..."
	@which $(CC) > /dev/null || (echo "ERROR: gcc not found" && exit 1)
	@which pkg-config > /dev/null || (echo "ERROR: pkg-config not found" && exit 1)
	@pkg-config --exists yaml-0.1 || (echo "ERROR: libyaml not found" && exit 1)
	@echo "✅ All dependencies satisfied"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TARGET_TEST)
	rm -f $(PARSER_DIR)/*.o
	@echo "✅ Clean complete"

# Clean everything including backup files
.PHONY: distclean
distclean: clean
	@echo "Deep cleaning..."
	rm -f *~ $(SRC_DIR)/*~ $(HIPAA_DIR)/*~ $(INC_DIR)/*~
	rm -f $(SRC_DIR)/*.o.backup $(HIPAA_DIR)/*.o.backup
	@echo "✅ Deep clean complete"

# Run the scanner
.PHONY: run
run: $(TARGET)
	@echo "Running HIPAA scanner..."
	@echo "Usage: ./$(TARGET) <config-file>"
	@echo "Example: ./$(TARGET) examples/config.json"

# Run test scanner
.PHONY: test
test: $(TARGET_TEST)
	@echo "Running test scan..."
	./$(TARGET_TEST)

# Run with example JSON file
.PHONY: run-json
run-json: $(TARGET)
	@if [ -f examples/json/test-config.json ]; then \
		./$(TARGET) examples/json/test-config.json; \
	else \
		echo "Error: examples/json/test-config.json not found"; \
	fi

# Run with example MD file
.PHONY: run-md
run-md: $(TARGET)
	@if [ -f examples/markdown/security-config.md ]; then \
		./$(TARGET) examples/markdown/security-config.md; \
	else \
		echo "Error: examples/markdown/security-config.md not found"; \
	fi

# Install dependencies (Ubuntu/Debian)
.PHONY: install-deps
install-deps:
	@echo "Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y gcc make pkg-config libyaml-dev
	@echo "✅ Dependencies installed"

# Create directory structure
.PHONY: setup
setup:
	@echo "Creating directory structure..."
	mkdir -p $(SRC_DIR)
	mkdir -p $(HIPAA_DIR)
	mkdir -p $(INC_DIR)/frameworks
	@echo "✅ Directory structure created"

# Show build info
.PHONY: info
info:
	@echo "=== GRC Scanner Build Info ==="
	@echo "Compiler: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Libraries: $(LDFLAGS)"
	@echo "Targets: $(TARGET), $(TARGET_TEST)"
	@echo "Source files:"
	@echo "  - $(MAIN_SRC)"
	@echo "  - $(MAIN_TEST_SRC)"
	@echo "  - $(CORE_SRC)"
	@echo "  - $(HIPAA_LOADER_SRC)"
	@echo "  - $(HIPAA_CHECKS_SRC)"
	@echo "  - $(HIPAA_SCANNER_SRC)"
	@echo "  - $(PARSER_UTILS_SRC)"
	@echo "  - $(MD_PARSER_SRC)"
	@echo "  - $(JSON_PARSER_SRC)"
	@echo "  - $(PDF_PARSER_SRC)"
	@echo "=============================="

# Debug build with symbols
.PHONY: debug
debug: CFLAGS += -g -O0 -DDEBUG
debug: clean $(TARGET)
	@echo "✅ Debug build complete"

# Release build with optimization
.PHONY: release
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)
	@echo "✅ Release build complete"

# Rebuild everything
.PHONY: rebuild
rebuild: clean all

# Help target
.PHONY: help
help:
	@echo "GRC Scanner - Available Make Targets:"
	@echo ""
	@echo "  make              - Build the project (default)"
	@echo "  make all          - Same as 'make'"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make distclean    - Deep clean including backups"
	@echo "  make run          - Show usage for main scanner"
	@echo "  make test         - Run test scanner (no file needed)"
	@echo "  make run-json     - Run with example JSON file"
	@echo "  make run-md       - Run with example MD file"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make release      - Build optimized release version"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make check-deps   - Check if dependencies are installed"
	@echo "  make install-deps - Install dependencies (Ubuntu/Debian)"
	@echo "  make setup        - Create directory structure"
	@echo "  make info         - Show build information"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                          # Build the project"
	@echo "  make clean all                # Clean and build"
	@echo "  make test                     # Run test scanner"
	@echo "  ./grc-scan config.json        # Scan JSON file"
	@echo "  ./grc-scan security.md        # Scan Markdown file"
	@echo "  ./grc-scan compliance.pdf     # Scan PDF file"
	@echo ""

# Phony targets (not actual files)
.PHONY: all clean distclean run test install-deps setup info debug release rebuild help check-deps
