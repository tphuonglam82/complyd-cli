#define _POSIX_C_SOURCE 200809L
#include "parsers/file_parsers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Simple JSON to key-value text converter
// Converts JSON format like {"key": "value"} to key: value format
static void json_to_text_recursive(const char *json, size_t *pos, char **output, size_t *out_pos, size_t *out_size, int depth) {
    size_t len = strlen(json);
    
    while (*pos < len) {
        char c = json[*pos];
        
        // Skip whitespace
        if (isspace(c)) {
            (*pos)++;
            continue;
        }
        
        // Handle object start - recurse into nested objects
        if (c == '{') {
            (*pos)++;
            json_to_text_recursive(json, pos, output, out_pos, out_size, depth + 1);
            continue;
        }
        
        // Handle object end
        if (c == '}') {
            (*pos)++;
            if (depth > 0) {
                return;  // Return from nested object
            }
            continue;  // Continue at top level
        }
        
        // Handle array start
        if (c == '[') {
            (*pos)++;
            // Skip array contents for now (simplified)
            int bracket_count = 1;
            while (*pos < len && bracket_count > 0) {
                if (json[*pos] == '[') bracket_count++;
                if (json[*pos] == ']') bracket_count--;
                (*pos)++;
            }
            continue;
        }
        
        // Handle comma
        if (c == ',') {
            (*pos)++;
            continue;
        }
        
        // Handle string (key or value)
        if (c == '"') {
            (*pos)++;
            size_t start = *pos;
            
            // Find end of string
            while (*pos < len && json[*pos] != '"') {
                if (json[*pos] == '\\') {
                    (*pos)++;  // Skip escaped character
                }
                (*pos)++;
            }
            
            size_t str_len = *pos - start;
            
            // Ensure buffer is large enough
            while (*out_pos + str_len + 10 >= *out_size) {
                *out_size *= 2;
                *output = realloc(*output, *out_size);
            }
            
            // Copy string content
            memcpy(*output + *out_pos, json + start, str_len);
            *out_pos += str_len;
            
            if (*pos < len) (*pos)++;  // Skip closing quote
            
            // Skip whitespace
            while (*pos < len && isspace(json[*pos])) {
                (*pos)++;
            }
            
            // Check if this is a key (followed by colon)
            if (*pos < len && json[*pos] == ':') {
                (*pos)++;
                // Add colon and space  
                (*output)[(*out_pos)++] = ':';
                (*output)[(*out_pos)++] = ' ';
                
                // Skip whitespace after colon
                while (*pos < len && isspace(json[*pos])) {
                    (*pos)++;
                }
            } else {
                // This is a value, add newline
                (*output)[(*out_pos)++] = '\n';
            }
            
            continue;
        }
        
        // Handle boolean/number values
        if (isalnum(c) || c == '-' || c == '.') {
            size_t start = *pos;
            while (*pos < len && (isalnum(json[*pos]) || json[*pos] == '-' || json[*pos] == '.')) {
                (*pos)++;
            }
            
            size_t val_len = *pos - start;
            
            // Ensure buffer is large enough
            while (*out_pos + val_len + 10 >= *out_size) {
                *out_size *= 2;
                *output = realloc(*output, *out_size);
            }
            
            // Copy value
            memcpy(*output + *out_pos, json + start, val_len);
            *out_pos += val_len;
            (*output)[(*out_pos)++] = '\n';
            
            continue;
        }
        
        // Skip other characters
        (*pos)++;
    }
}

// Parse JSON file and convert to key-value format
parse_result_t* parse_json_file(const char *filename) {
    if (!filename) {
        return NULL;
    }
    
    parse_result_t *result = calloc(1, sizeof(parse_result_t));
    if (!result) {
        return NULL;
    }
    
    // Read the file
    size_t file_length = 0;
    char *file_content = read_file_contents(filename, &file_length);
    
    if (!file_content) {
        result->success = 0;
        result->error_message = strdup("Failed to read JSON file");
        return result;
    }
    
    // Allocate output buffer
    size_t out_size = file_length * 2;  // Start with 2x the input size
    char *output = malloc(out_size);
    if (!output) {
        free(file_content);
        result->success = 0;
        result->error_message = strdup("Memory allocation failed");
        return result;
    }
    
    // Parse JSON and convert to text format
    size_t pos = 0;
    size_t out_pos = 0;
    json_to_text_recursive(file_content, &pos, &output, &out_pos, &out_size, 0);
    
    output[out_pos] = '\0';
    
    result->content = output;
    result->content_length = out_pos;
    result->success = 1;
    result->error_message = NULL;
    
    free(file_content);
    return result;
}
