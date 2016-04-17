#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "utility_functions.h"

#define COMMENT_CHAR '#'
#define DELIMITER_STR "="

/* TODO: Split put_line_in_dictionary function into smaller functions. */

const char * strip_leading_whitespace(const char * string) {
    /* Iterate over spaces until character is found, returned new pointer
     * position.
     */
    for(; isspace(*string); string++) {}
    return string;
}

int put_line_in_dictionary(const char * line, Dictionary * dict) {
    /* Copy string into temporary char array and tokenize temporary char array.
     * Use tokens to populate the Dictionary.
     */
    size_t length_line = strlen(line);
    char * temp_string = malloc(sizeof(char)*(length_line+1));
    memcpy(temp_string, line, length_line);
    temp_string[length_line] = '\0';

    const char * key = strtok(temp_string, DELIMITER_STR);
    if (key == NULL) {
        return -1;
    }
    const char * value = strtok(NULL, DELIMITER_STR);
    if (value == NULL) {
        return -1;
    }

    size_t length_key = strlen(key);
    size_t length_value = strlen(value);

    char stripped_key[length_key+1];
    char stripped_value[length_value+1];

    /* Initialize stripped key and value to avoid initialization error in
     * valgrind. */
    for (size_t i=0; i<length_key; i++) {
        stripped_key[i] = '\0';
    }

    for (size_t i=0; i<length_value; i++) {
        stripped_value[i] = '\0';
    }

    key = strip_leading_whitespace(key);
    value = strip_leading_whitespace(value);

    size_t i;
    size_t current_pos = 0;
    bool string_literal = false;

    for(i=0; (string_literal || (!isspace(key[i]) && key[i] != COMMENT_CHAR)); i++) {
        if (key[i] == '"') {
            string_literal = !string_literal;
            continue;
        }
        stripped_key[current_pos++] = key[i];
        if (string_literal && i > length_key) {
            ERROR("Reading outside key length, unmatched '\"'?");
        }
    }
    if (i == 0) {
        ERROR("size of key is 0.");
    }
    stripped_key[current_pos] = '\0';

    current_pos = 0;
    string_literal = false;
    for(i=0; (string_literal || (!isspace(value[i]) && value[i] != COMMENT_CHAR)); i++) {
        if (value[i] == '"') {
            string_literal = !string_literal;
            continue;
        }
        stripped_value[current_pos++] = value[i];
        if (string_literal && i > length_value) {
            ERROR("Reading outside value length, unmatched '\"'?");
        }
    }
    if (i == 0) {
        ERROR("size of value is 0.");
    }
    stripped_value[current_pos] = '\0';

    free(temp_string);

    return dictionary_put(stripped_key, stripped_value, dict);
}

Dictionary * parse_config(const char * config_path) {
    FILE * config_pointer = fopen(config_path, "r");

    size_t line_buffer_size = 512;
    size_t length_line;
    char line_buffer[line_buffer_size];
    const char * stripped_line;

    int ret_value = 0;

    Dictionary * dict = dictionary_create(0);

    while(fgets(line_buffer, line_buffer_size, config_pointer)) {

        stripped_line = strip_leading_whitespace(line_buffer);
        length_line = strlen(stripped_line);

        if (length_line == 0) {
            continue;
        } else if (length_line == 1) {
            if (isspace(line_buffer[length_line-1])) {
                continue;
            }
        } else if (stripped_line[0] == COMMENT_CHAR) {
            continue;
        }

        ret_value = put_line_in_dictionary(stripped_line, dict);
        if (ret_value < 0) {
            const char * format = "ERROR: put_line_in_dictionary returned error value %d with line: '%s'.\n";
            printf(format, ret_value, stripped_line);
        }

    }

    fclose(config_pointer);
    return dict;
}
