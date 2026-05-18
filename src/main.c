
#include <glad/glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "../headers/core/window.h"
#include "../headers/core/input.h"


int main(void){

    GLFWwindow* window;
    window_create(window, 1920, 1080, "CG1");

    while(!glfwWindowShouldClose(window)){
        //get input
        //than draw with input;
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}