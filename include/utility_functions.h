#ifndef UTILITY_FUNCTIONS_DEFINED
#define UTILITY_FUNCTIONS_DEFINED

typedef struct Dictionary Dictionary;

Dictionary * dictionary_create(size_t dict_size);

/* Used to put new key - value pairs into a defined hash_array/dict.
 * Return values:
 *   0: everything went fine.
 *  -1: could not allocate the necessary memory.
 */
int dictionary_put(const char * key, const char * value, Dictionary * dict);

char * dictionary_get(const char * key, Dictionary * dict);

void dictionary_free(Dictionary * dict);

#endif
