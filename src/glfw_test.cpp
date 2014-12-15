#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <stdio.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <vector>

#include "world.h"

GLFWwindow* initializeGLFWWindow(int, int, bool);

class Box {
   public:
      double length;   // Length of a box
      double breadth;  // Breadth of a box
      double height;   // Height of a box
};

int main(int argc, char* argv[]) {
    Box fucking_box;
    
    // Make the randomizer random
    srand(time(NULL));

    // Parse command line arguments
    float width;
    float height;
    bool fullscreen;
    
    glfwInit();

    if (argc == 2 && std::string(argv[1]) == "-f"){
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        width  = mode->width;
        height = mode->height;

        mode = NULL;
        delete mode;

        printf("Loading in fullscreen mode with resolution: %d by %d\n", (int)width, (int)height);
        fullscreen = true;
    } else if (argc == 4 && std::string(argv[1]) == "-w") {
        sscanf(argv[2], "%f", &width);
        sscanf(argv[3], "%f", &height);
        printf("Loading in windowed mode with resolution: %d by %d\n", (int)width, (int)height);
        fullscreen = false;
    } else {
        printf("\nCommand line options:\n");
        printf("\t-f\n");
        printf("\t\tRun in fullscreen mode\n");
        printf("\t-w <width> <height>\n");
        printf("\t\tRun in windowed mode with width and height\n\n");
        return 1;
    }

    // Create the window
    GLFWwindow* window = initializeGLFWWindow(width, height, fullscreen);

    // Create the world
    World world(window);

    std::vector<float> all_fps;
    float last_time = glfwGetTime();


    // Framebuffer Shtuff
    GLuint basic_vs = ShaderLoader::loadVertexShader("shaders/basic.vs");
    GLuint basic_fs = ShaderLoader::loadFragmentShader("shaders/basic.fs");
    GLuint basic_shader = ShaderLoader::combineShaderProgram(basic_vs, basic_fs);

    GLfloat planeVerts[] = {-0.5f,  0.5f, 0.0f, 0.0f,
                             0.5f,  0.5f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 1.0f,
                             0.5f, -0.5f, 1.0f, 1.0f,};

    GLuint  planeFaces[] = {2, 1, 0, 1, 2, 3};

    std::vector<GLfloat> vertices(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> elements(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));

    // We need to know how many faces to draw later on.
    GLuint num_faces = elements.size();
    
    // Create our Vertex Array Object (VAO) which will hold our vertex and element data. 
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Store all of the vertex data in a Vertex Buffer Object (VBO)
    glGenBuffers(1, &vbo);   
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Store all of the face data in a Element Buffer Object (EBO)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()* sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

    glUseProgram(basic_shader);
    glBindVertexArray(vao);
    // Bind the VBO to ensure the correct vertex data gets pushed to the shader.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Get the reference to the "position" attribute defined in
    // the vertex shader
    GLint posAttrib = glGetAttribLocation(basic_shader, "position");
    glEnableVertexAttribArray(posAttrib);
    // Load the position attributes from our vertex spec with width 4. The position
    // values start at index 0. Tell it to load 3 values.
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                           4*sizeof(float), 0);

    // Link the texture coordinates to the shader.
    GLint texAttrib = glGetAttribLocation(basic_shader, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    // Load the texture attributes from our vertex spec with width 4. The texture 
    // values start at index 4. Tell it to load 2 values.
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                           4*sizeof(float), (void*)(3*sizeof(float)));

    // Create frame buffer
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Create texture to hold color buffer
    GLuint texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    glUseProgram(basic_shader);
    glUniform1i(glGetUniformLocation(basic_shader, "texFramebuffer"), 0);

    // Display loop
    while(!glfwWindowShouldClose(window)) {
        // Swap display/rendering buffers
        glfwSwapBuffers(window);
        
        // Handle events
        glfwPollEvents();
        // Check to see if escape is pressed. If so, close the window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        // Clear the screen so we can draw again
        // glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // system("clear");
        float frame_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
        all_fps.push_back(1.0 / frame_time);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        world.update();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glEnable(GL_DEPTH_TEST);
        world.update();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST);
        glUseProgram(basic_shader);
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawElements(GL_TRIANGLES, num_faces, GL_UNSIGNED_INT, 0);
    }

    float total = 0;
    float min = all_fps[0];
    float max = all_fps[0];

    for (int i = 0; i < all_fps.size(); ++i){
        total += all_fps[i];
        if (all_fps[i] > max){
            max = all_fps[i];
        }
        if (all_fps[i] < min){
            min = all_fps[i];
        }
    }
    float average = total / all_fps.size();
    printf("Average FPS: %f\n", average);
    printf("Minimum FPS: %f\n", min);
    printf("Maximum FPS: %f\n", max);

    // Shut down GLFW before exiting the program
    glfwTerminate();

    // Nothing went wrong!
    return 0;
}

GLFWwindow* initializeGLFWWindow(int width, int height, bool fullscreen){
    // glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Set up the MSAA level 
    glfwWindowHint(GLFW_SAMPLES, 16);

    const char* windowTitle = "OpenGL";

    GLFWwindow* window;

    if (fullscreen){
        // Fullscreen 
        window = glfwCreateWindow(width, height, windowTitle, glfwGetPrimaryMonitor(),
            nullptr);

    } else {
        // Windowed
        window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr); 
    }

    // Hide the mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create the OpenGL context in the window
    glfwMakeContextCurrent(window);

    // Set up GLEW so that we can use abstracted OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // Print various info about OpenGL
    printf("\n");
    printf("Renderer:       %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("\n");

    // Set up the correct depth rendering 
    glEnable(GL_DEPTH_TEST);
    
    // Describe what constitutes the front face, and enable backface culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    // Alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Enable MSAA
    glEnable(GL_MULTISAMPLE);

    return window;   
}