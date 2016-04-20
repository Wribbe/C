#ifndef UTILITY_FUNCTIONS_DEFINED
#define UTILITY_FUNCTIONS_DEFINED

#include <stdbool.h>
#include <math.h>

/* Macro definitions. */
#define ERROR(message) do { fprintf(stderr, "ERROR: [%s:%s]: %s\n", __FILE__,__func__,message); return -1; } while(0)

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

/* Configure parser */

Dictionary * parse_config(const char * config_filename);

/* Math related functions */

typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];

void mat4_mul(mat4 res, mat4 A, mat4 B);
void mat3_mul(mat3 res, mat3 A, mat3 B);
void mat2_mul(mat2 res, mat2 A, mat2 B);

bool mat4_cmp(mat4 A, mat4 B);
bool mat3_cmp(mat3 A, mat3 B);
bool mat2_cmp(mat2 A, mat2 B);

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];


#endif
