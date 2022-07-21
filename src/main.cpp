#define GLFW_INCLUDE_NONE

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <GLFW/glfw3.h>
#include <glbinding-aux/meta.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <qrencode.h>

using namespace gl;

// constants
const uint32_t c_modules = 21;
const uint32_t c_border = 1;
const uint32_t c_size = c_modules + 2 * c_border;
const uint32_t c_scale = 20;
const uint32_t c_res = c_size * c_scale;

const uint32_t c_bytes = 3;
const uint32_t c_rowSize = c_size * c_bytes;
const uint32_t c_rowSizePadded = ((c_rowSize / 4) + ((c_rowSize % 4) > 0)) * 4;
const uint32_t c_bufSize = c_size * c_rowSizePadded;

const uint8_t c_black = 0x00;
const uint8_t c_white = 0xFF;

// globals
uint32_t g_texture;
uint32_t g_fbo;
uint8_t g_buffer[c_bufSize];

void logError(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam)
{
    std::cout << glbinding::aux::Meta::getString(source) << ": "
              << glbinding::aux::Meta::getString(type) << "\n>>> " << message
              << std::endl;
}

std::string getTime()
{
    const auto now = std::chrono::system_clock::now();
    const auto cNow = std::chrono::system_clock::to_time_t(now);
    auto ss = std::stringstream();
    ss << std::put_time(std::localtime(&cNow), "%FT%T%z");
    auto str = ss.str();
    const auto len = str.length();
    str.insert(len - 2, 1, ':');
    return str;
}

void fillTex()
{
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB8,
        c_size,
        c_size,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        g_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void initShow()
{
    glGenTextures(1, &g_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    for (uint32_t y = 0; y < c_size; ++y)
    {
        for (uint32_t x = 0; x < c_size; ++x)
        {
            const uint32_t addr = c_rowSizePadded * y + c_bytes * x;
            g_buffer[addr + 0] = c_white;
            g_buffer[addr + 1] = c_white;
            g_buffer[addr + 2] = c_white;
        }
    }
    fillTex();

    glGenFramebuffers(1, &g_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, g_texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "fbo not initialized" << std::endl;
        exit(-1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void show(QRcode* qr)
{
    for (uint32_t y = 0; y < c_modules; ++y)
    {
        for (uint32_t x = 0; x < c_modules; ++x)
        {
            uint32_t writeY = c_size - 1 - (y + c_border);
            uint32_t writeX = x + c_border;
            const uint32_t addr = c_rowSizePadded * writeY + c_bytes * writeX;
            uint8_t data = qr->data[y * qr->width + x];
            data &= 0b1;
            data = data > 0 ? c_black : c_white;
            g_buffer[addr + 0] = data;
            g_buffer[addr + 1] = data;
            g_buffer[addr + 2] = data;
        }
    }
    fillTex();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, g_fbo);
    glBlitFramebuffer(
        0,
        0,
        c_size,
        c_size,
        0,
        0,
        c_res,
        c_res,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(c_res, c_res, "clock", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glbinding::initialize(glfwGetProcAddress);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(logError, 0);

    std::string last = "";
    initShow();

    while (!glfwWindowShouldClose(window))
    {
        const auto cur = getTime();
        if (cur.compare(last) != 0)
        {
            last = cur;
            std::cout << last << std::endl;
            if (last.length() > 25)
                std::cout << "time too long:" << last.length() << std::endl;
            auto qr = QRcode_encodeString(
                last.c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 1);
            if (qr->version > 1)
                std::cout << "qr code too big:" << qr->version << std::endl;

            show(qr);
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
