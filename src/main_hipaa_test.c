#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grc_scanner.h"
#include "frameworks/hipaa.h"

// ANSI color codes for terminal output
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

// Print a horizontal line
void print_line(char c, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", c);
    }
    printf("\n");
}

// Print a box header
void print_box_header(const char *title) {
    printf("\n");
    print_line('=', 80);
    printf("%s%s%s%s\n", COLOR_BOLD, COLOR_CYAN, title, COLOR_RESET);
    print_line('=', 80);
}

// Print check result with color
void print_check_result(const check_result_t *result) {
    const char *status_color = result->passed ? COLOR_GREEN : COLOR_RED;
    const char *status_text = result->passed ? "PASS" : "FAIL";
    
    printf("\n┌─ [%s%s%s] %s%s - %s%s\n",
           status_color, status_text, COLOR_RESET,
           COLOR_BOLD, result->control_id, result->control_name, COLOR_RESET);
    
    printf("│  %sSeverity:%s %s\n", COLOR_BOLD, COLOR_RESET, result->severity);
    
    if (result->details) {
        printf("│  %sDetails:%s %s\n", COLOR_BOLD, COLOR_RESET, result->details);
    }
    
    if (!result->passed && result->remediation) {
        printf("│  %s%sRemediation:%s %s\n", 
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET, result->remediation);
    }
    
    printf("└─────────────────────────────────────────────────────────────────\n");
}

// Create test configuration string
char* create_test_config_string(void) {
    // Create a realistic test configuration
    const char *config_template = 
        "# Security Configuration\n"
        "encryption: enabled\n"
        "encrypt_at_rest: true\n"
        "kms_key_id: arn:aws:kms:us-east-1:123456789012:key/12345678-1234-1234-1234-123456789012\n"
        "\n"
        "# Audit and Logging\n"
        "audit_log: enabled\n"
        "cloudtrail: enabled\n"
        "logging: true\n"
        "\n"
        "# Authentication\n"
        "mfa_enabled: true\n"
        "require_mfa: true\n"
        "\n"
        "# Network Security\n"
        "tls: enabled\n"
        "tls_version: 1.3\n"
        "https_only: true\n"
        "\n"
        "# User Management\n"
        "unique_user_id: true\n"
        "iam_enabled: true\n"
        "\n"
        "# Backup\n"
        "backup_enabled: true\n"
        "automated_backup: true\n"
        "\n"
        "# Access Control\n"
        "access_termination: automated\n"
        "auto_logoff: enabled\n"
        "session_timeout: 15\n";
    
    return strdup(config_template);
}

int main(int argc, char *argv[]) {
    printf("%s%s", COLOR_BOLD, COLOR_CYAN);
    printf("╔════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                      HIPAA COMPLIANCE SCANNER v1.0                         ║\n");
    printf("║                    Testing Security Configuration                          ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", COLOR_RESET);
    
    // Create test configuration
    char *config_data = create_test_config_string();
    if (!config_data) {
        fprintf(stderr, "%sError: Failed to create test configuration%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    }
    
    printf("%sTest Configuration:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s%s%s\n", COLOR_YELLOW, config_data, COLOR_RESET);
    
    // Scan the configuration
    print_box_header("RUNNING HIPAA COMPLIANCE CHECKS");
    
    scan_result_t *scan_result = hipaa_scan_config(config_data);
    
    if (!scan_result) {
        fprintf(stderr, "%sError: Scan failed%s\n", COLOR_RED, COLOR_RESET);
        free(config_data);
        return 1;
    }
    
    // Display results
    print_box_header("SCAN RESULTS");
    
    for (size_t i = 0; i < scan_result->result_count; i++) {
        print_check_result(scan_result->results[i]);
    }
    
    // Print summary
    print_box_header("COMPLIANCE SUMMARY");
    
    double compliance_score = scan_result->result_count > 0 
        ? (double)scan_result->passed_count / scan_result->result_count * 100.0 
        : 0.0;
    
    printf("\n");
    printf("  Total Checks:    %s%zu%s\n", COLOR_BOLD, scan_result->result_count, COLOR_RESET);
    printf("  %sPassed:%s          %s%zu%s\n", 
           COLOR_GREEN, COLOR_RESET, COLOR_BOLD, scan_result->passed_count, COLOR_RESET);
    printf("  %sFailed:%s          %s%zu%s\n", 
           COLOR_RED, COLOR_RESET, COLOR_BOLD, scan_result->failed_count, COLOR_RESET);
    printf("  Compliance Score: %s%.1f%%%s\n", 
           compliance_score >= 80.0 ? COLOR_GREEN : COLOR_RED,
           compliance_score, COLOR_RESET);
    
    printf("\n");
    
    if (compliance_score >= 80.0) {
        printf("  %s✓ PASSED - Configuration meets HIPAA compliance requirements%s\n",
               COLOR_GREEN, COLOR_RESET);
    } else {
        printf("  %s✗ FAILED - Configuration does not meet HIPAA compliance requirements%s\n",
               COLOR_RED, COLOR_RESET);
        printf("  %sPlease review and remediate the failed checks above%s\n",
               COLOR_YELLOW, COLOR_RESET);
    }
    
    print_line('=', 80);
    printf("\n");
    
    // Cleanup
    free_scan_result(scan_result);
    free(config_data);
    
    return compliance_score >= 80.0 ? 0 : 1;
}