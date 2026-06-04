#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

static void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    (void)window;
    glViewport(0, 0, width, height);
}

GLFWwindow* window_create(int width, int height, const char* title){ 
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(/*640, 850*/width, height, title, NULL, NULL);

    if(!window){
        printf("Failed to create window\n");
        glfwTerminate();
        return NULL;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("GLEW error: %s\n", glewGetErrorString(err));
        return 0;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return window;
}