#ifndef UTILITY_FUNCTIONS_DEFINED
#define UTILITY_FUNCTIONS_DEFINED

/* utils_dict.c */

typedef struct dictionary dictionary;

dictionary * dictionary_create(size_t dict_size);

/* Used to put new key - value pairs into a defined hash_array/dict.
 * Return values:
 *   0: everything went fine.
 *  -1: could not allocate the necessary memory.
 */
int dictionary_put(char * key, char * value, dictionary * dict);

char * dictionary_get(char * key, dictionary * dict);

#endif
