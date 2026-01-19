#define _POSIX_C_SOURCE 200809L
#include "frameworks/hipaa.h"
#include <stdlib.h>
#include <string.h>

// Scan configuration against HIPAA compliance checks
scan_result_t* hipaa_scan_config(const char *config_data) {
    if (!config_data) {
        return NULL;
    }
    
    // Allocate scan result
    scan_result_t *result = calloc(1, sizeof(scan_result_t));
    if (!result) {
        return NULL;
    }
    
    // Allocate results array (8 checks)
    result->results = calloc(8, sizeof(check_result_t*));
    if (!result->results) {
        free(result);
        return NULL;
    }
    
    result->result_count = 0;
    result->passed_count = 0;
    result->failed_count = 0;
    
    // Check 1: Encryption at Rest
    int check1 = hipaa_check_encryption_at_rest(config_data);
    result->results[result->result_count++] = create_hipaa_encryption_result(check1, NULL);
    if (check1) result->passed_count++; else result->failed_count++;
    
    // Check 2: Audit Controls
    int check2 = hipaa_check_audit_controls(config_data);
    result->results[result->result_count++] = create_hipaa_audit_result(check2, NULL);
    if (check2) result->passed_count++; else result->failed_count++;
    
    // Check 3: Authentication (MFA)
    int check3 = hipaa_check_authentication(config_data);
    result->results[result->result_count++] = create_hipaa_mfa_result(check3, NULL);
    if (check3) result->passed_count++; else result->failed_count++;
    
    // Check 4: Encryption in Transit
    int check4 = hipaa_check_encryption_in_transit(config_data);
    result->results[result->result_count++] = create_hipaa_transit_encryption_result(check4, NULL);
    if (check4) result->passed_count++; else result->failed_count++;
    
    // Check 5: Unique User ID
    int check5 = hipaa_check_unique_user_id(config_data);
    result->results[result->result_count++] = create_hipaa_user_id_result(check5, NULL);
    if (check5) result->passed_count++; else result->failed_count++;
    
    // Check 6: Data Backup
    int check6 = hipaa_check_data_backup(config_data);
    result->results[result->result_count++] = create_hipaa_backup_result(check6, NULL);
    if (check6) result->passed_count++; else result->failed_count++;
    
    // Check 7: Access Termination
    int check7 = hipaa_check_access_termination(config_data);
    result->results[result->result_count++] = create_hipaa_termination_result(check7, NULL);
    if (check7) result->passed_count++; else result->failed_count++;
    
    // Check 8: Auto Logoff
    int check8 = hipaa_check_auto_logoff(config_data);
    result->results[result->result_count++] = create_hipaa_logoff_result(check8, NULL);
    if (check8) result->passed_count++; else result->failed_count++;
    
    return result;
}

// Free scan result
void free_scan_result(scan_result_t *result) {
    if (!result) return;
    
    if (result->results) {
        for (size_t i = 0; i < result->result_count; i++) {
            free_check_result(result->results[i]);
        }
        free(result->results);
    }
    
    free(result);
}