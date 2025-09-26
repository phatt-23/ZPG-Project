//
// Created by phatt on 9/22/25.
//

#ifndef INPUT_H
#define INPUT_H

#include "Debug/Logger.h"

namespace ZPG {

class Input {
public:
    Input();
    virtual ~Input();

    // API provided by singleton
    static void Init();
    static void Shutdown();
    static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
    static bool IsMouseButtonPressed(int buttonCode) { return s_Instance->IsMouseButtonPressedImpl(buttonCode); }
    static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
    static float GetMouseX() { return GetMousePosition().first; }
    static float GetMouseY() { return GetMousePosition().second; }
private:
    // implementation can differ
    virtual bool IsKeyPressedImpl(int keyCode) = 0;
    virtual bool IsMouseButtonPressedImpl(int buttonCode) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() = 0;
    inline static Input* s_Instance = nullptr;
};

}

#endif //INPUT_H
