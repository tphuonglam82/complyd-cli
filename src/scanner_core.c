#define _POSIX_C_SOURCE 200809L
#include "grc_scanner.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Load configuration from file (basic implementation)
config_t* scanner_load_config(const char *filepath) {
    if (!filepath) return NULL;
    
    FILE *fp = fopen(filepath, "r");
    if (!fp) return NULL;
    
    config_t *config = calloc(1, sizeof(config_t));
    if (!config) {
        fclose(fp);
        return NULL;
    }
    
    config->capacity = 10;
    config->items = calloc(config->capacity, sizeof(config_item_t));
    config->count = 0;
    
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;
        
        // Parse key: value
        char *colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            char *key = line;
            char *value = colon + 1;
            
            // Trim whitespace
            while (*value == ' ') value++;
            char *end = value + strlen(value) - 1;
            while (end > value && (*end == '\n' || *end == ' ')) *end-- = '\0';
            
            // Add to config
            if (config->count >= config->capacity) {
                config->capacity *= 2;
                config->items = realloc(config->items, 
                                      config->capacity * sizeof(config_item_t));
            }
            
            config->items[config->count].key = strdup(key);
            config->items[config->count].value = strdup(value);
            config->count++;
        }
    }
    
    fclose(fp);
    return config;
}

// Free configuration
void scanner_free_config(config_t *config) {
    if (!config) return;
    
    for (size_t i = 0; i < config->count; i++) {
        free(config->items[i].key);
        free(config->items[i].value);
    }
    free(config->items);
    free(config);
}

// Convert config to string
char* config_to_string(const config_t *config) {
    if (!config) return NULL;
    
    size_t total_size = 0;
    for (size_t i = 0; i < config->count; i++) {
        total_size += strlen(config->items[i].key) + 
                     strlen(config->items[i].value) + 3; // ": \n"
    }
    
    char *result = calloc(total_size + 1, 1);
    if (!result) return NULL;
    
    char *ptr = result;
    for (size_t i = 0; i < config->count; i++) {
        ptr += sprintf(ptr, "%s: %s\n", 
                      config->items[i].key, 
                      config->items[i].value);
    }
    
    return result;
}