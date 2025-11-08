//
// Created by phatt on 9/22/25.
//

#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Application.h"
#include "Debug/Logger.h"
#include "Profiling/Instrumentor.h"

extern ZPG::Application* ZPG::CreateApplication();

int main(void) {
    ZPG::Logger::Init();
    ZPG_CORE_INFO("Logger initialised.");

    ZPG_PROFILE_BEGIN_SESSION("Application Creation", "zpg-application-creation.json");
    ZPG_CORE_INFO("Creating application...");
    ZPG::Application* app = ZPG::CreateApplication();
    ZPG_PROFILE_END_SESSION();

    ZPG_PROFILE_BEGIN_SESSION("Application Run", "zpg-application-run.json");
    ZPG_CORE_INFO("Running...");
	app->Run();
    ZPG_PROFILE_END_SESSION();

    ZPG_PROFILE_BEGIN_SESSION("Application Deletion", "zpg-application-deletion.json");
    ZPG_CORE_INFO("Deleting app...");
    delete app;
    ZPG_PROFILE_END_SESSION();

    return EXIT_SUCCESS;
}

#endif //ENTRYPOINT_H
