//
// Created by shivaaz on 5/12/23.
//

#include "GLFWApplication.h"
#include "glloader/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "cassert"
#include "time.h"
#include "unistd.h"
#include "iostream"
#include "Logger.h"
#include "libffmpeg/FFUtils/FFMpegLogUtils.h"
#include "MediaMain.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout<<"keycallback"<<std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLFWApplication::~GLFWApplication() {

}

void GLFWApplication::run() {

    MediaMain::run();
    FFMpegLogUtils::printBuildConfig();
    FFMpegLogUtils::init();

    KSApplication::run();
    onWindowInit();
    while(!glfwWindowShouldClose(window))
    {
        sleep(1);
        glClearColor(1.0,0.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //onDestroy
    glfwDestroyWindow(window);
    glfwTerminate();
}

//https://www.glfw.org/docs/3.3/quick.html
void GLFWApplication::onWindowInit()
{
    glfwSetErrorCallback(error_callback);
    KSApplication::onWindowInit();
    if(!glfwInit())
    {
        assert(false);
    }

   // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
   // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "GLFW Application", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        glfwTerminate();//ondestrou
        assert(false);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    //load opengl
    gladLoadGL();//other variants.
    glfwSwapInterval(1);


}

void GLFWApplication::onDestroy() {
    KSApplication::onDestroy();
    glfwDestroyWindow(window);//onWindowDestroyed
    glfwTerminate();
}
