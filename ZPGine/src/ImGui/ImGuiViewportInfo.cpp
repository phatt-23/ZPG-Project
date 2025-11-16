//
// Created by phatt on 11/16/25.
//

#include "ImGuiViewportInfo.h"


namespace ZPG
{

    namespace Utils
    {
        static v2 to_v2(const ImVec2& vec)
        {
            return { vec.x, vec.y };
        }

    }

    void ImGuiViewportInfo::Invalidate()
    {
        IsFocused = ImGui::IsWindowFocused();
        IsHovered = ImGui::IsWindowHovered();

        AbsPosition = Utils::to_v2(ImGui::GetCursorScreenPos());
        Size = Utils::to_v2(ImGui::GetWindowSize());
        PanelSize = Utils::to_v2(ImGui::GetContentRegionAvail());
        Offset = Utils::to_v2(ImGui::GetCursorPos());

        AbsMinBounds = AbsPosition;
        AbsMaxBounds = AbsMinBounds + Size;

        auto [mouseX, mouseY] = ImGui::GetMousePos();
        mouseX -= AbsMinBounds.x;
        mouseY -= AbsMinBounds.y;
        mouseY = PanelSize.y - mouseY;
        MousePosition = { mouseX, mouseY };
    }
}
