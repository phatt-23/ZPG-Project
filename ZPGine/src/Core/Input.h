//
// Created by phatt on 9/22/25.
//

#ifndef INPUT_H
#define INPUT_H

namespace ZPG 
{

    class Input 
    {
    public:
        Input();
        virtual ~Input();

        // API provided by singleton
        static void Init();
        static void Shutdown();
        static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
        static bool IsMouseButtonPressed(int buttonCode) { return s_Instance->IsMouseButtonPressedImpl(buttonCode); }
        static v2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        static float GetMouseX() { return GetMousePosition().x; }
        static float GetMouseY() { return GetMousePosition().y; }

        static void ShowCursor() { s_Instance->ShowCursorImpl(); }
        static void HideCursor() { s_Instance->HideCursorImpl(); }
        static void GrabCursor() { s_Instance->GrabCursorImpl(); }
        static void ConfineCursor() { s_Instance->ConfineCursorImpl(); }

        static bool IsCursorShown() { return s_Instance->IsCursorShownImpl(); }
        static bool IsCursorHidden() { return s_Instance->IsCursorHiddenImpl(); }
        static bool IsCursorGrabbed() { return s_Instance->IsCursorGrabbedImpl(); }
        static bool IsCursorConfined() { return s_Instance->IsCursorConfinedImpl(); }

    private:
        // implementation can differ
        virtual bool IsKeyPressedImpl(int keyCode) = 0;
        virtual bool IsMouseButtonPressedImpl(int buttonCode) = 0;
        virtual v2 GetMousePositionImpl() = 0;
        virtual void ShowCursorImpl() = 0;
        virtual void HideCursorImpl() = 0;
        virtual void GrabCursorImpl() = 0;
        virtual void ConfineCursorImpl() = 0;
        virtual bool IsCursorShownImpl() = 0;
        virtual bool IsCursorHiddenImpl() = 0;
        virtual bool IsCursorGrabbedImpl() = 0;
        virtual bool IsCursorConfinedImpl() = 0;

        inline static Input* s_Instance = nullptr;
    };

}

#endif //INPUT_H
