/**
 * @file config.h
 * @brief Configuration file parsing and value extraction utilities
 *
 * This module provides functions for parsing configuration files with
 * key-value pairs and extracting various data types (strings, integers,
 * floats, vectors).
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

#define CONFIG_MAX_LIST_ITEMS 128
#define CONFIG_MAX_STRING_LENGTH 256

/**
 * @brief Remove leading and trailing whitespace from a string
 *
 * @param str Pointer to the string to trim
 * @return Pointer to trimmed string (possibly starting past str)
 */
char *config_trim (char *str);

/**
 * @brief Check if a line contains data (not a comment or empty)
 *
 * @param line The line to check
 * @return 1 if line is a data line, 0 otherwise
 */
int config_is_data_line (char *line);

/**
 * @brief Check if a line contains a specific key
 *
 * @param line The configuration line to search
 * @param key The key to look for
 * @return 1 if key is found in the line, 0 otherwise
 */
int config_line_has_key (const char *line, const char *key);

/**
 * @brief Parse a string value from a configuration line
 *
 * Extracts the string value from a "key=value" line format.
 *
 * @param line The configuration line
 * @param out Pointer to output buffer for the parsed string
 * @param outSize Size of the output buffer
 * @return 1 if parsing succeeded, 0 otherwise
 */
int config_parse_string_value (const char *line, char *out, size_t outSize);

/**
 * @brief Parse an integer value from a configuration line
 *
 * @param line The configuration line
 * @param out Pointer to output integer where result is stored
 * @return 1 if parsing succeeded, 0 otherwise
 */
int config_parse_int_value (const char *line, int *out);

/**
 * @brief Parse a floating-point value from a configuration line
 *
 * @param line The configuration line
 * @param out Pointer to output float where result is stored
 * @return 1 if parsing succeeded, 0 otherwise
 */
int config_parse_float_value (const char *line, float *out);

/**
 * @brief Parse a 3D vector value from a configuration line
 *
 * Extracts three float values representing x, y, z coordinates.
 *
 * @param line The configuration line
 * @param out Pointer to output float array (must hold at least 3 floats)
 * @return 1 if parsing succeeded, 0 otherwise
 */
int config_parse_vec3_value (const char *line, float *out);

/**
 * @brief Parse a 4D vector value from a configuration line
 *
 * Extracts four float values representing x, y, z, w coordinates or RGBA
 * values.
 *
 * @param line The configuration line
 * @param out Pointer to output float array (must hold at least 4 floats)
 * @return 1 if parsing succeeded, 0 otherwise
 */
int config_parse_vec4_value (const char *line, float *out);

/**
 * @brief Parse a list of strings from a configuration line
 *
 * Extracts multiple comma-separated string values from a line.
 *
 * @param line The configuration line
 * @param items Pointer to 2D array for storing parsed strings
 * @param maxItems Maximum number of items to parse
 * @return Number of items parsed
 */
int config_parse_string_list_value (const char *line,
                                    char items[][CONFIG_MAX_STRING_LENGTH],
                                    int maxItems);

/**
 * @brief Find a configuration line containing a specific key
 *
 * Searches through a configuration file and returns the complete line
 * containing the specified key.
 *
 * @param configPath Path to the configuration file
 * @param key The key to search for
 * @param outLine Pointer to output buffer for the matching line
 * @param outLineSize Size of the output buffer
 * @return 1 if key was found, 0 otherwise
 */
int config_find_line_by_key (const char *configPath, const char *key,
                             char *outLine, size_t outLineSize);

#endif // CONFIG_H