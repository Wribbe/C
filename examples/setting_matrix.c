#include <stdlib.h>
#include <stdio.h>

typedef float mat3[3][3];
typedef float vec3[3];

//size_t mat_index(size_t col, size_t row, size_t size) {
//    return col * size + row;
//}
//
//void mat_setf(float * dest, float value, size_t mat_size) {
//    size_t index;
//    for(size_t i = 0; i<mat_size; i++) {
//        for(size_t j = 0; j<mat_size; j++) {
//            index = mat_index(i, j, mat_size);
//            dest[index] = value;
//        }
//    }
//}
//
//void mat3_setf(mat3 dest, float value) {
//    mat_setf(dest[0], value, 3);
//}

void tri_center(mat3 coords, vec3 result) {
    /*
     * [x1][y1][z1] [0*3,_]
     * [x2][y2][z2] [1*3,_]
     * [x3][y3][z3] [2*3,_]
     */

//    float x1 = 0.0f;
//    float x2 = 0.0f;
//    float x3 = 0.0f;
//
//    float y1 = 0.0f;
//    float y2 = 0.0f;
//    float y3 = 0.0f;
//
//    x1 = coords[3][0];
//    x2 = coords[3][0];
//    x3 = coords[3][0];
//
//    y1 = coords[3][1];
//    y2 = coords[3][1];
//    y3 = coords[3][1];
//
//    float sum_x = 0.0f;
//    float sum_y = 0.0f;
//
//    sum_x = x1+x2+x3;
//    sum_y = y1+y2+y3;
//
//    float res_x = sum_x/3.0f;
//    float res_y = sum_y/3.0f;
//    float res_z = 0.0f;

//    result[0] = res_x;
//    result[1] = res_y;
//    result[3] = res_z;
    result[0] = coords[3][0];
    result[1] = coords[3][1];
    result[3] = res_z;
}

int main(void) {

    mat3 new_coords = {0};
    vec3 result = {0};
    //mat3_setf(new_coords, 1.0f);

 //   for (size_t i=0; i<3; i++) {
 //       for (size_t j=0; j<3; j++) {
 //           new_coords[i][j] = 0.0f;
 //       }
 //   }

    vec3 mass_center = {0};
//    for (int i=0; i<3; i++) {
//        mass_center[i] = 0.0f;
//    }

    float curr_x = 0.0f;

    tri_center(new_coords, mass_center);

    curr_x = mass_center[0];

    if (curr_x < 1.0f) {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}
