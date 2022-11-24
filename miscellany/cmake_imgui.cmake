#
# Author  : SunXin
# Modify  : 2022-11-22
# Version : 1.0.0.0
# Content :
#   1.Initialize commit.
#

set(IMGUI_NAME imgui)
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/libs/imgui)
set(IMGUI_SOURCES
        # Sources
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        # Headers
        ${IMGUI_DIR}/imconfig.h
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/imgui_internal.h
        ${IMGUI_DIR}/imstb_rectpack.h
        ${IMGUI_DIR}/imstb_textedit.h
        ${IMGUI_DIR}/imstb_truetype.h
        )

# Add 'imgui' static archive.
add_library(${IMGUI_NAME} STATIC ${IMGUI_SOURCES})
target_include_directories(${IMGUI_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/imgui)
target_compile_definitions(${IMGUI_NAME} PUBLIC
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS)