#!/bin/bash

# HIPAA Compliance Scanner - Integration Test Suite
# This script runs all test cases and generates a report

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
SCANNER="$PROJECT_ROOT/grc-scan"
TEST_FIXTURES="$PROJECT_ROOT/tests/fixtures"
COMPLIANT_DIR="$TEST_FIXTURES/compliant"
NON_COMPLIANT_DIR="$TEST_FIXTURES/non_compliant"

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Print header
print_header() {
    echo -e "${BOLD}${BLUE}"
    echo "═══════════════════════════════════════════════════════════════════"
    echo "  HIPAA COMPLIANCE SCANNER - INTEGRATION TEST SUITE"
    echo "═══════════════════════════════════════════════════════════════════"
    echo -e "${NC}"
}

# Print section header
print_section() {
    echo -e "\n${BOLD}${BLUE}━━━ $1 ━━━${NC}\n"
}

# Run a single test
run_test() {
    local test_file=$1
    local expected_result=$2  # "pass" or "fail"
    local test_name=$(basename "$test_file")
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}[TEST $TOTAL_TESTS]${NC} Testing: $test_name"
    
    # Run the scanner
    if $SCANNER "$test_file" > /tmp/scanner_output_$$.txt 2>&1; then
        scan_exit_code=0
    else
        scan_exit_code=$?
    fi
    
    # Check expected vs actual result
    if [ "$expected_result" == "pass" ]; then
        if [ $scan_exit_code -eq 0 ]; then
            echo -e "${GREEN}  ✓ PASSED${NC} - Configuration is compliant (as expected)\n"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}  ✗ FAILED${NC} - Expected compliance but got non-compliance\n"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            cat /tmp/scanner_output_$$.txt
        fi
    else
        if [ $scan_exit_code -ne 0 ]; then
            echo -e "${GREEN}  ✓ PASSED${NC} - Configuration is non-compliant (as expected)\n"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}  ✗ FAILED${NC} - Expected non-compliance but got compliance\n"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            cat /tmp/scanner_output_$$.txt
        fi
    fi
    
    rm -f /tmp/scanner_output_$$.txt
}

# Check if scanner exists
check_scanner() {
    if [ ! -f "$SCANNER" ]; then
        echo -e "${RED}Error: Scanner not found at $SCANNER${NC}"
        echo "Please build the scanner first: make clean && make"
        exit 1
    fi
    
    if [ ! -x "$SCANNER" ]; then
        echo -e "${RED}Error: Scanner is not executable${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✓ Scanner found: $SCANNER${NC}\n"
}

# Main test execution
main() {
    print_header
    check_scanner
    
    # Test 1: Compliant configurations (should PASS)
    print_section "Testing Compliant Configurations (Expected: 100% compliance)"
    
    if [ -d "$COMPLIANT_DIR" ]; then
        for test_file in "$COMPLIANT_DIR"/*.json "$COMPLIANT_DIR"/*.md "$COMPLIANT_DIR"/*.yaml; do
            if [ -f "$test_file" ]; then
                run_test "$test_file" "pass"
            fi
        done
    else
        echo -e "${YELLOW}Warning: Compliant test directory not found${NC}"
    fi
    
    # Test 2: Non-compliant configurations (should FAIL)
    print_section "Testing Non-Compliant Configurations (Expected: <80% compliance)"
    
    if [ -d "$NON_COMPLIANT_DIR" ]; then
        for test_file in "$NON_COMPLIANT_DIR"/*.json "$NON_COMPLIANT_DIR"/*.md "$NON_COMPLIANT_DIR"/*.yaml; do
            if [ -f "$test_file" ]; then
                run_test "$test_file" "fail"
            fi
        done
    else
        echo -e "${YELLOW}Warning: Non-compliant test directory not found${NC}"
    fi
    
    # Print summary
    print_section "TEST SUMMARY"
    
    echo -e "${BOLD}Total Tests:${NC}   $TOTAL_TESTS"
    echo -e "${GREEN}${BOLD}Passed:${NC}        $PASSED_TESTS"
    echo -e "${RED}${BOLD}Failed:${NC}        $FAILED_TESTS"
    
    if [ $TOTAL_TESTS -gt 0 ]; then
        SUCCESS_RATE=$(( (PASSED_TESTS * 100) / TOTAL_TESTS ))
        echo -e "${BOLD}Success Rate:${NC}  ${SUCCESS_RATE}%"
    fi
    
    echo ""
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════════════"
        echo "  ALL TESTS PASSED! ✓"
        echo "═══════════════════════════════════════════════════════════════════${NC}"
        exit 0
    else
        echo -e "${RED}${BOLD}═══════════════════════════════════════════════════════════════════"
        echo "  SOME TESTS FAILED! ✗"
        echo "═══════════════════════════════════════════════════════════════════${NC}"
        exit 1
    fi
}

# Run main function
main
