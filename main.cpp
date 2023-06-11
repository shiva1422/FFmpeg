#include <iostream>

#include "GLFWApplication.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    std::shared_ptr<GLFWApplication> app = std::make_shared<GLFWApplication>();
    app->run();
    return 0;
}
