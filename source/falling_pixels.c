#include <stdlib.h>
#include <stdio.h>

#include "falling_pixels.h"
#include "utility_functions.h"

#define UNUSED(x) (void)x


void error(const char * message) {
    fprintf(stderr, "ERROR: %s, aborting.\n", message);
    exit(EXIT_FAILURE);
}

int to_int(const char * string) {
    return strtol(string, NULL, 10);
}

int main(void) {

    if (!glfwInit()) {
        error("Could not load GLFW library");
    }

    const char * config_path = "config/falling_pixels.txt";
    Dictionary * config = parse_config(config_path);

    int width = to_int(dictionary_get("WIDTH", config));
    int height = to_int(dictionary_get("HEIGHT", config));
    const char * title = dictionary_get("TITLE", config);
    GLFWmonitor * monitor = (GLFWmonitor*)dictionary_get("MONITOR", config);
    GLFWwindow * share = (GLFWwindow*)dictionary_get("SHARE", config);

    GLFWwindow * window = glfwCreateWindow(width, height, title, monitor, share);

    if(!window) {
        glfwTerminate();
        error("Could not open window");
    }

    /* Make the window's context current. */
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)) {

        /* Render here. */

        /* Swap from and back buffers. */
        glfwSwapBuffers(window);

        /* Poll for and process events. */
        glfwPollEvents();

    }

    glfwTerminate();
    dictionary_free(config);
}
