#include "Window.h"

#include "Debug/Asserter.h"
#include "Platform/Linux/LinuxWindow.h"

namespace ZPG {

scope<Window> Window::Create(const WindowProps& props) {
    return MakeScope<LinuxWindow>(props);
}

}
