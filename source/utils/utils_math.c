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

void mat4_mul(mat4 res, mat4 A, mat4 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 4, &op_mul);
}

void mat3_mul(mat3 res, mat3 A, mat3 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 3, &op_mul);
}

void mat2_mul(mat2 res, mat2 A, mat2 B) {
    mat_iter_3(&res[0][0], &A[0][0], &B[0][0], 2, &op_mul);
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

/* Vectors. */

/* Mixed functions. */

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


