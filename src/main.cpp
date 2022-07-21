#include <glbinding/gl/gl.h>

#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    const auto size = 21;
    const auto scale = 20;
    const auto res = size * scale;
    window = glfwCreateWindow(res, res, "clock", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
