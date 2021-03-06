#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "falling_pixels.h"
#include "utility_functions.h"

int tests_run = 0;

char * test_glfw_init(void) {
    mu_assert("Could not load GLFW.", glfwInit() != 0);
    return 0;
}

char * test_create_window(void) {

    int width = 640;
    int height = 480;
    const char* title = "Test Window";
    GLFWmonitor * monitor = NULL;
    GLFWwindow * share = NULL;

    GLFWwindow * window = glfwCreateWindow(width, height, title, monitor, share);
    mu_assert("Could not initialize window.", window);
    glfwTerminate();
    return 0;
}

char * test_compilation_error_without_graphics_flags(void) {

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

char * test_dynamic_array_allocation(void) {
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

char * test_utils_dictionary(void) {

    size_t dict_size = 1;

    const char * key = "";
    const char * value = "";
    const char * return_value = "";

    size_t error_message_size = 512;
    char error[error_message_size];

    Dictionary * dict = dictionary_create(dict_size);

    key = "key";
    value = "value";

    const char * error_format = "key: '%s' did not return value: '%s', got '%s' instead.";

    /* Insert and check return for first key. */
    dictionary_put(key, value, dict);
    return_value = dictionary_get(key, dict);
    snprintf(error, error_message_size, error_format, key, value, return_value);
    mu_assert(error, strcmp(value, return_value) == 0);

    value = "other value";

    /* Replace first value with other value. */
    dictionary_put(key, value, dict);
    return_value = dictionary_get(key, dict);
    snprintf(error, error_message_size, error_format, key, value, return_value);
    mu_assert(error, strcmp(value, return_value) == 0);

    key = "other_key";
    value = "other_key_value";

    /* Insert second value and retrieve it. */
    dictionary_put(key, value, dict);
    return_value = dictionary_get(key, dict);
    snprintf(error, error_message_size, error_format, key, value, return_value);
    mu_assert(error, strcmp(value, return_value) == 0);

    key = "key";
    value = "other value";

    /* Check value of the previous element. */
    return_value = dictionary_get(key, dict);
    snprintf(error, error_message_size, error_format, key, value, return_value);
    mu_assert(error, strcmp(value, return_value) == 0);

    /* Insert third key-value pair and check result. */
    key = "third_key";
    key = "third_value";

    dictionary_put(key, value, dict);
    return_value = dictionary_get(key, dict);
    snprintf(error, error_message_size, error_format, key, value, return_value);
    mu_assert(error, strcmp(value, return_value) == 0);

    /* Re check the previous key-value pair. */
    key = "other_key";
    value = "other_key_value";

    return_value = dictionary_get(key, dict);
    snprintf(error, error_message_size, error_format, key, value, return_value);
    mu_assert(error, strcmp(value, return_value) == 0);

    dictionary_free(dict);

    /* Create a string of unique keys and values, populate larger dict and
     * check the returned values. */
    size_t test_size = 200;
    size_t second_dict_size = 20;

    dict = dictionary_create(second_dict_size);

    size_t test_char_size = 4;
    char test_data[test_size][test_char_size];

    for(size_t i = 0; i<test_size; i++) {
        snprintf(test_data[i], test_char_size, "%zu", i);
        dictionary_put(test_data[i], test_data[i], dict);
    }

    for(size_t i = 0; i<test_size; i++) {
        mu_assert("Did not match.", strcmp(test_data[i], dictionary_get(test_data[i], dict)) == 0);
    }

    dictionary_free(dict);

    return 0;
}

char * test_config_parser(void) {

    const char * config_filename = "config/test/minimal_test_config.txt";
    Dictionary * config_dict = parse_config(config_filename);

    size_t error_buffer_size = 256;

    #define value_buffer_size 128

    char error_buffer[error_buffer_size];

    const char control_values[][2][value_buffer_size] = {
        {"FIRST_KEY","FIRST_VALUE"},
        {"SECOND_KEY","SECOND_VALUE"},
        {"THIRD_KEY","THIRD_VALUE"},
        {"FORTH KEY","FORTH   VALUE"},
        {"FIFTH #KEY", "FIFTH###VALUE"},
        {"NULL_KEY", "NULL"},
    };

    size_t num_control_values =  sizeof(control_values)/2/value_buffer_size;

    const char * error_format = "Key '%s' did not return '%s', got '%s' instead.";

    const char * current_key = NULL;
    const char * current_value = NULL;
    const char * current_return = NULL;

    for(size_t i=0; i<num_control_values; i++) {

        current_key = control_values[i][0];
        current_value = control_values[i][1];
        current_return = dictionary_get(current_key, config_dict);

        if(current_return == NULL && strcmp(current_value, "NULL") != 0) {
            current_return = "NULL";
        }

        snprintf(error_buffer, error_buffer_size, error_format, current_key, current_value, current_return);

        if (strcmp(current_value, "NULL") == 0) {
            mu_assert(error_buffer, current_return == NULL);
        } else {
            mu_assert(error_buffer, strcmp(current_value, current_return) == 0);
        }
    }

    /* Test that NULL is returned when a value is replaced with NULL. */
    dictionary_put("FIRST_KEY", NULL, config_dict);
    char * return_string = dictionary_get("FIRST_KEY", config_dict);
    mu_assert("Writing over key with NULL does not return NULL.", return_string == NULL);

    dictionary_free(config_dict);

    #undef value_buffer_size

    return 0;
}

char * test_maths(void) {

    mat4 m4a = {0};
    mat4_set(m4a, m4id);

    mat4 m4b = {0};
    mat4_set(m4b, m4id);

    mat4 m4res = {0};

    mat4_mul(m4res, m4a, m4b);

    mu_assert("m4res != A", mat4_cmp(m4res, m4a));

    mat2 m2a = {
        { 2.0f,  3.0f},
        { 1.0f, -1.0f},
    };

    mat2 m2b = {
        { 2.0f,  3.0f},
        { 1.0f, -1.0f},
    };

    mat2 m2corr = {
        { 7.0f,  3.0f},
        { 1.0f,  4.0f},
    };

    mat2 m2res = {0};

    mat2_mul(m2res, m2a, m2b);
    mu_assert("m2res != m2corr", mat2_cmp(m2res, m2corr));

    mat4_set(m4a, (mat4){
        {4.0f, 5.0f, 6.0f, 1.0f},
        {6.0f, 5.0f, 4.0f, 4.0f},
        {4.0f, 6.0f, 5.0f, 9.0f},
        {6.0f, 5.0f, 4.0f, 4.0f},
    });

    mat4_set(m4b, (mat4){
        {1.0f, 2.0f, 3.0f, 4.0f},
        {3.0f, 2.0f, 1.0f, 2.0f},
        {2.0f, 1.0f, 3.0f, 6.0f},
        {2.0f, 1.0f, 3.0f, 6.0f},
    });

    mat4 m4corr = {
        {33.0f, 25.0f, 38.0f, 68.0f},
        {37.0f, 30.0f, 47.0f, 82.0f},
        {50.0f, 34.0f, 60.0f, 112.0f},
        {37.0f, 30.0f, 47.0f, 82.0f},
    };

    mat4_mul(m4res, m4a, m4b);
    mu_assert("m4res != m4corr", mat4_cmp(m4res, m4corr));

    /* Test addition of m2a and m2b. */
    mat2_set(m2corr ,(mat2){
        { 4.0f,  6.0f},
        { 2.0f, -2.0f},
    });

    mat2_add(m2res, m2a, m2b);
    mu_assert("Addition of m2a + m2b not same as m2corr", mat2_cmp(m2res, m2corr));

    /* Test float multiplication. */

    mat4_set(m4corr, (mat4){ /* m4a * 2 */
        {8.0f, 10.0f, 12.0f, 2.0f},
        {12.0f, 10.0f, 8.0f, 8.0f},
        {8.0f, 12.0f, 10.0f, 18.0f},
        {12.0f, 10.0f, 8.0f, 8.0f},
    });

    mfl4_mul(m4res, m4a, 2.0f);
    mu_assert("m4a * 2 does not equal m4corr.", mat4_cmp(m4res, m4corr));

    /* Test dividing matrix with float. */

    mfl4_div(m4res, m4corr, 2.0f);
    mu_assert("rel4 not equal m4a when dividing by 2.0f", mat4_cmp(m4res, m4a));

    /* Test addition of float to matrix. */

    mat3 m3a = {
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
        {1.0f, 2.0f, 3.0f},
    };

    mat3 m3corr = {
        {3.0f, 4.0f, 5.0f},
        {3.0f, 4.0f, 5.0f},
        {3.0f, 4.0f, 5.0f},
    };

    mat3 m3res = {0};

    mfl3_add(m3res, m3a, 2.0f);
    mu_assert("m3res does not equal m3corr after addition.", mat3_cmp(m3corr, m3res));

    /* Test subtraction of float to matrix. */

    mat3_set(m3corr, (mat3){
        {-2.0f, -1.0f, 0.0f},
        {-2.0f, -1.0f, 0.0f},
        {-2.0f, -1.0f, 0.0f},
    });

    mfl3_sub(m3res, m3a, 3.0f);
    mu_assert("m3res dose not equal m3corr after subtraction.", mat3_cmp(m3corr, m3res));
    mfl3_add(m3res, m3a, -3.0f);
    mu_assert("m3res dose not equal m3corr after negative addition.", mat3_cmp(m3corr, m3res));

    /* Test multiplying a matrix with a vector. */

    vec3 v3a = {1.0f, 2.0f, 3.0f};
    vec3 v3corr = {14.0f, 14.0f, 14.0f};

    vec3 v3res = {0};

    mve3_mul(v3res, m3a, v3a);
    mu_assert("v3a not equal to v3corr after matrix vector multiplication.", vec3_cmp(v3corr, v3res));

    /* Test adding vector to matrix. */

    mve3_add(m3res, m3a, v3a);

    mat3_set(m3corr,(mat3){
        {2.0f, 3.0f, 4.0f},
        {3.0f, 4.0f, 5.0f},
        {4.0f, 5.0f, 6.0f},
    });

    mu_assert("m3res not equal m3coor after adding v3a to m3a.", mat3_cmp(m3res, m3corr));

    /* Test matrix addition. */

    mat4_set(m4a, (mat4){
        {1.0f, 2.0f, 3.0f, 4.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
    });

    mat4_set(m4b, (mat4){
        {1.0f, 2.0f, 3.0f, 4.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
        {1.0f, 2.0f, 3.0f, 4.0f},
    });

    mat4_set(m4corr, (mat4){
        {2.0f, 4.0f, 6.0f, 8.0f},
        {2.0f, 4.0f, 6.0f, 8.0f},
        {2.0f, 4.0f, 6.0f, 8.0f},
        {2.0f, 4.0f, 6.0f, 8.0f},
    });

    mat4_add(m4res, m4a, m4b);

    mu_assert("m4res not equal m4corr after addition of m4a+m4b.", mat4_cmp(m4res, m4corr));

    /* Test matrix scaling. */

    mat4_set(m4a, m4id);
    mfl4_scale(m4a, m4a, 2.0f);
    mat4_set(m4corr, (mat4){
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 2.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    });

    mu_assert("m4a != m4coor after scaling id matrix with 2.0f", mat4_cmp(m4a, m4corr));

    /* Test matrix translation. */

    mat4_set(m4a, m4id);
    mve4_translate(m4a, m4a, (vec3){1.0f, 2.0f, 3.0f});
    mat4_set(m4corr, (mat4){
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 2.0f},
        {0.0f, 0.0f, 1.0f, 3.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    });

    mu_assert("m4a != m4coor after translating id matrix with {1.0f, 2.0f, 3.0f}", mat4_cmp(m4a, m4corr));

    return 0;
}

char * all_tests(void) {
    mu_run_test(test_glfw_init);
    mu_run_test(test_create_window);
    mu_run_test(test_compilation_error_without_graphics_flags);
    mu_run_test(test_utils_dictionary);
    mu_run_test(test_dynamic_array_allocation);
    mu_run_test(test_config_parser);
    mu_run_test(test_maths);
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
