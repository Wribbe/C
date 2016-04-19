#include "utility_functions.h"

void mat4_mul(mat4 res, mat4 A, mat4 B) {
    size_t matrix_dim = 4;
    for(size_t i = 0; i<matrix_dim; i++) {
        for(size_t j = 0; j<matrix_dim; j++) {
            res[i][j] = 0;
            for(size_t k = 0; k<matrix_dim; k++) {
                res[i][j] += A[i][k]*B[k][j];
            }
        }
    }
}
