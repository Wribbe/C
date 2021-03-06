#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "falling_pixels.h"
#include "utility_functions.h"

#include "portaudio.h"
#define NUM_SECONDS   (4)
#define SAMPLE_RATE   (44100)

#define UNUSED(x) (void)x

bool keymap[1024] = {0};
double mouse_pos[2] = {0};
bool mousemap[4] = {0};

typedef struct {
    float left_phase;
    float right_phase;
} paTestData;

static paTestData data = {0};

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods) {
    UNUSED(mods);
    UNUSED(window);
    if (action == GLFW_PRESS && !mousemap[button]) {
        mousemap[button] = true;
    } else if (action == GLFW_RELEASE && mousemap[button]) {
        mousemap[button] = false;
    }
}

static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos) {
    UNUSED(window);
    if (xpos > 0.0f) {
        mouse_pos[0] = xpos;
    }
    if (ypos > 0.0f) {
        mouse_pos[1] = ypos;
    }
}

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    (void) statusFlags;
    (void) timeInfo;
    /* Cast data passed through stream to our structure. */
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->left_phase;  /* left */
        *out++ = data->right_phase;  /* right */
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
        data->right_phase += 0.03f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
    }
    return 0;
}

GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
};

typedef struct Event_data{
    float xdiff;
    float ydiff;
    float zdiff;
    float rotation;
} Event_data;

void init_event_data(Event_data * event_data) {
    event_data->xdiff = 0.0f;
    event_data->ydiff = 0.0f;
    event_data->zdiff = 0.0f;
    event_data->rotation = 0.0f;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    UNUSED(scancode);
    UNUSED(mods);
    UNUSED(window);

    if (action == GLFW_PRESS && !keymap[key]) {
        keymap[key] = true;
    } else if (action == GLFW_RELEASE && keymap[key]) {
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

void event_processing(bool * keymap, GLFWwindow * window, Event_data * event_data) {

    float x_speed = 0.03f;
    float y_speed = 0.03f;
    float rotation_speed = 0.1f;

    if (keymap[GLFW_KEY_SPACE]) {
        if (keymap[GLFW_KEY_H]) {
            event_data->rotation -= rotation_speed;
        }
        if (keymap[GLFW_KEY_U]) {
            event_data->rotation += rotation_speed;
        }
    } else {
        if (keymap[GLFW_KEY_ESCAPE]) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (keymap[GLFW_KEY_H]) {
            event_data->xdiff -= x_speed;
        }
        if (keymap[GLFW_KEY_U]) {
            event_data->xdiff += x_speed;
        }
        if (keymap[GLFW_KEY_N]) {
            event_data->ydiff += y_speed;
        }
        if (keymap[GLFW_KEY_Y]) {
            event_data->ydiff -= y_speed;
        }
    }
}

void tri_center(mat3 coords, vec3 result) {
    /*
     * [x1][y1][z1] [0*3,_]
     * [x2][y2][z2] [1*3,_]
     * [x3][y3][z3] [2*3,_]
     */

    float x1 = coords[0][0];
    float x2 = coords[1][0];
    float x3 = coords[2][0];

    float y1 = coords[0][1];
    float y2 = coords[1][1];
    float y3 = coords[2][1];

    float sum_x = x1+x2+x3;
    float sum_y = y1+y2+y3;

    result[0] = sum_x/3.0f;
    result[1] = sum_y/3.0f;
    result[2] = 0.0f;
}

typedef struct Point {
    float x;
    float y;
    float z;
} Point;

typedef struct TriInfo {
    vec3 center;
    mat4 bounding_box;
    Point sorted_x[3];
    Point sorted_y[3];
    float height;
    float width;
} TriInfo;

void sort_coordinates(mat3 coords, Point * x_coords, Point * y_coords) {
    float max_x = -3.0f;
    float max_y = -3.0f;

    int xpos[3] = {0};
    int ypos[3] = {0};

    if(coords[0][0] < coords[1][0]) {
        xpos[0] = 0;
        xpos[1] = 1;
    } else {
        xpos[0] = 1;
        xpos[1] = 0;
    }
    if(coords[0][xpos[1]] < coords[2][0]) {
        xpos[2] = 2;
    } else {
        xpos[2] = xpos[1];
        xpos[1] = 3;
    }

    for (int i=0; i<3; i++) {
        x_coords[i] = (Point){coords[xpos[i]][0], 0.0f, 0.0f};
    }
}

float get_topmost(mat3 coords, char pos) {
    int index = 0;

    if (pos == 'y') {
        index = 1;
    } else if (pos == 'z') {
        index = 2;
    }

    float max = -2.0f;
    for (int i=0; i<2; i++) {
        if (coords[index][i] > max) {
            max = coords[index][i];
        }
    }
    return max;
}

void tri_info(mat3 coords, TriInfo info) {
   tri_center(coords, info.center);
   Point x[3];
   Point y[3];
   sort_coordinates(coords, x, y);
   Point temp;
   for (int i=0; i<3; i++) {
       temp = x[i];
       printf("Sorted x-coord[%d]: %f\n", i, temp.x);
   }
}


void center_rotation(mat4 rotation, float angle, vec3 axis, mat3 new_coords) {

    mat4 trans_origo, trans_back, temp_rot, temp_mul;
    vec3 mass_center = {0};

    mat4_set(trans_origo, m4id);
    mat4_set(trans_back, m4id);
    mat4_set(temp_rot, m4id);
    mat4_set(temp_mul, m4id);

    tri_center(new_coords, mass_center);

    mve4_translate(trans_origo, trans_origo, (vec3){-mass_center[0], -mass_center[1], 0.0f});
    mfv4_rotate(temp_rot, angle, axis);
    mve4_translate(trans_back, trans_back, (vec3){mass_center[0], mass_center[1], 0.0f});

    mat4_mul(temp_mul, trans_back, temp_rot);
    mat4_mul(trans_back, temp_mul, trans_origo);

    mat4_set(rotation, trans_back);
}

static paTestData data;

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

    /* Set up audio. */

    PaStream *stream;
    PaError err;

    data.left_phase = data.right_phase = 0.0;
    err = Pa_Initialize();
    if( err != paNoError ) {
        error("Initialization of PortAudio failed.");
    }
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                2,          /* stereo output */
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                256,        /* frames per buffer */
                                patestCallback,
                                &data );
    if( err != paNoError ) {
        error("PortAudio could not open stream.");
    }

    //err = Pa_StartStream( stream );

    /* Shaders. */

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    mat4 transform = {0};
    mat4 rotation = {0};
    mat4 temp = {0};

    mat4_set(transform, m4id);

    GLuint transform_location = glGetUniformLocation(shaderProgram, "transform");

    Event_data event_data;
    init_event_data(&event_data);
    mat4 multemp = {0};

    float adjusted_x, adjusted_y;

    bool prev_mouse = false;

    int current_index = 0;
    int total_indices = 3;
    int total_coords = total_indices*3;

    mat3 temp_coords = {0};
    mat3 new_coords = {0};

    vec3 mass_center = {0};
    float curr_x,curr_y,curr_z;

    bool draw_clone = false;

    while(!glfwWindowShouldClose(window)) {

        mat4_set(temp, m4id);

        /* Poll for and process events. */
        prev_mouse = mousemap[GLFW_MOUSE_BUTTON_LEFT];
        glfwPollEvents();
        if (prev_mouse && !mousemap[GLFW_MOUSE_BUTTON_LEFT]) {

            adjusted_x = ((mouse_pos[0]/width)*2.0f)-1.0f;
            adjusted_y = ((mouse_pos[1]/height)*-2.0f)+1.0f;

            temp_coords[current_index][0] = adjusted_x;
            temp_coords[current_index][1] = adjusted_y;
            temp_coords[current_index][2] = 0.0f;

            current_index += 1;
            if (current_index >= total_indices) {
                mat3_set(new_coords, temp_coords);
                current_index = 0;
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*total_coords, (void*)&new_coords);
                init_event_data(&event_data);
            }
        }
        event_processing(keymap, window, &event_data);

        /* Render here. */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        /* Set temp matrix to transform matrix. */
        mat4_set(temp, transform);
        /* Translate temp matrix with x_displacement. */
        mve4_translate(temp, temp, (vec3){event_data.xdiff, event_data.ydiff, event_data.zdiff});

        tri_center(new_coords, mass_center);
        curr_x = mass_center[0] + temp[0][3];
        curr_y = mass_center[1] + temp[1][3];
        curr_z = mass_center[2] + temp[2][3];

        UNUSED(curr_z);

        TriInfo info_tri = {0};
        tri_info(new_coords, info_tri);

        if (curr_x > 1.0f) {
            /* Snap back to left part of screen, -1.0f */
            event_data.xdiff -= 2.0f;
        } else if (curr_x < -1.0f) {
            event_data.xdiff += 2.0f;
        }

        if (curr_y > 1.0f) {
            /* Snap back to left part of screen, -1.0f */
            event_data.ydiff -= 2.0f;
        } else if (curr_y < -1.0f) {
            event_data.ydiff += 2.0f;
        }


        //mfv4_rotate(rotation, M_PI*event_data.rotation, (vec3){0.0f, 0.0f, -1.0f
        center_rotation(rotation, M_PI*event_data.rotation, (vec3){0.0f, 0.0f, -1.0f}, new_coords);
        mat4_mul(multemp, temp, rotation);


        glUniformMatrix4fv(transform_location, 1, MATORD, mat_ptr(multemp));


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        if (draw_clone) {
            multemp[0][3] *= -1;
            multemp[1][3] *= -1;
            glUniformMatrix4fv(transform_location, 1, MATORD, mat_ptr(multemp));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        /* Swap from and back buffers. */
        glfwSwapBuffers(window);

        if (keymap[GLFW_KEY_SPACE]) {
            Pa_StartStream(stream);
        } else {
            Pa_StopStream(stream);
        }
    }

    glfwTerminate();
    dictionary_free(config);
}
