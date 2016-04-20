#include "utility_functions.h"

/* Matrices. */

void mat_mul(float * res, float * A, float * B, size_t mat_size) {
    for(size_t i = 0; i<mat_size; i++) {
        for(size_t j = 0; j<mat_size; j++) {
            res[i * mat_size + j] = 0;
            for(size_t k = 0; k<mat_size; k++) {
                res[i * mat_size + j] += A[i * mat_size + k] * B[k * mat_size + j];
            }
        }
    }
}

void mat4_mul(mat4 res, mat4 A, mat4 B) {
    mat_mul(&res[0][0], &A[0][0], &B[0][0], 4);
}

void mat3_mul(mat3 res, mat3 A, mat3 B) {
    mat_mul(&res[0][0], &A[0][0], &B[0][0], 3);
}

void mat2_mul(mat2 res, mat2 A, mat2 B) {
    mat_mul(&res[0][0], &A[0][0], &B[0][0], 2);
}

bool mat_cmp(float * A, float * B, size_t mat_size) {
    size_t index;
    for(size_t i = 0; i<mat_size; i++) {
        for(size_t j = 0; j<mat_size; j++) {
            index = i * mat_size + j;
            if (A[index] != B[index]) {
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

/* Vectors. */
