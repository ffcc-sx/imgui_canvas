cmake_minimum_required(VERSION 3.20.2)
project(imgui_canvas)
set(CMAKE_CXX_STANDARD 17)

# Build 'glfw' module.
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(libs/glfw)

# Build 'imgui' module of static.
include(miscellany/cmake_imgui.cmake)

file(GLOB_RECURSE PROJECT_SOURCES LIST_DIRECTORIES false src/*.cpp)
add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp
        )
target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/libs/imgui
        ${PROJECT_SOURCE_DIR}/libs/imgui/backends
        ${PROJECT_SOURCE_DIR}/libs/glfw/include
        ${PROJECT_SOURCE_DIR}/libs/format
        )

target_link_libraries(${PROJECT_NAME}
        glfw
        imgui
        $<$<STREQUAL:$<PLATFORM_ID>,Windows>:gdi32>
        $<$<STREQUAL:$<PLATFORM_ID>,Windows>:opengl32>
        $<$<STREQUAL:$<PLATFORM_ID>,Windows>:imm32>
        )
add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ARGS bin
)