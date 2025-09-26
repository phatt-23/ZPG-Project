//
// Created by phatt on 9/22/25.
//

#ifndef LINUXINPUT_H
#define LINUXINPUT_H

#include "Core/Input.h"
#include "Debug/Logger.h"

namespace ZPG {

class LinuxInput : public Input {
public:
    LinuxInput();
    ~LinuxInput() override;
private:
    virtual bool IsKeyPressedImpl(int keyCode) override;
    virtual bool IsMouseButtonPressedImpl(int buttonCode) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
};

}

#endif //LINUXINPUT_H
