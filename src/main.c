#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grc_scanner.h"
#include "frameworks/hipaa.h"
#include "parsers/file_parsers.h"

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

// Print usage information
void print_usage(const char *program_name) {
    printf("Usage: %s <config-file>\n\n", program_name);
    printf("Supported file formats:\n");
    printf("  - Markdown (.md, .markdown)\n");
    printf("  - JSON (.json)\n");
    printf("  - PDF (.pdf)\n");
    printf("  - Text/YAML (.txt, .yaml, .yml, .conf, .config)\n\n");
    printf("Example:\n");
    printf("  %s config.json\n", program_name);
    printf("  %s security-policy.md\n", program_name);
    printf("  %s compliance-doc.pdf\n", program_name);
}

// Print banner
void print_banner(void) {
    printf("%s%s", COLOR_BOLD, COLOR_CYAN);
    printf("\n");
    printf("   ██████╗ ██████╗ ███╗   ███╗██████╗ ██╗  ██╗   ██╗██████╗ \n");
    printf("  ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██║  ╚██╗ ██╔╝██╔══██╗\n");
    printf("  ██║     ██║   ██║██╔████╔██║██████╔╝██║   ╚████╔╝ ██║  ██║\n");
    printf("  ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██║    ╚██╔╝  ██║  ██║\n");
    printf("  ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ███████╗██║   ██████╔╝\n");
    printf("   ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝   ╚═════╝ \n");
    printf("%s\n", COLOR_RESET);
    printf("%s                    Complyd Scanner v1.0%s\n\n", COLOR_BOLD, COLOR_RESET);
}

int main(int argc, char *argv[]) {
    print_banner();
    
    // Check command line arguments
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    
    // Detect and display file type
    file_type_t file_type = detect_file_type(filename);
    const char *file_type_str = "Unknown";
    
    switch (file_type) {
        case FILE_TYPE_MD: file_type_str = "Markdown"; break;
        case FILE_TYPE_JSON: file_type_str = "JSON"; break;
        case FILE_TYPE_PDF: file_type_str = "PDF"; break;
        case FILE_TYPE_YAML: file_type_str = "YAML"; break;
        case FILE_TYPE_TEXT: file_type_str = "Text"; break;
        default: file_type_str = "Unknown"; break;
    }
    
    printf("%sScanning file:%s %s\n", COLOR_BOLD, COLOR_RESET, filename);
    printf("%sFile type:%s %s\n", COLOR_BOLD, COLOR_RESET, file_type_str);
    
    // Parse the file
    print_box_header("PARSING CONFIGURATION FILE");
    
    parse_result_t *parse_result = parse_file(filename);
    
    if (!parse_result || !parse_result->success) {
        fprintf(stderr, "%sError parsing file:%s %s\n", 
                COLOR_RED, COLOR_RESET, 
                parse_result && parse_result->error_message ? parse_result->error_message : "Unknown error");
        
        if (parse_result) {
            free_parse_result(parse_result);
        }
        return 1;
    }
    
    printf("%s✓ Successfully parsed file (%zu bytes)%s\n", 
           COLOR_GREEN, parse_result->content_length, COLOR_RESET);
    
    // Display parsed content (first 500 chars)
    printf("\n%sParsed Configuration (preview):%s\n", COLOR_BOLD, COLOR_RESET);
    print_line('-', 80);
    
    size_t preview_len = parse_result->content_length < 500 ? parse_result->content_length : 500;
    printf("%s%.*s%s", COLOR_YELLOW, (int)preview_len, parse_result->content, COLOR_RESET);
    if (parse_result->content_length > 500) {
        printf("\n... (truncated)");
    }
    printf("\n");
    print_line('-', 80);
    
    // Run HIPAA compliance checks
    print_box_header("RUNNING HIPAA COMPLIANCE CHECKS");
    
    scan_result_t *scan_result = hipaa_scan_config(parse_result->content);
    
    if (!scan_result) {
        fprintf(stderr, "%sError: Scan failed%s\n", COLOR_RED, COLOR_RESET);
        free_parse_result(parse_result);
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
    printf("  File:            %s%s%s\n", COLOR_BOLD, filename, COLOR_RESET);
    printf("  File Type:       %s%s%s\n", COLOR_BOLD, file_type_str, COLOR_RESET);
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
    free_parse_result(parse_result);
    
    return compliance_score >= 80.0 ? 0 : 1;
}
