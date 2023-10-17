/*
 * Copy from Piccolo
 */

#include "WindowSystem.h"

WindowSystem::~WindowSystem() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

#define SPW_RENDER_API_OPENGL
#define SPW_VSYNC

void WindowSystem::Init(const WindowCreateInfo& create_info) {
    if (!glfwInit()) {
        SPW_ERROR("Failed to Create GLFW Window");
        return;
    }

    m_WindowExt = create_info.windowExt;

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    m_window = glfwCreateWindow(m_WindowExt.width, m_WindowExt.height, create_info.title, nullptr, nullptr);
    if (!m_window) {
        SPW_ERROR("Failed to Create Window Handle");
        glfwTerminate();
        return;
    }


// TODO Latter Vulkan Support
#ifdef SPW_RENDER_API_OPENGL
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        SPW_ERROR("Failed to Load GLAD");
        return;
    }
#endif

#ifdef SPW_VSYNC
    glfwSwapInterval(1);
#endif

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetFramebufferSizeCallback(m_window, FramebufferCallback);
    glfwSetWindowSizeCallback(m_window, WindowSizeCallback);

    // [ref](https://www.glfw.org/docs/latest/input_guide.html#cursor_pos)
    glfwSetCursorPosCallback(m_window, MousePositionCallBack);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallBack);
    glfwSetScrollCallback(m_window, MouseScrollCallback);
    // glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetCursorPos(m_window, m_WindowExt.width / 2, m_WindowExt.height / 2);

    glfwSetErrorCallback(ErrorCallback);

    PollEvents();
}

void WindowSystem::PollEvents() const {
    glfwPollEvents();
}

void WindowSystem::SwapBuffers() const {
    glfwSwapBuffers(m_window);
}

bool WindowSystem::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void WindowSystem::SetTitle(const char *title) { glfwSetWindowTitle(m_window, title); }

GLFWwindow *WindowSystem::GetWindowHandle() const { return m_window; }

WindowExt WindowSystem::GetWindowSize() const { return m_WindowExt; }

void WindowSystem::SetFocusMode(bool mode) {
    m_is_focus_mode = mode;
    glfwSetInputMode(m_window, GLFW_CURSOR, m_is_focus_mode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

