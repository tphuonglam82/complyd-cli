#define _POSIX_C_SOURCE 200809L
#include "parsers/file_parsers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Read entire file contents into memory
char* read_file_contents(const char *filename, size_t *length) {
    if (!filename || !length) {
        return NULL;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return NULL;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (file_size < 0) {
        fclose(fp);
        return NULL;
    }
    
    // Allocate buffer
    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }
    
    // Read file
    size_t bytes_read = fread(buffer, 1, file_size, fp);
    fclose(fp);
    
    buffer[bytes_read] = '\0';
    *length = bytes_read;
    
    return buffer;
}

// Detect file type from filename extension
file_type_t detect_file_type(const char *filename) {
    if (!filename) {
        return FILE_TYPE_UNKNOWN;
    }
    
    const char *ext = strrchr(filename, '.');
    if (!ext) {
        return FILE_TYPE_TEXT;
    }
    
    ext++;  // Skip the dot
    
    // Convert to lowercase for comparison
    char ext_lower[10];
    size_t i;
    for (i = 0; i < sizeof(ext_lower) - 1 && ext[i]; i++) {
        ext_lower[i] = tolower(ext[i]);
    }
    ext_lower[i] = '\0';
    
    if (strcmp(ext_lower, "md") == 0 || strcmp(ext_lower, "markdown") == 0) {
        return FILE_TYPE_MD;
    } else if (strcmp(ext_lower, "json") == 0) {
        return FILE_TYPE_JSON;
    } else if (strcmp(ext_lower, "pdf") == 0) {
        return FILE_TYPE_PDF;
    } else if (strcmp(ext_lower, "yaml") == 0 || strcmp(ext_lower, "yml") == 0) {
        return FILE_TYPE_YAML;
    } else if (strcmp(ext_lower, "txt") == 0 || strcmp(ext_lower, "conf") == 0 || 
               strcmp(ext_lower, "config") == 0) {
        return FILE_TYPE_TEXT;
    }
    
    return FILE_TYPE_UNKNOWN;
}

// Parse file based on detected type
parse_result_t* parse_file(const char *filename) {
    if (!filename) {
        return NULL;
    }
    
    file_type_t type = detect_file_type(filename);
    
    switch (type) {
        case FILE_TYPE_MD:
            return parse_md_file(filename);
        
        case FILE_TYPE_JSON:
            return parse_json_file(filename);
        
        case FILE_TYPE_PDF:
            return parse_pdf_file(filename);
        
        case FILE_TYPE_TEXT:
        case FILE_TYPE_YAML:
        case FILE_TYPE_UNKNOWN:
        default: {
            // For text/unknown files, just read the content as-is
            parse_result_t *result = calloc(1, sizeof(parse_result_t));
            if (!result) {
                return NULL;
            }
            
            size_t length = 0;
            char *content = read_file_contents(filename, &length);
            
            if (!content) {
                result->success = 0;
                result->error_message = strdup("Failed to read file");
                return result;
            }
            
            result->content = content;
            result->content_length = length;
            result->success = 1;
            result->error_message = NULL;
            
            return result;
        }
    }
}

// Free parse result structure
void free_parse_result(parse_result_t *result) {
    if (!result) {
        return;
    }
    
    if (result->content) {
        free(result->content);
    }
    
    if (result->error_message) {
        free(result->error_message);
    }
    
    free(result);
}
