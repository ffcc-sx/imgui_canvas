/*
 * Author  : SunXin
 * Modify  : 2022-11-25
 * Version : 1.0.0.0
 * Content :
 *   1.First initialize.
 */

#include "Application.h"

// Uncomment for ImGui DEPRECATION test.
// #define GL_SILENCE_DEPRECATION

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuiEx/imgui_node_editor.h"
#include "Format.h"

#include <iostream>

using namespace std;

//==================== Initialize static members.   ====================//
Application         *Application::_instance     {nullptr};
GLFWwindow          *Application::_window       {nullptr};
ImVec4              *Application::_color_clear  {nullptr};
Application::State   Application::_state        {Application::State::Idle};

int Application::exec(const std::function<void()> &embed) {
    // Exit code for exec().
    int code_exit = 0;
    // Check state.
    if(_state == State::Running) {
        cout << "Application already running." << endl;
        return -1;
    } else {
        _state = State::Running;
    }

    //==================== Copied from ImGui(Ver 1.89.1) sample. ====================//
    // Decide GL+GLSL versions.
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    //--------------------------------------------------------------------------------//

    glfwSetErrorCallback([](int error, const char* description){
        cout << util::Format("Glfw Error {0}: {1}", error, description) << endl;
    });
    if (!glfwInit()) return -1;

    // Create window with graphics context
    const char *title = "Dear ImGui GLFW+OpenGL3 example";
    _window = glfwCreateWindow(1280, 720, title, nullptr, nullptr);
    if (_window == nullptr) return -1;
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // TODO: Try to modify this style when running.
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // TODO: The ImGuiEx using config to save layout, try make it optional.
    ax::NodeEditor::Config config;
    config.SettingsFile = "Simple.json";
    auto m_Context = ax::NodeEditor::CreateEditor(&config);

    // Main loop
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Invoke content render.
        _onRender(embed);
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(color_clear()->x * color_clear()->w, color_clear()->y * color_clear()->w, color_clear()->z * color_clear()->w, color_clear()->w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(_window);
    }

    ax::NodeEditor::DestroyEditor(m_Context);

    //==================== Exit application::exec() loop. ====================//
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(_window);
    glfwTerminate();
    _state = State::Exited;
    return code_exit;
}

ImVec4 *Application::color_clear() {
    if(!_color_clear) _color_clear = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    return _color_clear;
}

Application *Application::instance() {
    if(!_instance) _instance = new Application;
    return _instance;
}

constexpr int Application::_window_style() {
    return  ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void Application::_onRender(const std::function<void()> &embed) {
    // Render fixed full window panel of background.
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    const auto windowBorderSize = ImGui::GetStyle().WindowBorderSize;
    ImGui::Begin("window_background", nullptr, _window_style());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, windowBorderSize);
    // Invoke embed render function.
    embed();
    // Finished render fixed full window panel of background.
    ImGui::PopStyleVar();
    ImGui::End();
}

// ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
// ImGui::Separator();
// ax::NodeEditor::SetCurrentEditor(m_Context);
// ax::NodeEditor::Begin("My Editor", ImVec2(0.0, 0.0f));
// int uniqueId = 1;
// // Start drawing nodes.
// ax::NodeEditor::BeginNode(uniqueId++);
// ImGui::Text("Node A");
// ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
// ImGui::Text("-> In");
// ax::NodeEditor::EndPin();
// ImGui::SameLine();
// ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
// ImGui::Text("Out ->");
// ax::NodeEditor::EndPin();
// ax::NodeEditor::EndNode();
// ax::NodeEditor::End();
// ax::NodeEditor::SetCurrentEditor(nullptr);