#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "falling_pixels.h"
#include "utility_functions.h"

#define UNUSED(x) (void)x

bool keymap[1024] = {0};

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
};

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    UNUSED(scancode);
    UNUSED(mods);
    UNUSED(window);

    if (action == GLFW_PRESS) {
        keymap[key] = true;
    } else {
        keymap[key] = false;
    }
}

void error(const char * message) {
    fprintf(stderr, "ERROR: %s, aborting.\n", message);
    exit(EXIT_FAILURE);
}

int to_int(const char * string) {
    return strtol(string, NULL, 10);
}

void event_processing(bool * keymap, GLFWwindow * window) {
    if (keymap[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
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

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        error("Could not initialize GLEW.");
    }

    const GLchar * vertexShaderSource = \
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "\n"
        "uniform mat4 transform;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = transform * vec4(position, 1.0f);\n"
        //"   gl_Position = vec4(position, 1.0f);\n"
        "}\n";

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        GLchar message[1024];
        const GLchar * error_format = "Vertex shader compilation failed with: %s\n";
        snprintf(message, 1024, error_format, infoLog);
        error(message);
    }

    const GLchar * fragmentShaderSource = \
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        GLchar message[1024];
        const GLchar * error_format = "Fragment shader compilation failed with: %s\n";
        snprintf(message, 1024, error_format, infoLog);
        error(message);
    }

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        GLchar message[1024];
        const GLchar * error_format = "Shader linkage failed with: %s\n";
        snprintf(message, 1024, error_format, infoLog);
        error(message);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glfwSetKeyCallback(window, key_callback);

    mat4 transform = {0};

    mat4_set(transform, m4id);
    mfl4_scale(transform, transform, 0.5f);
    mve4_translate(transform, transform, (vec3){0.5f, 0.5f, 0.0f});

    mat4_print(transform);

    mat4 rotation = {0};
    mfl4_rotate(rotation, 0.25*M_PI, (vec3){0.0f, 0.0f, 1.0f});

    mat4 temp = {0};
    mat4_mul(temp, transform, rotation);

    mat4_set(transform, temp);
    mat4_print(transform);

    GLuint transform_location = glGetUniformLocation(shaderProgram, "transform");

    float x_displacement = 0.0f;

    while(!glfwWindowShouldClose(window)) {

        mat4_set(temp, m4id);

        /* Poll for and process events. */
        glfwPollEvents();
        event_processing(keymap, window);

        /* Render here. */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        /* Set temp matrix to transform matrix. */
        mat4_set(temp, transform);
        /* Translate temp matrix with x_displacement. */
        mve4_translate(temp, temp, (vec3){x_displacement, 0.0f, 0.0f});

        glUniformMatrix4fv(transform_location, 1, MATORD, mat_ptr(temp));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        /* Swap from and back buffers. */
        glfwSwapBuffers(window);

        if (temp[0][3]+x_displacement >= 1.0f) {
            x_displacement = 0.0f;
        }
        x_displacement += 0.01f;
    }

    glfwTerminate();
    dictionary_free(config);
}
