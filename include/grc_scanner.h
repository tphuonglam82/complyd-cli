#ifndef GRC_SCANNER_H
#define GRC_SCANNER_H

#include <stddef.h>
#include <stdbool.h>

// Configuration item structure
typedef struct {
    char *key;
    char *value;
} config_item_t;

// Configuration structure
typedef struct {
    config_item_t *items;
    size_t count;
    size_t capacity;
} config_t;

// Scanner core functions
config_t* scanner_load_config(const char *filepath);
void scanner_free_config(config_t *config);
char* config_to_string(const config_t *config);

#endif // GRC_SCANNER_H