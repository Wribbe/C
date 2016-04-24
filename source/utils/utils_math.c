#include "utility_functions.h"

/* Matrices. */

size_t mat_index(size_t col, size_t row, size_t size) {
    return col * size + row;
}

float op_add(float a, float b) {
    return a + b;
}

float op_sub(float a, float b) {
    return a - b;
}

float op_div(float a, float b) {
    return a / b;
}

float op_mul(float a, float b) {
    return a * b;
}

void mat_iter_3(float * res, float * A, float * B, size_t mat_size, float (*op)(float a, float b)) {

    size_t res_index, A_index, B_index;

    for(size_t i = 0; i<mat_size; i++) {

        for(size_t j = 0; j<mat_size; j++) {

            res_index = mat_index(i, j, mat_size);
            res[res_index] = 0;

            for(size_t k = 0; k<mat_size; k++) {

                A_index = mat_index(i, k, mat_size);
                B_index = mat_index(k, j, mat_size);

                res[res_index] += op(A[A_index], B[B_index]);
            }
        }
    }
}

void mat_iter_p(float * A, size_t mat_size) {
    size_t index;
    for(size_t i = 0; i < mat_size; i++) {
        for(size_t j = 0; j < mat_size; j++) {
            index = mat_index(i, j, mat_size);
            printf("[%zu][%zu]: %0.2f ", i, j, A[index]);
        }
        printf("\n");
    }
}

void mat4_print(mat4 A) {
    mat_iter_p(&A[0][0], 4);
}

void mat3_print(mat3 A) {
    mat_iter_p(&A[0][0], 3);
}

void mat2_print(mat2 A) {
    mat_iter_p(&A[0][0], 2);
}

void mat4_mul(mat4 res, mat4 A, mat4 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 4, &op_mul);
}

void mat3_mul(mat3 res, mat3 A, mat3 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 3, &op_mul);
}

void mat2_mul(mat2 res, mat2 A, mat2 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 2, &op_mul);
}

void mat4_div(mat4 res, mat4 A, mat4 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 4, &op_div);
}

void mat3_div(mat3 res, mat3 A, mat3 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 3, &op_div);
}

void mat2_div(mat2 res, mat2 A, mat2 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 2, &op_div);
}

void mat_iter_2(float * ret, float * A, float * B, size_t mat_size, float (*op)(float a, float b)) {
    size_t index;
    for(size_t i = 0; i<mat_size; i++) {
        for(size_t j = 0; j<mat_size; j++) {
            index = mat_index(i, j, mat_size);
            ret[index] =  op(A[index], B[index]);
        }
    }
}

void mat4_add(mat4 res, mat4 A, mat4 B) {
    mat_iter_2(&res[0][0], &A[0][0], &B[0][0], 4, &op_add);
}

void mat3_add(mat3 res, mat3 A, mat3 B) {
    mat_iter_2(&res[0][0], &A[0][0], &B[0][0], 3, &op_add);
}

void mat2_add(mat2 res, mat2 A, mat2 B) {
    mat_iter_2(&res[0][0], &A[0][0], &B[0][0], 2, &op_add);
}

void mat4_sub(mat4 res, mat4 A, mat4 B) {
    mat_iter_2(&res[0][0], &A[0][0], &B[0][0], 4, &op_sub);
}

void mat3_sub(mat3 res, mat3 A, mat3 B) {
    mat_iter_2(&res[0][0], &A[0][0], &B[0][0], 3, &op_sub);
}

void mat2_sub(mat2 res, mat2 A, mat2 B) {
    mat_iter_2(&res[0][0], &A[0][0], &B[0][0], 2, &op_sub);
}

bool mat_cmp(float * A, float * B, size_t mat_size) {
    size_t cmp_index;
    for(size_t i = 0; i<mat_size; i++) {
        for(size_t j = 0; j<mat_size; j++) {
            cmp_index = mat_index(i, j, mat_size);
            if (A[cmp_index] != B[cmp_index]) {
                return false;
            }
        }
    }
    return true;
}

bool mat4_cmp(mat4 A, mat4 B) {
    return mat_cmp(&A[0][0], &B[0][0], 4);
}

bool mat3_cmp(mat3 A, mat3 B) {
    return mat_cmp(&A[0][0], &B[0][0], 3);
}

bool mat2_cmp(mat2 A, mat2 B) {
    return mat_cmp(&A[0][0], &B[0][0], 2);
}

void mat_set(float * dest, float * source, size_t mat_size) {
    size_t index;
    for(size_t i = 0; i<mat_size; i++) {
        for(size_t j = 0; j<mat_size; j++) {
            index = mat_index(i, j, mat_size);
            dest[index] = source[index];
        }
    }
}

void mat4_set(mat4 dest, mat4 source) {
    mat_set(&dest[0][0], &source[0][0], 4);
}

void mat3_set(mat3 dest, mat3 source) {
    mat_set(&dest[0][0], &source[0][0], 3);
}

void mat2_set(mat2 dest, mat2 source) {
    mat_set(&dest[0][0], &source[0][0], 2);
}

void mat4_identity(mat4 res) {
    mat4_set(res, (mat4) {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    });
}

void mat3_identity(mat3 res) {
    mat3_set(res, (mat3) {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
    });
}

void mat2_identity(mat2 res) {
    mat2_set(res, (mat2) {
        {1.0f, 0.0f},
        {0.0f, 1.0f},
    });
}

/* Vectors. */

void vec_iteration(float * dest, float * a, float * b, size_t vec_size, float (*op)(float a, float b)) {
    for(size_t i = 0; i<vec_size; i++) {
        dest[i] = op(a[i], b[i]);
    }
}

void vec4_add(vec4 res, vec4 a, vec4 b) {
    vec_iteration(res, a, b, 4, &op_add);
}

void vec3_add(vec3 res, vec3 a, vec3 b) {
    vec_iteration(res, a, b, 3, &op_add);
}

void vec2_add(vec2 res, vec2 a, vec2 b) {
    vec_iteration(res, a, b, 2, &op_add);
}

void vec4_sub(vec4 res, vec4 a, vec4 b) {
    vec_iteration(res, a, b, 4, &op_sub);
}

void vec3_sub(vec3 res, vec3 a, vec3 b) {
    vec_iteration(res, a, b, 3, &op_sub);
}

void vec2_sub(vec2 res, vec2 a, vec2 b) {
    vec_iteration(res, a, b, 2, &op_sub);
}

void vec4_mul(vec4 res, vec4 a, vec4 b) {
    vec_iteration(res, a, b, 4, &op_mul);
}

void vec3_mul(vec3 res, vec3 a, vec3 b) {
    vec_iteration(res, a, b, 3, &op_mul);
}

void vec2_mul(vec2 res, vec2 a, vec2 b) {
    vec_iteration(res, a, b, 2, &op_mul);
}

void vec4_div(vec4 res, vec4 a, vec4 b) {
    vec_iteration(res, a, b, 4, &op_div);
}

void vec3_div(vec3 res, vec3 a, vec3 b) {
    vec_iteration(res, a, b, 3, &op_div);
}

void vec2_div(vec2 res, vec2 a, vec2 b) {
    vec_iteration(res, a, b, 2, &op_div);
}

bool vec_cmp(float * a, float * b, size_t size) {
    for(size_t i = 0; i<size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool vec4_cmp(vec4 a, vec4 b) {
    return vec_cmp(a, b, 4);
}

bool vec3_cmp(vec3 a, vec3 b) {
    return vec_cmp(a, b, 3);
}

bool vec2_cmp(vec2 a, vec2 b) {
    return vec_cmp(a, b, 2);
}

/* Mixed functions. */

/* Matrix and float functions. */

void mfl_iterate(float * dest, float * source, float scalar, size_t mat_size, float (*op)(float a, float b)) {
    size_t index;
    for(size_t i = 0; i<mat_size; i++) {
        for(size_t j = 0; j<mat_size; j++) {
            index = mat_index(i, j, mat_size);
            dest[index] = op(source[index], scalar);
        }
    }
}

void mfl4_mul(mat4 res, mat4 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 4, &op_mul);
}

void mfl3_mul(mat3 res, mat3 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 3, &op_mul);
}

void mfl2_mul(mat2 res, mat2 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 2, &op_mul);
}

void mfl4_div(mat4 res, mat4 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 4, &op_div);
}

void mfl3_div(mat3 res, mat3 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 3, &op_div);
}

void mfl2_div(mat2 res, mat2 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 2, &op_div);
}

void mfl4_add(mat4 res, mat4 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 4, &op_add);
}

void mfl3_add(mat3 res, mat3 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 3, &op_add);
}

void mfl2_add(mat2 res, mat2 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 2, &op_add);
}

void mfl4_sub(mat4 res, mat4 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 4, &op_sub);
}

void mfl3_sub(mat3 res, mat3 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 3, &op_sub);
}

void mfl2_sub(mat2 res, mat2 A, float f) {
    mfl_iterate(&res[0][0], &A[0][0], f, 2, &op_sub);
}

/* Matrix and vector functions */

void mve_iterate(float * dest, float * M, float * v, size_t size, float (*op)(float a, float b)) {
    size_t index;
    for(size_t i = 0; i<size; i++) {
        dest[i] = 0;
        for(size_t j = 0; j<size; j++) {
            index = mat_index(i, j, size);
            dest[i] += op(M[index], v[j]);
        }
    }
}

void mve4_mul(vec4 res, mat4 A, vec4 v) {
    mve_iterate(res, &A[0][0], v, 4, &op_mul);
}

void mve3_mul(vec3 res, mat3 A, vec3 v) {
    mve_iterate(res, &A[0][0], v, 3, &op_mul);
}

void mve2_mul(vec2 res, mat2 A, vec2 v) {
    mve_iterate(res, &A[0][0], v, 2, &op_mul);
}

void mve4_div(vec4 res, mat4 A, vec4 v) {
    mve_iterate(res, &A[0][0], v, 4, &op_div);
}

void mve3_div(vec3 res, mat3 A, vec3 v) {
    mve_iterate(res, &A[0][0], v, 3, &op_div);
}

void mve2_div(vec2 res, mat2 A, vec2 v) {
    mve_iterate(res, &A[0][0], v, 2, &op_div);
}

void mve_add_iter(float * dest, float * M, float * v, size_t size, float (*op)(float a, float b)) {
    size_t index;
    for(size_t i = 0; i<size; i++) {
        for(size_t j = 0; j<size; j++) {
            index = mat_index(i, j, size);
            dest[index] = op(M[index], v[i]);
        }
    }
}

void mve4_add(mat4 res, mat4 A, vec4 v) {
    mve_add_iter(&res[0][0], &A[0][0], v, 4, &op_add);
}

void mve3_add(mat3 res, mat3 A, vec3 v) {
    mve_add_iter(&res[0][0], &A[0][0], v, 3, &op_add);
}

void mve2_add(mat2 res, mat2 A, vec2 v) {
    mve_add_iter(&res[0][0], &A[0][0], v, 2, &op_add);
}

void mve4_sub(mat4 res, mat4 A, vec4 v) {
    mve_add_iter(&res[0][0], &A[0][0], v, 4, &op_sub);
}

void mve3_sub(mat3 res, mat3 A, vec3 v) {
    mve_add_iter(&res[0][0], &A[0][0], v, 3, &op_sub);
}

void mve2_sub(mat2 res, mat2 A, vec2 v) {
    mve_add_iter(&res[0][0], &A[0][0], v, 2, &op_sub);
}
