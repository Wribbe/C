#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 256;
#define HASH_PRIME 31;

typedef struct hash_element {
    struct hash_element * next;
    char * key;
    union {
        char * value;
        int size;
    };
} hash_element;

unsigned int get_hash(char * string, size_t array_size) {
    unsigned int hash_value = 0;
    for(; *string != '\0'; string++) {
        hash_value = *string + hash_value * HASH_PRIME;
    }
    return hash_value % array_size;
}

int dict_define(hash_element ** array_head, size_t size) {
    size_t array_size = 0;
    if (size <= 0) {
        array_size = 1+DEFAULT_SIZE;
    } else {
        array_size = size+1;
    }
    hash_element * array = malloc(sizeof(hash_element)*array_size);
    array[0].size = array_size;
    //array[0].value = malloc(sizeof(char)*40);
    //strcpy(array[0].value, "This is a test.\n");
    *array_head = &array[0];
    return 0;
}

char * dict_get(char * key, hash_element * hash_array) {
    return "Return string from dict_get.";
}

int dict_put(char * key, char * value, hash_element * hash_array) {

    int size = hash_array[0].size;
    unsigned int hash_value = get_hash(key, size);

    int length_key =  strlen(key)+1;
    int length_value = strlen(value)+1;

    printf("length_key: %d, length_value: %d\n", length_key, length_value);

    hash_element * current_element_pointer;

    current_element_pointer = &(hash_array[hash_value]);
    while(current_element_pointer != NULL) {
        if (current_element_pointer->value == NULL) {
            /* Check if we are at last element in list. */

            /* Allocate memory for new key, check for null, copy string data,
             * and ensure null termination. */
            current_element_pointer->key = malloc(sizeof(char)*length_key);
            if (!current_element_pointer->key) {
                return -1;
            }
            strncpy(current_element_pointer->key, key, length_key);
            current_element_pointer->key[length_key] = '\0';

            /* Allocate memory for new value, check for null, copy string data,
             * and ensure null termination. */
            current_element_pointer->value = malloc(sizeof(char)*length_value);
            if (!current_element_pointer->value) {
                return -1;
            }
            strncpy(current_element_pointer->value, value, length_value);
            current_element_pointer->value[length_value] = '\0';

            /* Create new last element. */
            current_element_pointer->next = malloc(sizeof(hash_element));
            if (!current_element_pointer->next) {
                return -1;
            }
            /* Set new struct elements to NULL. */
            current_element_pointer->next->next = NULL;
            current_element_pointer->next->key = NULL;
            current_element_pointer->next->value = NULL;

            printf("hash: %d for %s returns value == NULL.\n", hash_value, key);
            return 0;
        } else if (strcmp(current_element_pointer->key, key) == 0) {
            /* Check if keys match. */
            printf("Found key that matches %s\n", key);
        }

        /* Continue with next element in linked list. */
        current_element_pointer = current_element_pointer->next;
    }
    return 0;
}
