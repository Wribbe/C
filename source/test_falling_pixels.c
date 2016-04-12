#include <stdlib.h>
#include <stdio.h>

#include "minunit.h"
#include "falling_pixels.h"

int tests_run = 0;

static char * test_glfw_init(void) {
    mu_assert("Could not load GLFW.", glfwInit() != 0);
    return 0;
}

static char * test_create_window(void) {

    int width = 640;
    int height = 480;
    const char* title = "Test Window";
    GLFWmonitor * monitor = NULL;
    GLFWwindow * share = NULL;

    GLFWwindow * window = glfwCreateWindow(width, height, title, monitor, share);
    mu_assert("Could not initialize window.", window != NULL);
    return 0;
}

static char * test_compilation_error_without_graphics_flags(void) {

    const char * SOURCE_PATH = "source/dummy_glfw_compilation_test.c";
    const char * CFLAGS = "-Wall -Wextra -pedantic -g";
    const char * GRAPHICS_FLAGS  = "-lGLEW -lglfw3 -lGL -lX11 -lXrandr -lXi -lXxf86vm -lm -ldl -lXinerama -lXcursor -lrt -lpthread";
    const char * OUTPUT_FILE = "test_glfw_executable";

    size_t buffer_size = 256;
    char command_buffer[buffer_size];

    const char * correct_format = "cc %s -o %s %s %s > /dev/null 2>&1";
    //const char * faulty_format = "cc %s %s %s -o %s";

    const char * remove_output_file = "rm -f %s";

    int compilation_retcode = 0;
    snprintf(command_buffer, buffer_size, correct_format, SOURCE_PATH, OUTPUT_FILE, CFLAGS, GRAPHICS_FLAGS);

    compilation_retcode = system(command_buffer);
    mu_assert("Retcode from correct compilation was not 0.", compilation_retcode == 0);

    snprintf(command_buffer, buffer_size, remove_output_file, OUTPUT_FILE);
    system(command_buffer);

    //snprintf(command_buffer, buffer_size, faulty_format, SOURCE_PATH, CFLAGS, GRAPHICS_FLAGS, OUTPUT_FILE);
    snprintf(command_buffer, buffer_size, correct_format, SOURCE_PATH, OUTPUT_FILE, CFLAGS, "");
    compilation_retcode = system(command_buffer);
    mu_assert("Retcode from faulty compilation was 0.", compilation_retcode != 0);

    snprintf(command_buffer, buffer_size, remove_output_file, OUTPUT_FILE);
    system(command_buffer);
    return 0;
}

static char * all_tests(void) {
    mu_run_test(test_glfw_init);
    mu_run_test(test_create_window);
    mu_run_test(test_compilation_error_without_graphics_flags);
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
