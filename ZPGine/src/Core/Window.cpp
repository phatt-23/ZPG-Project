#include "Window.h"

#include "Debug/Asserter.h"
#include "Platform/Linux/LinuxWindow.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

    scope<Window> Window::Create(WindowProps&& props) 
    {
        ZPG_PROFILE_FUNCTION();
        return MakeScope<LinuxWindow>(props);
    }

}
