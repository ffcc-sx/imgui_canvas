/*
 * Author  : SunXin
 * Modify  : 2022-11-27
 * Version : 1.0.0.0
 * Content :
 *   1.First initialize.
 */

#pragma once

#include "internal_defines.h"

namespace ImGuiEx::api {

    class Config {
    public:
        Config()
        : SettingsFile("NodeEditor.json")
        , BeginSaveSession(nullptr)
        , EndSaveSession(nullptr)
        , SaveSettings(nullptr)
        , LoadSettings(nullptr)
        , SaveNodeSettings(nullptr)
        , LoadNodeSettings(nullptr)
        , UserPointer(nullptr)
        , CustomZoomLevels()
        , CanvasSizeMode(CanvasSizeMode::FitVerticalView)
        , DragButtonIndex(0)
        , SelectButtonIndex(0)
        , NavigateButtonIndex(1)
        , ContextMenuButtonIndex(1) { }

        const char *SettingsFile;
        ConfigSession BeginSaveSession;
        ConfigSession EndSaveSession;
        ConfigSaveSettings SaveSettings;
        ConfigLoadSettings LoadSettings;
        ConfigSaveNodeSettings SaveNodeSettings;
        ConfigLoadNodeSettings LoadNodeSettings;
        void *UserPointer;
        ImVector<float> CustomZoomLevels;
        CanvasSizeMode CanvasSizeMode;
        int DragButtonIndex;        // Mouse button index drag action will react to (0-left, 1-right, 2-middle)
        int SelectButtonIndex;      // Mouse button index select action will react to (0-left, 1-right, 2-middle)
        int NavigateButtonIndex;    // Mouse button index navigate action will react to (0-left, 1-right, 2-middle)
        int ContextMenuButtonIndex; // Mouse button index context menu action will react to (0-left, 1-right, 2-middle)
    };

}
