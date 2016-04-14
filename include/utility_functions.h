#ifndef UTILITY_FUNCTIONS_DEFINED
#define UTILITY_FUNCTIONS_DEFINED

/* utils_dict.c */

typedef struct hash_element hash_element;

int dict_define(hash_element ** array_head, size_t size);
char * dict_get(char * key, hash_element * hash_array);

/* Used to put new key - value pairs into a defined hash_array/dict.
 * Return values:
 *   0: everything went fine.
 *  -1: could not allocate the necessary memory.
 */
int dict_put(char * key, char * value, hash_element * hash_array);

#endif
