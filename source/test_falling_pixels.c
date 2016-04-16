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

    size_t node_array_size = 10;
    size_t dictionary_size = 1;

    typedef struct Node {
        struct Node * next;
        char * key;
        char * value;
    } Node;

    typedef struct dictionary {
        size_t size;
        Node ** node_array;
    } dictionary;

    dictionary * dict = calloc(sizeof(dictionary), dictionary_size);
    dict->node_array = calloc(sizeof(Node *), node_array_size);

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

    Node ** node_array = dict->node_array;
    Node * new_head = NULL;

    for (size_t i = 0; i<key_value_pairs; i++) {
        new_head = malloc(sizeof(Node));
        new_head->next = node_array[i];
        node_array[i] = new_head;

        new_head->key = malloc(sizeof(char)*(strlen(keys[i])+1));
        memcpy(new_head->key, keys[i], strlen(keys[i]));
        new_head->key[strlen(keys[i])] = '\0';

        new_head->value = malloc(sizeof(char)*(strlen(values[i])+1));
        memcpy(new_head->value, values[i], strlen(values[i]));
        new_head->value[strlen(values[i])] = '\0';

        for(size_t j = 0; j<key_value_pairs; j++) {
            new_head = malloc(sizeof(Node));
            new_head->next = node_array[i];
            node_array[i] = new_head;

            new_head->key = malloc(sizeof(char)*(strlen(keys[j])+1));
            memcpy(new_head->key, keys[j], strlen(keys[j]));
            new_head->key[strlen(keys[j])] = '\0';

            new_head->value = malloc(sizeof(char)*(strlen(values[j])+1));
            memcpy(new_head->value, values[j], strlen(values[j]));
            new_head->value[strlen(values[j])] = '\0';
        }
    }

    int print = 0;

    if (print) {
        Node * current_node = NULL;
        for (size_t i = 0; i<key_value_pairs; i++) {
            current_node = node_array[i];
            printf("key: %s, value: %s.\n", current_node->key, current_node->value);
            current_node = current_node->next;
            while(current_node != NULL) {
                printf("  key: %s, value: %s.\n", current_node->key, current_node->value);
                current_node = current_node->next;
            }
        }
    }

    Node * next_node = NULL;
    Node * current_node = NULL;
    for (size_t i=0; i<key_value_pairs; i++) {
        current_node = dict->node_array[i];
        while(current_node != NULL) {
            next_node = current_node->next;
            free(current_node->key);
            free(current_node->value);
            free(current_node);
            current_node = next_node;
        }
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
