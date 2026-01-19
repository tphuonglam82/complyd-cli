#define _POSIX_C_SOURCE 200809L
#include "frameworks/hipaa.h"
#include <string.h>
#include <stdlib.h>

// ==================== CHECK 1: Encryption at Rest ====================
int hipaa_check_encryption_at_rest(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "encryption: enabled") ||
        strstr(config_data, "encrypt_at_rest: true") ||
        strstr(config_data, "kms_key_id:") ||
        strstr(config_data, "server_side_encryption") ||
        strstr(config_data, "encrypted: true")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_encryption_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.312(a)(2)(iv)");
    result->control_name = strdup("Encryption and Decryption");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ? 
        "Encryption at rest is enabled" : 
        "Encryption at rest is NOT enabled");
    result->remediation = passed ? NULL : 
        strdup("Enable encryption at rest using KMS or equivalent encryption service");
    return result;
}

// ==================== CHECK 2: Audit Controls ====================
int hipaa_check_audit_controls(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "audit_log: enabled") ||
        strstr(config_data, "cloudtrail: enabled") ||
        strstr(config_data, "logging: true") ||
        strstr(config_data, "audit_enabled: true") ||
        strstr(config_data, "monitoring: enabled")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_audit_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.312(b)");
    result->control_name = strdup("Audit Controls");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Audit logging is enabled" :
        "Audit logging is NOT enabled");
    result->remediation = passed ? NULL :
        strdup("Enable comprehensive audit logging and monitoring for all system activities");
    return result;
}

// ==================== CHECK 3: Authentication (MFA) ====================
int hipaa_check_authentication(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "mfa_enabled: true") ||
        strstr(config_data, "multi_factor: true") ||
        strstr(config_data, "require_mfa: true") ||
        strstr(config_data, "2fa_required: true") ||
        strstr(config_data, "mfa: enforced")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_mfa_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.312(d)");
    result->control_name = strdup("Person or Entity Authentication");
    result->severity = passed ? "INFO" : "CRITICAL";
    result->details = details ? strdup(details) : strdup(passed ?
        "Multi-factor authentication is enabled" :
        "Multi-factor authentication is NOT enabled");
    result->remediation = passed ? NULL :
        strdup("Implement Multi-Factor Authentication (MFA) for all user accounts accessing PHI");
    return result;
}

// ==================== CHECK 4: Encryption in Transit ====================
int hipaa_check_encryption_in_transit(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "tls: enabled") ||
        strstr(config_data, "ssl_enabled: true") ||
        strstr(config_data, "https_only: true") ||
        strstr(config_data, "enforce_ssl: true") ||
        strstr(config_data, "tls_version: 1.2") ||
        strstr(config_data, "tls_version: 1.3")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_transit_encryption_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.312(e)(2)(ii)");
    result->control_name = strdup("Transmission Security - Encryption");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Encryption in transit is enabled" :
        "Encryption in transit is NOT enabled");
    result->remediation = passed ? NULL :
        strdup("Enable TLS 1.2 or higher for all data transmission");
    return result;
}

// ==================== CHECK 5: Unique User Identification ====================
int hipaa_check_unique_user_id(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "unique_user_id: true") ||
        strstr(config_data, "user_identification: enforced") ||
        strstr(config_data, "iam_enabled: true") ||
        strstr(config_data, "individual_accounts: true")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_user_id_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.312(a)(2)(i)");
    result->control_name = strdup("Unique User Identification");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Unique user identification is enforced" :
        "Unique user identification is NOT enforced");
    result->remediation = passed ? NULL :
        strdup("Implement unique user identification for all system access - no shared accounts");
    return result;
}

// ==================== CHECK 6: Data Backup ====================
int hipaa_check_data_backup(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "backup: enabled") ||
        strstr(config_data, "backup_enabled: true") ||
        strstr(config_data, "automated_backup: true") ||
        strstr(config_data, "disaster_recovery: enabled")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_backup_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.308(a)(7)(ii)(A)");
    result->control_name = strdup("Data Backup Plan");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Data backup is configured" :
        "Data backup is NOT configured");
    result->remediation = passed ? NULL :
        strdup("Establish automated backup procedures with regular testing");
    return result;
}

// ==================== CHECK 7: Access Termination ====================
int hipaa_check_access_termination(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "access_termination: automated") ||
        strstr(config_data, "offboarding: enabled") ||
        strstr(config_data, "account_lifecycle: managed")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_termination_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.308(a)(3)(ii)(C)");
    result->control_name = strdup("Termination Procedures");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Access termination procedures are in place" :
        "Access termination procedures are NOT configured");
    result->remediation = passed ? NULL :
        strdup("Implement automated access termination procedures for departing personnel");
    return result;
}

// ==================== CHECK 8: Auto Logoff ====================
int hipaa_check_auto_logoff(const char* config_data) {
    if (!config_data) return 0;
    
    if (strstr(config_data, "auto_logoff: enabled") ||
        strstr(config_data, "session_timeout:") ||
        strstr(config_data, "idle_timeout:")) {
        return 1;
    }
    return 0;
}

check_result_t* create_hipaa_logoff_result(int passed, const char* details) {
    check_result_t* result = calloc(1, sizeof(check_result_t));
    if (!result) return NULL;
    
    result->passed = passed;
    result->control_id = strdup("164.312(a)(2)(iii)");
    result->control_name = strdup("Automatic Logoff");
    result->severity = passed ? "INFO" : "LOW";
    result->details = details ? strdup(details) : strdup(passed ?
        "Automatic logoff is configured" :
        "Automatic logoff is NOT configured");
    result->remediation = passed ? NULL :
        strdup("Configure automatic session termination after period of inactivity");
    return result;
}

// ==================== Cleanup Function ====================
void free_check_result(check_result_t* result) {
    if (!result) return;
    
    if (result->control_id) free(result->control_id);
    if (result->control_name) free(result->control_name);
    if (result->details) free(result->details);
    if (result->remediation) free(result->remediation);
    
    free(result);
}