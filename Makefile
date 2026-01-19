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

# Target executable
TARGET = grc-scan-hipaa

# Source files
MAIN_SRC = $(SRC_DIR)/main_hipaa_test.c
CORE_SRC = $(SRC_DIR)/scanner_core.c
HIPAA_LOADER_SRC = $(HIPAA_DIR)/hipaa_loader.c
HIPAA_CHECKS_SRC = $(HIPAA_DIR)/hipaa_checks.c
HIPAA_SCANNER_SRC = $(HIPAA_DIR)/hipaa_scanner.c

# Object files
MAIN_OBJ = $(SRC_DIR)/main_hipaa_test.o
CORE_OBJ = $(SRC_DIR)/scanner_core.o
HIPAA_LOADER_OBJ = $(HIPAA_DIR)/hipaa_loader.o
HIPAA_CHECKS_OBJ = $(HIPAA_DIR)/hipaa_checks.o
HIPAA_SCANNER_OBJ = $(HIPAA_DIR)/hipaa_scanner.o

# All object files
OBJS = $(MAIN_OBJ) $(CORE_OBJ) $(HIPAA_LOADER_OBJ) $(HIPAA_CHECKS_OBJ) $(HIPAA_SCANNER_OBJ)

# Header files
HEADERS = $(INC_DIR)/grc_scanner.h $(INC_DIR)/frameworks/hipaa.h

# Default target
.PHONY: all
all: check-deps $(TARGET)

# Link target
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Build successful! Run with: ./$(TARGET)"

# Compile main program
$(MAIN_OBJ): $(MAIN_SRC) $(HEADERS)
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
	rm -f $(OBJS) $(TARGET)
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
	./$(TARGET)

# Run with test config
.PHONY: test
test: $(TARGET)
	@echo "Running test scan..."
	@if [ -f test_config.yaml ]; then \
		./$(TARGET) test_config.yaml; \
	else \
		./$(TARGET); \
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
	@echo "Target: $(TARGET)"
	@echo "Source files:"
	@echo "  - $(MAIN_SRC)"
	@echo "  - $(CORE_SRC)"
	@echo "  - $(HIPAA_LOADER_SRC)"
	@echo "  - $(HIPAA_CHECKS_SRC)"
	@echo "  - $(HIPAA_SCANNER_SRC)"
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
	@echo "  make run          - Build and run the scanner"
	@echo "  make test         - Run with test configuration"
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
	@echo "  make              # Build the project"
	@echo "  make clean all    # Clean and build"
	@echo "  make debug run    # Debug build and run"
	@echo ""

# Phony targets (not actual files)
.PHONY: all clean distclean run test install-deps setup info debug release rebuild help check-deps