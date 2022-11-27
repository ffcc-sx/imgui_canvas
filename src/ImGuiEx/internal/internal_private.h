//
// Created by sunny on 2022-11-27.
//

#pragma once
#include "internal_common.h"
#include "imgui_internal.h"

namespace ImGuiEx::api::internal {

    inline ImRect ImGui_GetItemRect() {
        return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    }

    inline ImVec2 ImGui_GetMouseClickPos(ImGuiMouseButton buttonIndex) {
        if (ImGui::IsMouseDown(buttonIndex))
            return ImGui::GetIO().MouseClickedPos[buttonIndex];
        else
            return ImGui::GetMousePos();
    }
}