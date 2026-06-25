#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

#define CONFIG_MAX_LIST_ITEMS 128
#define CONFIG_MAX_STRING_LENGTH 256

/**
 *  @brief
 *
 *  @param str
 *  @return
 */
char *config_trim (char *str);

/**
 *  @brief
 *
 *  @param line
 *  @return
 */
int config_is_data_line (char *line);

/**
 *  @brief
 *
 *  @param line
 *  @param key
 *  @return
 */
int config_line_has_key (const char *line, const char *key);

/**
 *  @brief
 *
 *  @param line
 *  @param out
 *  @param outSize
 *  @return
 */
int config_parse_string_value (const char *line, char *out, size_t outSize);

/**
 *  @brief
 *
 *  @param line
 *  @param out
 *  @return
 */
int config_parse_int_value (const char *line, int *out);

/**
 *  @brief
 *
 *  @param line
 *  @param out
 *  @return
 */
int config_parse_float_value (const char *line, float *out);

/**
 *  @brief
 *
 *  @param line
 *  @param out
 *  @return
 */
int config_parse_vec3_value (const char *line, float out[3]);

/**
 *  @brief
 *
 *  @param line
 *  @param out
 *  @return
 */
int config_parse_vec4_value (const char *line, float out[4]);

/**
 *  @brief
 *
 *  @param line
 *  @param items
 *  @param maxItems
 *  @return
 */
int config_parse_string_list_value (const char *line,
                                    char items[][CONFIG_MAX_STRING_LENGTH],
                                    int maxItems);

/**
 *  @brief
 *
 *  @param configPath
 *  @param key
 *  @param outLine
 *  @param outLineSize
 *  @return
 */
int config_find_line_by_key (const char *configPath, const char *key,
                             char *outLine, size_t outLineSize);

#endif // CONFIG_H