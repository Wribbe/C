#ifndef UTILITY_FUNCTIONS_DEFINED
#define UTILITY_FUNCTIONS_DEFINED

#include <stdbool.h>
#include <math.h>

/* Global Constants */

#define MATORD GL_TRUE

/* Macro definitions. */
#define ERROR(message) do { fprintf(stderr, "ERROR: [%s:%s]: %s\n", __FILE__,__func__,message); return -1; } while(0)

#define mat_ptr(matrix) &matrix[0][0]

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

/* Type definitions. */

typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

#define m4id (mat4){ \
        {1.0f, 0.0f, 0.0f, 0.0f}, \
        {0.0f, 1.0f, 0.0f, 0.0f}, \
        {0.0f, 0.0f, 1.0f, 0.0f}, \
        {0.0f, 0.0f, 0.0f, 1.0f}, \
    }

#define m3id (mat3){ \
        {1.0f, 0.0f, 0.0f}, \
        {0.0f, 1.0f, 0.0f}, \
        {0.0f, 0.0f, 1.0f}, \
    }

#define m2id (mat2){ \
        {1.0f, 0.0f}, \
        {0.0f, 1.0f}, \
    }

/* Matrix functions. */

void mat4_mul(mat4 res, mat4 A, mat4 B);
void mat3_mul(mat3 res, mat3 A, mat3 B);
void mat2_mul(mat2 res, mat2 A, mat2 B);

void mat4_div(mat4 res, mat4 A, mat4 B);
void mat3_div(mat3 res, mat3 A, mat3 B);
void mat2_div(mat2 res, mat2 A, mat2 B);

void mat4_add(mat4 res, mat4 A, mat4 B);
void mat3_add(mat3 res, mat3 A, mat3 B);
void mat2_add(mat2 res, mat2 A, mat2 B);

void mat4_sub(mat4 res, mat4 A, mat4 B);
void mat3_sub(mat3 res, mat3 A, mat3 B);
void mat2_sub(mat2 res, mat2 A, mat2 B);

bool mat4_cmp(mat4 A, mat4 B);
bool mat3_cmp(mat3 A, mat3 B);
bool mat2_cmp(mat2 A, mat2 B);

void mat4_set(mat4 dest, mat4 src);
void mat3_set(mat3 dest, mat3 src);
void mat2_set(mat2 dest, mat2 src);

void mat4_print(mat4 A);
void mat3_print(mat3 A);
void mat2_print(mat2 A);

/* Vector functions. */

void vec4_add(vec4 res, vec4 a, vec4 b);
void vec3_add(vec3 res, vec3 a, vec3 b);
void vec2_add(vec2 res, vec2 a, vec2 b);

void vec4_sub(vec4 res, vec4 a, vec4 b);
void vec3_sub(vec3 res, vec3 a, vec3 b);
void vec2_sub(vec2 res, vec2 a, vec2 b);

void vec4_mul(vec4 res, vec4 a, vec4 b);
void vec3_mul(vec3 res, vec3 a, vec3 b);
void vec2_mul(vec2 res, vec2 a, vec2 b);

void vec4_div(vec4 res, vec4 a, vec4 b);
void vec3_div(vec3 res, vec3 a, vec3 b);
void vec2_div(vec2 res, vec2 a, vec2 b);

bool vec4_cmp(vec4 a, vec4 b);
bool vec3_cmp(vec3 a, vec3 b);
bool vec2_cmp(vec2 a, vec2 b);

/* Mixed functions. */

/* Matrix and vector functions. */

void mve4_mul(vec4 res, mat4 A, vec4 v);
void mve3_mul(vec3 res, mat3 A, vec3 v);
void mve2_mul(vec2 res, mat2 A, vec2 v);

void mve4_div(vec4 res, mat4 A, vec4 v);
void mve3_div(vec3 res, mat3 A, vec3 v);
void mve2_div(vec2 res, mat2 A, vec2 v);

void mve4_add(mat4 res, mat4 A, vec4 v);
void mve3_add(mat3 res, mat3 A, vec3 v);
void mve2_add(mat2 res, mat2 A, vec2 v);

void mve4_sub(mat4 res, mat4 A, vec4 v);
void mve3_sub(mat3 res, mat3 A, vec3 v);
void mve2_sub(mat2 res, mat2 A, vec2 v);

void mve4_translate(mat4 res, mat4 A, vec3 v);
void mve3_translate(mat3 res, mat3 A, vec2 v);
void mve2_translate(mat2 res, mat2 A, float v[1]);

/* Matrix and float functions. */

void mfl4_mul(mat4 res, mat4 A, float f);
void mfl3_mul(mat3 res, mat3 A, float f);
void mfl2_mul(mat2 res, mat2 A, float f);

void mfl4_div(mat4 res, mat4 A, float f);
void mfl3_div(mat3 res, mat3 A, float f);
void mfl2_div(mat2 res, mat2 A, float f);

void mfl4_add(mat4 res, mat4 A, float f);
void mfl3_add(mat3 res, mat3 A, float f);
void mfl2_add(mat2 res, mat2 A, float f);

void mfl4_sub(mat4 res, mat4 A, float f);
void mfl3_sub(mat3 res, mat3 A, float f);
void mfl2_sub(mat2 res, mat2 A, float f);

void mfl4_scale(mat4 res, mat4 A, float f);
void mfl3_scale(mat3 res, mat3 A, float f);
void mfl2_scale(mat2 res, mat2 A, float f);

/* Matrix, float and vector functions. */

void mfv4_rotate(mat4 res, float radians, vec3 rotation);

#endif
