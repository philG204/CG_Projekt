#include "../../headers/utilities/config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
config_trim(char *str)
{
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') {
        return str;
    }

    char *end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    return str;
}

int
config_is_data_line(char *line)
{
    char *trimmed = config_trim(line);

    if (trimmed[0] == '\0') {
        return 0;
    }

    if (trimmed[0] == '/' && trimmed[1] == '/') {
        return 0;
    }

    return 1;
}

int
config_line_has_key(const char *line, const char *key)
{
    while (isspace((unsigned char)*line)) {
        line++;
    }

    size_t keyLength = strlen(key);

    if (strncmp(line, key, keyLength) != 0) {
        return 0;
    }

    line += keyLength;

    while (isspace((unsigned char)*line)) {
        line++;
    }

    return *line == '=';
}

int
config_parse_string_value(const char *line, char *out, size_t outSize)
{
    const char *equal = strchr(line, '=');

    if (equal == NULL || out == NULL || outSize == 0) {
        return 0;
    }

    equal++;

    while (isspace((unsigned char)*equal)) {
        equal++;
    }

    if (*equal == '"') {
        equal++;

        size_t len = 0;

        while (equal[len] != '\0' && equal[len] != '"') {
            len++;
        }

        if (len >= outSize) {
            len = outSize - 1;
        }

        strncpy(out, equal, len);
        out[len] = '\0';

        return 1;
    }

    size_t len = 0;

    while (equal[len] != '\0' &&
           equal[len] != ';' &&
           equal[len] != '\n' &&
           equal[len] != '\r') {
        len++;
    }

    while (len > 0 && isspace((unsigned char)equal[len - 1])) {
        len--;
    }

    if (len >= outSize) {
        len = outSize - 1;
    }

    strncpy(out, equal, len);
    out[len] = '\0';

    return 1;
}

int
config_parse_float_value(const char *line, float *out)
{
    printf("entering config_find_line_by_key\n");
    const char *equal = strchr(line, '=');

    if (equal == NULL || out == NULL) {
        return 0;
    }

    *out = strtof(equal + 1, NULL);

    return 1;
}

int
config_parse_vec3_value(const char *line, float out[3])
{
    const char *start = strchr(line, '[');
    const char *end = strchr(line, ']');

    if (start == NULL || end == NULL || end <= start || out == NULL) {
        return 0;
    }

    char buffer[256];

    size_t len = end - start - 1;

    if (len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1;
    }

    strncpy(buffer, start + 1, len);
    buffer[len] = '\0';

    char *token = strtok(buffer, ",");

    for (int i = 0; i < 3; i++) {
        if (token == NULL) {
            return 0;
        }

        token = config_trim(token);
        out[i] = strtof(token, NULL);

        token = strtok(NULL, ",");
    }

    return 1;
}

int
config_parse_string_list_value(const char *line,
                               char items[][CONFIG_MAX_STRING_LENGTH],
                               int maxItems)
{
    const char *start = strchr(line, '[');
    const char *end = strchr(line, ']');

    if (start == NULL || end == NULL || end <= start || items == NULL) {
        return 0;
    }

    char buffer[1024];

    size_t len = end - start - 1;

    if (len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1;
    }

    strncpy(buffer, start + 1, len);
    buffer[len] = '\0';

    int count = 0;

    char *token = strtok(buffer, ",");

    while (token != NULL && count < maxItems) {
        char *item = config_trim(token);

        if (*item == '"') {
            item++;
        }

        size_t itemLen = strlen(item);

        if (itemLen > 0 && item[itemLen - 1] == '"') {
            item[itemLen - 1] = '\0';
        }

        if (item[0] != '\0') {
            strncpy(items[count], item, CONFIG_MAX_STRING_LENGTH - 1);
            items[count][CONFIG_MAX_STRING_LENGTH - 1] = '\0';
            count++;
        }

        token = strtok(NULL, ",");
    }

    return count;
}

int
config_find_line_by_key(const char *configPath,
                        const char *key,
                        char *outLine,
                        size_t outLineSize)
{
    FILE *file = fopen(configPath, "r");

    if (file == NULL) {
        printf("config_find_line_by_key: config konnte nicht geöffnet werden: %s\n",
               configPath);
        return 0;
    }

    char line[512];

    while (fgets(line, sizeof(line), file) != NULL) {
        char *dataLine = config_trim(line);

        if (dataLine[0] == '\0') {
            continue;
        }

        if (dataLine[0] == '/' && dataLine[1] == '/') {
            continue;
        }

        if (config_line_has_key(dataLine, key)) {
            strncpy(outLine, dataLine, outLineSize - 1);
            outLine[outLineSize - 1] = '\0';

            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;
}