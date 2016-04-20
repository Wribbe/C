#include "utility_functions.h"

/* Matrices. */

size_t mat_index(size_t col, size_t row, size_t size) {
    return col * size + row;
}

void mat_mul(float * res, float * A, float * B, size_t mat_size) {

    size_t res_index, A_index, B_index;

    for(size_t i = 0; i<mat_size; i++) {

        for(size_t j = 0; j<mat_size; j++) {

            res_index = mat_index(i, j, mat_size);
            res[res_index] = 0;

            for(size_t k = 0; k<mat_size; k++) {

                A_index = mat_index(i, k, mat_size);
                B_index = mat_index(k, j, mat_size);

                res[res_index] += A[A_index] * B[B_index];
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

/* Vectors. */
