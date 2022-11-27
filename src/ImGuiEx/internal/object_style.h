//
// Created by sunny on 2022-11-27.
//

#pragma once

#include "internal_defines.h"

namespace ImGuiEx::api {
    
    struct Style {
        ImVec4 NodePadding;
        float NodeRounding;
        float NodeBorderWidth;
        float HoveredNodeBorderWidth;
        float SelectedNodeBorderWidth;
        float PinRounding;
        float PinBorderWidth;
        float LinkStrength;
        ImVec2 SourceDirection;
        ImVec2 TargetDirection;
        float ScrollDuration;
        float FlowMarkerDistance;
        float FlowSpeed;
        float FlowDuration;
        ImVec2 PivotAlignment;
        ImVec2 PivotSize;
        ImVec2 PivotScale;
        float PinCorners;
        float PinRadius;
        float PinArrowSize;
        float PinArrowWidth;
        float GroupRounding;
        float GroupBorderWidth;
        float HighlightConnectedLinks;
        float SnapLinkToPinDir; // when true link will start on the line defined by pin direction
        ImVec4 Colors[StyleColor_Count];

        Style() {
            NodePadding = ImVec4(8, 8, 8, 8);
            NodeRounding = 12.0f;
            NodeBorderWidth = 1.5f;
            HoveredNodeBorderWidth = 3.5f;
            SelectedNodeBorderWidth = 3.5f;
            PinRounding = 4.0f;
            PinBorderWidth = 0.0f;
            LinkStrength = 100.0f;
            SourceDirection = ImVec2(1.0f, 0.0f);
            TargetDirection = ImVec2(-1.0f, 0.0f);
            ScrollDuration = 0.35f;
            FlowMarkerDistance = 30.0f;
            FlowSpeed = 150.0f;
            FlowDuration = 2.0f;
            PivotAlignment = ImVec2(0.5f, 0.5f);
            PivotSize = ImVec2(0.0f, 0.0f);
            PivotScale = ImVec2(1, 1);
#if IMGUI_VERSION_NUM > 18101
            PinCorners = ImDrawFlags_RoundCornersAll;
#else
            PinCorners = ImDrawCornerFlags_All;
#endif
            PinRadius = 0.0f;
            PinArrowSize = 0.0f;
            PinArrowWidth = 0.0f;
            GroupRounding = 6.0f;
            GroupBorderWidth = 1.0f;
            HighlightConnectedLinks = 0.0f;
            SnapLinkToPinDir = 0.0f;

            Colors[StyleColor_Bg] = ImColor(60, 60, 70, 200);
            Colors[StyleColor_Grid] = ImColor(120, 120, 120, 40);
            Colors[StyleColor_NodeBg] = ImColor(32, 32, 32, 200);
            Colors[StyleColor_NodeBorder] = ImColor(255, 255, 255, 96);
            Colors[StyleColor_HovNodeBorder] = ImColor(50, 176, 255, 255);
            Colors[StyleColor_SelNodeBorder] = ImColor(255, 176, 50, 255);
            Colors[StyleColor_NodeSelRect] = ImColor(5, 130, 255, 64);
            Colors[StyleColor_NodeSelRectBorder] = ImColor(5, 130, 255, 128);
            Colors[StyleColor_HovLinkBorder] = ImColor(50, 176, 255, 255);
            Colors[StyleColor_SelLinkBorder] = ImColor(255, 176, 50, 255);
            Colors[StyleColor_HighlightLinkBorder] = ImColor(204, 105, 0, 255);
            Colors[StyleColor_LinkSelRect] = ImColor(5, 130, 255, 64);
            Colors[StyleColor_LinkSelRectBorder] = ImColor(5, 130, 255, 128);
            Colors[StyleColor_PinRect] = ImColor(60, 180, 255, 100);
            Colors[StyleColor_PinRectBorder] = ImColor(60, 180, 255, 128);
            Colors[StyleColor_Flow] = ImColor(255, 128, 64, 255);
            Colors[StyleColor_FlowMarker] = ImColor(255, 128, 64, 255);
            Colors[StyleColor_GroupBg] = ImColor(0, 0, 0, 160);
            Colors[StyleColor_GroupBorder] = ImColor(255, 255, 255, 32);
        }
    };

}

