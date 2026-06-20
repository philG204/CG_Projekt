#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

#define CONFIG_MAX_LIST_ITEMS 128
#define CONFIG_MAX_STRING_LENGTH 256

char *
config_trim(char *str);

int
config_is_data_line(char *line);

int
config_line_has_key(const char *line, const char *key);

int
config_parse_string_value(const char *line, char *out, size_t outSize);

int
config_parse_int_value(const char *line, int *out);

int
config_parse_float_value(const char *line, float *out);

int
config_parse_vec3_value(const char *line, float out[3]);

int
config_parse_vec4_value(const char *line, float out[4]);

int
config_parse_string_list_value(const char *line,
                               char items[][CONFIG_MAX_STRING_LENGTH],
                               int maxItems);

int
config_find_line_by_key(const char *configPath,
                        const char *key,
                        char *outLine,
                        size_t outLineSize);

#endif // CONFIG_H