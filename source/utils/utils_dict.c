#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utility_functions.h"

#define DEFAULT_SIZE 256;
#define HASH_PRIME 31;

typedef struct Node {
    struct Node * next;
    char * key;
    char * value;
} Node;

typedef struct Dictionary {
    size_t size;
    Node ** node_array;
} Dictionary;

size_t length_array(Dictionary * dictionary) {
    return dictionary->size;
}

unsigned int get_hash(char * string, size_t array_size) {
    unsigned int hash_value = 0;
    for(; *string != '\0'; string++) {
        hash_value = *string + hash_value * HASH_PRIME;
    }
    hash_value = hash_value % array_size;
    return hash_value;
}

Dictionary * dictionary_create(size_t size) {
    /* Create a dictionary structure of size 'size'.
     * If size is <= 0, use DEFAULT_SIZE.
     */
    size_t array_size = 0;
    if (size <= 0) {
        array_size = DEFAULT_SIZE;
    } else {
        array_size = size;
    }
    Dictionary * dictionary = malloc(sizeof(Dictionary));
    if (!dictionary) {
        return NULL;
    }
    dictionary->size = array_size;
    dictionary->node_array = calloc(sizeof(Node), array_size);
    return dictionary;
}

Node * iterate(char * key, Dictionary * dictionary) {
    /* Iterate over all elements until a matching key is found or the end of
     * the liked list is reached. Return that hash element at that position. */

    unsigned int hash_value = get_hash(key, length_array(dictionary));
    Node * current_node = dictionary->node_array[hash_value];

    char * current_key = "";

    while(current_node != NULL) {

        current_key = current_node->key;

        if (strcmp(current_key, key) == 0) {
            /* Found matching key, return corresponding hash element. */
            return current_node;
        }
        /* Continue with next element in linked list. */
        current_node = current_node->next;
    }
    /* No matching key found, return end of list. */
    return current_node;
}

void dictionary_free(Dictionary * dict) {
    /* Deallocate all array indexes from 0->dict_size.
     * Iterate through the linked lists at each index and deallocate their
     * memory.
     */

    size_t dict_size = length_array(dict);
    Node * current_node = NULL;
    Node * next_node = NULL;

    for(size_t i=0; i<dict_size; i++) {
        current_node = dict->node_array[i];
        while(current_node != NULL) {
            next_node = current_node->next;
            free(current_node->key);
            free(current_node->value);
            free(current_node);
            current_node = next_node;
        }
    }
    free(dict->node_array);
}

char * dictionary_get(char * key, Dictionary * dict) {
    /* Return value of retrieved dictionary element or NULL if nothing was
     * found.
     * */
    Node * returned_node = iterate(key, dict);
    if (returned_node == NULL) {
        return NULL;
    }
    return returned_node->value;
}

int dictionary_put(char * key, char * value, Dictionary * dictionary) {

    size_t length_key =  strlen(key);
    size_t length_value = strlen(value);

    Node * returned_node = iterate(key, dictionary);

    if (returned_node == NULL) {
        /* No key found, create a new one at head of list.
         *
         * Allocate new memory for new key, check for null, copy string data,
         * and ensure null termination. */

        unsigned int head_index = get_hash(key, length_array(dictionary));
        /* Create new Node to use as head. */
        Node * new_head = malloc(sizeof(Node));

        /* Replace head at head_index, store old head as new_head next. */
        new_head->next = dictionary->node_array[head_index];
        dictionary->node_array[head_index] = new_head;

        new_head->key = malloc(sizeof(char)*(length_key+1));
        if (!new_head->key) {
            return -1;
        }
        memcpy(new_head->key, key, length_key);
        (new_head->key)[length_key] = '\0';

        /* Allocate memory for new value, check for null, copy string data,
         * and ensure null termination. */

        new_head->value = malloc(sizeof(char)*(length_value+1));
        if (!new_head->value) {
            return -1;
        }
        memcpy(new_head->value, value, length_value);
        (new_head->value)[length_value] = '\0';

        return 0;
    } else {
        /* Key does exist, replace old value with new value. */
        char ** current_value = &returned_node->value;
        *current_value = realloc(*current_value, length_value+1);
        *current_value = memcpy(*current_value, value, length_value);
        (*current_value)[length_value] = '\0';
        return 0;
    }

    return 0;
}
