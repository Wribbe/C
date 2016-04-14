#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 256;
#define HASH_PRIME 31;

typedef struct dictionary {
    struct dictionary * next;
    char * key;
    union {
        char * value;
        int size;
    };
} dictionary;

size_t length_array(dictionary * hash_array) {
    return hash_array[0].size;
}

unsigned int get_hash(char * string, size_t array_size) {
    unsigned int hash_value = 0;
    for(; *string != '\0'; string++) {
        hash_value = *string + hash_value * HASH_PRIME;
    }
    /* Shift range from 0->n to 0->(n-1) and then add one to shift it to 1->n
     * to avoid the 0 position since that is where the size of the array is
     * stored. */
    hash_value = (hash_value % (array_size-1))+1;
    return hash_value;
}

dictionary * dictionary_create(size_t size) {
    size_t array_size = 0;
    if (size <= 0) {
        array_size = 1+DEFAULT_SIZE;
    } else {
        array_size = size+1;
    }
    dictionary * array = calloc(array_size, sizeof(dictionary));
    if (!array) {
        return NULL;
    }
    array[0].size = array_size;
    return array;
}

dictionary * iterate(char * key, dictionary * hash_array) {
    /* Iterate over all elements until a matching key is found or the end of
     * the liked list is reached. Return that hash element at that position. */

    unsigned int hash_value = get_hash(key, length_array(hash_array));
    dictionary * current_element_pointer = &(hash_array[hash_value]);

    char * current_key = "";

    while(current_element_pointer->value != NULL) {

        current_key = current_element_pointer->key;

        if (strcmp(current_key, key) == 0) {
            /* Found matching key, return corresponding hash element. */
            return current_element_pointer;
        }
        /* Continue with next element in linked list. */
        current_element_pointer = current_element_pointer->next;
    }
    /* No matching key found, return end of list. */
    return current_element_pointer;
}

char * dictionary_get(char * key, dictionary * hash_array) {
    return "Return string from dictionary_get.";
}

int dictionary_put(char * key, char * value, dictionary * hash_array) {

    size_t length_key =  strlen(key);
    size_t length_value = strlen(value);

    dictionary * returned_element = iterate(key, hash_array);

    if (returned_element->value == NULL) {
        /* No key found, create a new one.
         *
         * Allocate new memory for new key, check for null, copy string data,
         * and ensure null termination. */

        returned_element->key = malloc(sizeof(char)*(length_key+1));
        if (!returned_element->key) {
            return -1;
        }
        memcpy(returned_element->key, key, length_key);
        (returned_element->key)[length_key] = '\0';

        /* Allocate memory for new value, check for null, copy string data,
         * and ensure null termination. */

        returned_element->value = malloc(sizeof(char)*(length_value+1));
        if (!returned_element->value) {
            return -1;
        }
        memcpy(returned_element->value, value, length_value);
        (returned_element->value)[length_value] = '\0';

        /* Create new last element. */
        returned_element->next = malloc(sizeof(dictionary));
        if (returned_element->next) {
            return -1;
        }
        /* Set new struct elements to NULL. */
        dictionary * next_hash = returned_element->next;
        next_hash->next = NULL;
        next_hash->key = NULL;
        next_hash->value = NULL;

        return 0;
    } else {
        /* Key does exist, replace old value with new value. */
        char ** current_value = &returned_element->value;
        *current_value = realloc(*current_value, length_value+1);
        *current_value = memcpy(*current_value, value, length_value);
        (*current_value)[length_value] = '\0';
        return 0;
    }

    return 0;
}
