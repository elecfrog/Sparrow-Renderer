/*
 * Copy from Piccolo
 * TODO There are some bugs but I dont know why!
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Utils/MarCrossF.h"

#include <array>
#include <functional>
#include <vector>

struct WindowCreateInfo {
    int width{960};
    int height{540};
    const char *title{"Sparrow Renderer"};
    bool is_fullscreen{false};
};

class WindowSystem {
public:
    WindowSystem() = default;

    ~WindowSystem();

    void Init(WindowCreateInfo create_info);

    void PollEvents() const;

    void SwapBuffers() const;

    bool ShouldClose() const;

    void SetTitle(const char *title);

    GLFWwindow *GetWindowHandle() const;

    std::array<int, 2> GetWindowSize() const;

    using onKeyFunc = std::function<void(int, int, int, int)>;
    using onMouseButtonFunc = std::function<void(int, int, int)>;
    using onScrollFunc = std::function<void(double, double)>;
    using onCursorPosFunc = std::function<void(double, double)>;
    
    // framebuffer
    using onFramebufferFunc = std::function<void(int, int)>;
    void RegisterOnFramebufferFunc(const onFramebufferFunc &func) { m_onFramebufferFunc.push_back(func); }
    std::vector<onFramebufferFunc> m_onFramebufferFunc;

    
    void RegisterOnMouseScrollFunc(const onScrollFunc &func) { m_onScrollFunc.push_back(func); }

    void RegisterOnMouseButtonFunc(const onMouseButtonFunc &func) { m_onMouseButtonFunc.push_back(func); }

    void RegisterOnCursorPosFunc(const onCursorPosFunc &func) { m_onCursorPosFunc.push_back(func); }

    bool IsMouseButtonDown(int button) const {
        if (button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_LAST) {
            return false;
        }
        return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }

    bool GetFocusMode() const { return m_is_focus_mode; }

    void SetFocusMode(bool mode);

protected:
    // window event callbacks
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

        LogSystem::Debug("keyCallback > Key: {}, ScanCode: {}, Action: {}, Mods: {}", key, scancode, action, mods);

        WindowSystem *app = (WindowSystem *) glfwGetWindowUserPointer(window);
        if (app) {
            app->onKey(key, scancode, action, mods);
        }
    }

    // FrameBuffer
    static void FramebufferCallback(GLFWwindow *window, int width, int height) {

        WindowSystem *app = (WindowSystem *) glfwGetWindowUserPointer(window);
        if (app) {
//            LogSystem::Debug("FrameBuffer Callback");

            int w, h;
            glfwGetFramebufferSize(window, &w, &h);

//            LogSystem::Debug("FrameBuffer Size{}, {}", w, h);

            glViewport(0, 0, w, h);

            app->onFrameBufferChanged(w, h);
        }
    }

    // [ref](https://www.glfw.org/docs/latest/input_guide.html#cursor_pos)
    static void MousePositionCallBack(GLFWwindow *window, double xpos, double ypos) {
        WindowSystem *app = (WindowSystem *) glfwGetWindowUserPointer(window);
        if (app) {
            app->onCursorPos(xpos, ypos);
        }
    }

    static void MouseButtonCallBack(GLFWwindow *window, int button, int action, int mods) {
        WindowSystem *app = (WindowSystem *) glfwGetWindowUserPointer(window);
        if (app) {
            app->onMouseButton(button, action, mods);
        }

    }

    static void WindowSizeCallback(GLFWwindow *window, int width, int height) {

        WindowSystem *app = (WindowSystem *) glfwGetWindowUserPointer(window);
        if (app) {
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);

            app->m_width = width;
            app->m_height = height;

//            LogSystem::Debug("Window Size Callback")

        }
    }

    static void MouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        WindowSystem *app = (WindowSystem *) glfwGetWindowUserPointer(window);

        if (app) {
            app->onScroll(xOffset, yOffset);
        }
    }


    static void ErrorCallback(int error, const char *description) {
        spdlog::critical("Error: {0}", description);
    }

    void onKey(int key, int scancode, int action, int mods) {
        //TODO: Fix Key Events
    }

    void onCursorPos(double xpos, double ypos) {
        for (auto &func: m_onCursorPosFunc)
            func(xpos, ypos);
    }

    void onMouseButton(int button, int action, int mods) {
        for (auto &func: m_onMouseButtonFunc)
            func(button, action, mods);
    }

    void onScroll(double xoffset, double yoffset) {
        for (auto &func: m_onScrollFunc)
            func(xoffset, yoffset);
    }
    
    void onFrameBufferChanged(int xoffset, int yoffset) {
        for (auto &func: m_onFramebufferFunc)
            func(xoffset, yoffset);
    }


private:

    GLFWwindow *m_window{nullptr};
    int m_width{0};
    int m_height{0};

    bool m_is_focus_mode{false};

    std::vector<onKeyFunc> m_onKeyFunc;
    std::vector<onCursorPosFunc> m_onCursorPosFunc;
    std::vector<onMouseButtonFunc> m_onMouseButtonFunc;
    std::vector<onScrollFunc> m_onScrollFunc;
};
