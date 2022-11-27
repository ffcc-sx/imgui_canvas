// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "Application.h"

#include "ImGuiEx/imguiEx.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

int main(int, char**) {
    // TODO: The ImGuiEx using config to save layout, try make it optional.
    ImGuiEx::api::Config config;
    config.SettingsFile = "Simple.json";
    auto m_Context = ImGuiEx::api::CreateEditor(&config);
    auto render = [&](){
        auto &io = ImGui::GetIO();
        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
        ImGui::Separator();

        ImGuiEx::api::SetCurrentEditor(m_Context);
        ImGuiEx::api::Begin("My Editor", ImVec2(0.0, 0.0f));
        int uniqueId = 1;
        // Start drawing nodes.
        ImGuiEx::api::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ImGuiEx::api::BeginPin(uniqueId++, ImGuiEx::api::PinKind::Input);
        ImGui::Text("-> In");
        ImGuiEx::api::EndPin();
        ImGui::SameLine();
        ImGuiEx::api::BeginPin(uniqueId++, ImGuiEx::api::PinKind::Output);
        ImGui::Text("Out ->");
        ImGuiEx::api::EndPin();
        ImGuiEx::api::EndNode();
        ImGuiEx::api::End();
        ImGuiEx::api::SetCurrentEditor(nullptr);
    };

    Application::exec(render);
    ImGuiEx::api::DestroyEditor(m_Context);
}
