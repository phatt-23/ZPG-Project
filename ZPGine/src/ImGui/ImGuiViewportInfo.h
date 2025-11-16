//
// Created by phatt on 11/16/25.
//
#pragma once

namespace ZPG
{
    struct ImGuiViewportInfo
    {
        // called inside an imgui scope
        void Invalidate();

        v2      AbsPosition;    // absolute window position (monitor space)
        v2      AbsMinBounds;   // absolute min bound (the position of the top left corner)
        v2      AbsMaxBounds;   // absolute max bound (the position of the bottom right corner)
        v2      Size;           // the size of the widget
        v2      Offset;         // where the panel starts (offset from the top left corner)
        v2      PanelSize;      // the available region (without the top tab), note: size - offset
        bool    IsFocused;      // the widget is focused
        bool    IsHovered;      // the mouse cursor is hovering over the widget
        v2      MousePosition;  // the last mouse position, (0, 0) is in the bottom-left corner

        // helpers
        bool IsActive() const { return IsFocused || IsHovered; }
    };
}
