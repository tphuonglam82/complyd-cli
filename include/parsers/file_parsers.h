#ifndef FILE_PARSERS_H
#define FILE_PARSERS_H

#include <stddef.h>

// File types supported
typedef enum {
    FILE_TYPE_UNKNOWN = 0,
    FILE_TYPE_MD,
    FILE_TYPE_JSON,
    FILE_TYPE_PDF,
    FILE_TYPE_YAML,
    FILE_TYPE_TEXT
} file_type_t;

// Parser result structure
typedef struct {
    char *content;           // Parsed content as text
    size_t content_length;   // Length of content
    int success;             // 1 if parsing succeeded, 0 otherwise
    char *error_message;     // Error message if parsing failed
} parse_result_t;

// Function declarations for file parsers
file_type_t detect_file_type(const char *filename);
parse_result_t* parse_file(const char *filename);
parse_result_t* parse_md_file(const char *filename);
parse_result_t* parse_json_file(const char *filename);
parse_result_t* parse_pdf_file(const char *filename);
void free_parse_result(parse_result_t *result);

// Helper function to read entire file
char* read_file_contents(const char *filename, size_t *length);

#endif // FILE_PARSERS_H
