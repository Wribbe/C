#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#define UNUSED(x) (void)x

void error(char* message) {
    fprintf(stderr, "ERROR: %s, aborting.\n", message);
    exit(EXIT_FAILURE);
}

int main(void) {

    GLFWwindow* window;


    if (!glfwInit()) {
        error("Could not load GLFW library");
    }


    UNUSED(window);
}
