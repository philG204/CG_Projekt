#include "../../include/glad/glad.h";
//#include <glad/glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

static void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    (void)window;
    glViewport(0, 0, width, height);
}

GLFWwindow* window_create(int width, int height, const char* title){ 
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


    if(1 /*if WM is hyprland*/){
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return NULL;
        }
    } else{
        //GLenum err = glewInit();
        //if (err != GLEW_OK) {
        //    printf("GLEW error: %s\n", glewGetErrorString(err));
        //    return -1;
        //}
        //if (err == GLEW_OK) {
        //    printf("GLEW initialization failed!\n");
        //    return -1;
        //}
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return window;
}