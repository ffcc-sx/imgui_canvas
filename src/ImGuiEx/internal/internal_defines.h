//
// Created by sunny on 2022-11-27.
//

#pragma once

namespace ImGuiEx::api {
    struct NodeId;
    struct LinkId;
    struct PinId;

    enum class PinKind {
        Input, Output
    };

    enum class FlowDirection {
        Forward, Backward
    };

    enum class CanvasSizeMode {
        FitVerticalView,        // Previous view will be scaled to fit new view on Y axis
        FitHorizontalView,      // Previous view will be scaled to fit new view on X axis
        CenterOnly,             // Previous view will be centered on new view
    };

    enum class SaveReasonFlags : uint32_t {
        None = 0x00000000,
        Navigation = 0x00000001,
        Position = 0x00000002,
        Size = 0x00000004,
        Selection = 0x00000008,
        AddNode = 0x00000010,
        RemoveNode = 0x00000020,
        User = 0x00000040
    };
    inline SaveReasonFlags operator|(SaveReasonFlags lhs, SaveReasonFlags rhs) {
        return static_cast<SaveReasonFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }
    inline SaveReasonFlags operator&(SaveReasonFlags lhs, SaveReasonFlags rhs) {
        return static_cast<SaveReasonFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }

    using ConfigSaveSettings = bool (*)(const char *data, size_t size, SaveReasonFlags reason, void *userPointer);
    using ConfigLoadSettings = size_t (*)(char *data, void *userPointer);
    using ConfigSaveNodeSettings = bool (*)(NodeId nodeId, const char *data, size_t size, SaveReasonFlags reason, void *userPointer);
    using ConfigLoadNodeSettings = size_t (*)(NodeId nodeId, char *data, void *userPointer);
    using ConfigSession = void (*)(void *userPointer);
}

