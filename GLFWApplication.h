//
// Created by shivaaz on 5/12/23.
//

#ifndef FFMPEG_GLFWAPPLICATION_H
#define FFMPEG_GLFWAPPLICATION_H

#include "Application.hpp"

class GLFWwindow;
class GLFWApplication : public  KSApplication{
    
public:
    
    ~GLFWApplication() override;

    void run() override;

protected:
    void onWindowInit() override;

    void onDestroy() override;

    GLFWwindow* window = nullptr;


};


#endif //FFMPEG_GLFWAPPLICATION_H
