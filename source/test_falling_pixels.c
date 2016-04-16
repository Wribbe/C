#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "falling_pixels.h"
#include "utility_functions.h"

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

static char * test_dynamic_array_allocation(void) {
    /* Check if assumptions about dynamic memory allocation and arrays in
     * combination with linked lists holds. */

    size_t node_list_size = 10;
    size_t dictionary_size = 1;

    typedef struct Node {
        struct Node * next;
        char * key;
        char * value;
    } Node;

    typedef struct dictionary {
        size_t size;
        Node * node_array;
    } dictionary;

    dictionary * dict = calloc(sizeof(dictionary), dictionary_size);
    dict->node_array = calloc(sizeof(Node), node_array_size);

    /* Keys should be able to collide --> each node in node_array should be a
     * linked list. Array as top level to provide O(1) lookup for hash-value.
     *
     * [[ ],[ ],[ ],...] <- node_array
     *   |   |
     *  [ ] [ ]
     *       |
     *      [ ]
     *
     */

    size_t key_value_pairs = 4;

    const char * keys[] = {
        "one",
        "two",
        "three",
        "four",
    };

    const char * values[] = {
        "first",
        "second",
        "third",
        "fourth",
    };

    Node * node_array = dict->node_array;

    for (size_t i = 0; i<key_value_pairs; i++) {
        node_array[i].key = keys[i];
        node_array[i].value = values[i];
    }

    for (size_t i = 0; i < key_value_pairs; i++) {
        printf("key: %s, value: %s\n", node_array[i].key, node_array[i].value);
    }

    free(dict->node_array);
    free(dict);

    return 0;
}

static char * test_utils_dictionary(void) {

    size_t dict_size = 100;

    char * key = "";
    char * value = "";
    char * return_value = "";

    size_t error_message_size = 512;
    char error[error_message_size];

    dictionary * dict = dictionary_create(dict_size);

    key = "key";
    value = "value";

    dictionary_put(key, value, dict);
    return_value = dictionary_get(key, dict);

    const char * error_format = "key: '%s' did not return value: '%s', got '%s' instead.";
    snprintf(error, error_message_size, error_format, key, value, return_value);

    //mu_assert(error, 1 == 2);

    dictionary_free(dict);
    free(dict);
    return 0;
}

static char * all_tests(void) {
//    mu_run_test(test_glfw_init);
//    mu_run_test(test_create_window);
//    mu_run_test(test_compilation_error_without_graphics_flags);
//    mu_run_test(test_utils_dictionary);
    mu_run_test(test_dynamic_array_allocation);
    return 0;
}

int main(void) {
    char * result = all_tests();
    if(result != 0) {
        printf("%s\n", result);
        free(result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Test run: %d\n", tests_run);

    return result != 0;
}
