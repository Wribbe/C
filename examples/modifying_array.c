#include <stdlib.h>

typedef float vec3[3];

void set_vector(vec3 dest) {

    float a = 1.0f;
    float b = 1.0f;


    dest[0] = a+b;
    dest[1] = a+b;
    dest[2] = a+b;
}

int main(void) {
    vec3 src = {1,2,3};
    vec3 dest = {0};
    set_vector(dest);

    float value = 0;

    value = dest[0];

    if (value > 1.0f) {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
