#ifndef HIPAA_H
#define HIPAA_H

#include <stdbool.h>
#include <stddef.h>

// HIPAA Control structure
typedef struct {
    char *id;
    char *name;
    char *description;
    char *category;
} hipaa_control_t;

// HIPAA Framework structure
typedef struct {
    hipaa_control_t *controls;
    size_t control_count;
    size_t control_capacity;
} hipaa_framework_t;

// Check result structure
typedef struct {
    bool passed;
    char *control_id;
    char *control_name;
    const char *severity;  // "CRITICAL", "HIGH", "MEDIUM", "LOW", "INFO"
    char *details;
    char *remediation;
} check_result_t;

// Scan result structure
typedef struct {
    check_result_t **results;
    size_t result_count;
    size_t passed_count;
    size_t failed_count;
} scan_result_t;

// Framework loader functions
hipaa_framework_t* hipaa_load_framework(const char *yaml_file);
void hipaa_free_framework(hipaa_framework_t *framework);

// Check functions - return 1 (true) if passed, 0 (false) if failed
int hipaa_check_encryption_at_rest(const char *config_data);
int hipaa_check_audit_controls(const char *config_data);
int hipaa_check_authentication(const char *config_data);
int hipaa_check_encryption_in_transit(const char *config_data);
int hipaa_check_unique_user_id(const char *config_data);
int hipaa_check_data_backup(const char *config_data);
int hipaa_check_access_termination(const char *config_data);
int hipaa_check_auto_logoff(const char *config_data);

// Result creator functions
check_result_t* create_hipaa_encryption_result(int passed, const char *details);
check_result_t* create_hipaa_audit_result(int passed, const char *details);
check_result_t* create_hipaa_mfa_result(int passed, const char *details);
check_result_t* create_hipaa_transit_encryption_result(int passed, const char *details);
check_result_t* create_hipaa_user_id_result(int passed, const char *details);
check_result_t* create_hipaa_backup_result(int passed, const char *details);
check_result_t* create_hipaa_termination_result(int passed, const char *details);
check_result_t* create_hipaa_logoff_result(int passed, const char *details);

// Scanner function
scan_result_t* hipaa_scan_config(const char *config_data);

// Cleanup functions
void free_check_result(check_result_t *result);
void free_scan_result(scan_result_t *result);

#endif // HIPAA_H