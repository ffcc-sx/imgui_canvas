/*
 * Author  : SunXin
 * Modify  : 2022-11-25
 * Version : 1.0.0.0
 * Content :
 *   1.First initialize.
 */

#pragma once

#include <memory>

class GLFWwindow;

class Application {

public:
	static int      exec();

private:
    Application()   = default;
    ~Application()  = default;

    static void     _onRender();

    //==================== Type defines     ====================//
    enum class State { Idle, Running, Exited};

    //==================== Auxiliary member ====================//
    static Application     *_instance;
    static State            _state;
    static GLFWwindow      *_window;
};