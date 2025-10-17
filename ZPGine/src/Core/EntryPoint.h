//
// Created by phatt on 9/22/25.
//

#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Application.h"
#include "Debug/Logger.h"

extern ZPG::Application* ZPG::CreateApplication();

int main(void) {
    ZPG::Logger::Init();
    ZPG_CORE_INFO("Logger initialised.");

    ZPG_CORE_INFO("Creating application...");
    ZPG::Application* app = ZPG::CreateApplication();

    ZPG_CORE_INFO("Running...");
	app->Run();

    ZPG_CORE_INFO("Deleting app...");
    delete app;

    return EXIT_SUCCESS;
}

#endif //ENTRYPOINT_H
