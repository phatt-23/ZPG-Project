#include "Window.h"

#include "Debug/Asserter.h"
#include "Platform/Linux/LinuxWindow.h"

namespace ZPG {

Scope<Window> Window::Create(const WindowProps& props) {
    return CreateScope<LinuxWindow>(props);
}

}
