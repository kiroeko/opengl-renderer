#include "GLFW/glfw3.h"
#include <iostream>

int main()
{
    if (!glfwInit()) {
        std::cout << "init failed" << std::endl;
        return;
    }

    std::cout << "init successed" << std::endl;
    glfwTerminate();
}