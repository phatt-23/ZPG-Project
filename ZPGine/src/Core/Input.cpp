#include "Input.h"
#include "Platform/Linux/LinuxInput.h"
#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG 
{

    Input::Input() 
    {
    }

    Input::~Input() 
    {
    }

    void Input::Init() 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(Input::s_Instance == nullptr, "Input already initialized.");
        Input::s_Instance = new LinuxInput();
    }

    void Input::Shutdown() 
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(s_Instance != nullptr, "Cannot shutdown Input as isn't initialized.");
        free(Input::s_Instance);
        s_Instance = nullptr;
    }

}

