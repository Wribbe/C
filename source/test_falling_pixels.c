#include <stdlib.h>
#include <stdio.h>

#include "minunit.h"
#include "falling_pixels.h"

int tests_run = 0;

static char * test_glfw_init() {
    mu_assert("Could not load GLFW.", glfwInit() != 0);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_glfw_init);
    return 0;
}

int main(void) {
    char * result = all_tests();
    if(result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Test run: %d\n", tests_run);
    return result != 0;
}
