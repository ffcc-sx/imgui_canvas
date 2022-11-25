/*
 * Author  : SunXin
 * Modify  : 2022-11-25
 * Version : 1.0.0.0
 * Content :
 *   1.First initialize.
 */

#pragma once

#include <memory>
#include <functional>

class GLFWwindow;
class ImVec4;

class Application {
    //==================== Type defines     ====================//
    enum class State { Idle, Running, Exited};

public:
	static int                  exec(const std::function<void()> &embed = [](){});
    static inline ImVec4       *color_clear();
    static inline Application  *instance();

private:
    Application()   = default;
    ~Application()  = default;

    // Copied from ImGui(Ver 1.89.1) sample.
    static constexpr inline int _window_style();
    static void                 _onRender(const std::function<void()> &embed);

    //==================== Auxiliary member ====================//
    static Application     *_instance;
    static GLFWwindow      *_window;
    static ImVec4          *_color_clear;
    static State            _state;

};