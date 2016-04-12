#include <stdlib.h>
#include <stdio.h>

#include "falling_pixels.h"

#define UNUSED(x) (void)x


void error(char * message) {
    fprintf(stderr, "ERROR: %s, aborting.\n", message);
    exit(EXIT_FAILURE);
}

int main(void) {

    if (!glfwInit()) {
        error("Could not load GLFW library");
    }

    int width = 640;
    int height = 480;
    const char * title = "Falling Pixels.";
    GLFWmonitor * monitor = NULL;
    GLFWwindow * share = NULL;

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
}
