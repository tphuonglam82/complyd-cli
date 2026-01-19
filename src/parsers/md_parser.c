#define _POSIX_C_SOURCE 200809L
#include "parsers/file_parsers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Parse Markdown file - extract content and convert to plain text
parse_result_t* parse_md_file(const char *filename) {
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
        result->error_message = strdup("Failed to read MD file");
        return result;
    }
    
    // For Markdown, we'll do simple processing:
    // 1. Remove markdown headers (#, ##, etc.) but keep the text
    // 2. Remove markdown formatting (**, *, _, etc.)
    // 3. Remove code block markers (```)
    // 4. Keep the essential configuration text
    
    char *processed = malloc(file_length + 1);
    if (!processed) {
        free(file_content);
        result->success = 0;
        result->error_message = strdup("Memory allocation failed");
        return result;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    int in_code_block = 0;
    int at_line_start = 1;
    
    while (in_pos < file_length) {
        char c = file_content[in_pos];
        
        // Handle code blocks
        if (at_line_start && in_pos + 2 < file_length &&
            file_content[in_pos] == '`' &&
            file_content[in_pos + 1] == '`' &&
            file_content[in_pos + 2] == '`') {
            in_code_block = !in_code_block;
            in_pos += 3;
            // Skip to end of line
            while (in_pos < file_length && file_content[in_pos] != '\n') {
                in_pos++;
            }
            if (in_pos < file_length) {
                in_pos++;
            }
            at_line_start = 1;
            continue;
        }
        
        // If in code block, copy as-is
        if (in_code_block) {
            processed[out_pos++] = c;
            at_line_start = (c == '\n');
            in_pos++;
            continue;
        }
        
        // Remove markdown headers
        if (at_line_start && c == '#') {
            // Skip all # characters and following spaces
            while (in_pos < file_length && 
                   (file_content[in_pos] == '#' || file_content[in_pos] == ' ')) {
                in_pos++;
            }
            at_line_start = 0;
            continue;
        }
        
        // Remove bold/italic markers (but not underscores in words)
        if (c == '*') {
            // Skip one or two asterisks
            in_pos++;
            if (in_pos < file_length && file_content[in_pos] == '*') {
                in_pos++;
            }
            continue;
        }
        
        // Handle underscore - only remove if it's a markdown emphasis marker
        // (at word boundary), not if it's part of a word like mfa_enabled
        if (c == '_') {
            // Check if it's emphasis (not surrounded by alphanumeric)
            int prev_alnum = (in_pos > 0 && isalnum(file_content[in_pos - 1]));
            int next_alnum = (in_pos + 1 < file_length && isalnum(file_content[in_pos + 1]));
            
            if (prev_alnum && next_alnum) {
                // It's part of a word like mfa_enabled, keep it
                processed[out_pos++] = c;
                at_line_start = 0;
                in_pos++;
                continue;
            } else {
                // It's markdown emphasis, skip it
                in_pos++;
                if (in_pos < file_length && file_content[in_pos] == '_') {
                    in_pos++;
                }
                continue;
            }
        }
        
        // Remove inline code markers
        if (c == '`') {
            in_pos++;
            continue;
        }
        
        // Copy regular character
        processed[out_pos++] = c;
        at_line_start = (c == '\n');
        in_pos++;
    }
    
    processed[out_pos] = '\0';
    
    result->content = processed;
    result->content_length = out_pos;
    result->success = 1;
    result->error_message = NULL;
    
    free(file_content);
    return result;
}
