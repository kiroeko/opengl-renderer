#include "GLFW/glfw3.h"
#include <iostream>

int main()
{
    if (!glfwInit()) {
        std::cout << "init failed" << std::endl;
        return 0;
    }

    std::cout << "init successed" << std::endl;
    glfwTerminate();
    return 0;
}