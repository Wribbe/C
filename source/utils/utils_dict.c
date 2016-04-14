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
    printf("Hashing %s, ", string);
    for(; *string != '\0'; string++) {
        hash_value = *string + hash_value * HASH_PRIME;
    }
    /* Shift range from 0->n to 0->(n-1) and then add one to shift it to 1->n
     * to avoid the 0 position since that is where the size of the array is
     * stored. */
    hash_value = (hash_value % (array_size-1))+1;
    printf("returning: %d\n", hash_value);
    return hash_value;
}

int dict_define(hash_element ** array_head, size_t size) {
    size_t array_size = 0;
    if (size <= 0) {
        array_size = 1+DEFAULT_SIZE;
    } else {
        array_size = size+1;
    }
    hash_element * array = calloc(array_size, sizeof(hash_element));
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

    size_t array_size = hash_array[0].size;
    unsigned int hash_value = get_hash(key, array_size);

    size_t length_key =  strlen(key);
    size_t length_value = strlen(value);

    printf("length_key: %zu, length_value: %zu\n", length_key, length_value);

    hash_element * current_element_pointer;

    char ** current_key = NULL;
    char ** current_value = NULL;
    hash_element ** current_next = NULL;

    current_element_pointer = &(hash_array[hash_value]);
    while(current_element_pointer != NULL) {
        /* Didn't find the key. */
        current_value = &current_element_pointer->value;
        current_key = &(current_element_pointer->key);
        if (*current_value == NULL) {

            /* Allocate memory for new key, check for null, copy string data,
             * and ensure null termination. */

            *current_key = malloc(sizeof(char)*length_key+1);
            if (!*current_key) {
                return -1;
            }
            memcpy(*current_key, key, length_key);
            (*current_key)[length_key] = '\0';

            /* Allocate memory for new value, check for null, copy string data,
             * and ensure null termination. */

            current_value = &(current_element_pointer->value);
            *current_value = malloc(sizeof(char)*length_value+1);
            if (!*current_value) {
                return -1;
            }
            memcpy(*current_value, value, length_value);
            (*current_value)[length_value] = '\0';

            /* Create new last element. */
            current_next = &(current_element_pointer->next);
            *current_next = malloc(sizeof(hash_element));
            if (!*current_next) {
                return -1;
            }
            /* Set new struct elements to NULL. */
            hash_element * next_hash = *current_next;
            next_hash->next = NULL;
            next_hash->key = NULL;
            next_hash->value = NULL;

            printf("hash: %d for %s returns value == NULL.\n", hash_value, key);
            return 0;
        /* Check if keys match. */
        } else if (strcmp(*current_key, key) == 0) {
            /* Replace the current value with the new value. */
            *current_value = realloc(*current_value, length_value);
            *current_value = memcpy(*current_value, value, length_value);
            (*current_value)[length_value] = '\0';
            return 0;
        }

        /* Continue with next element in linked list. */
        current_element_pointer = *current_next;
    }
    return 0;
}
