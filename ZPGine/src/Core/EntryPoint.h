//
// Created by phatt on 9/22/25.
//

#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <iostream>
#include "Application.h"

extern ZPG::Application* ZPG::CreateApplication();

int main(void) {
    std::cout << "Running..." << std::endl;

    auto* app = ZPG::CreateApplication();
	app->Run();
    delete app;

    return EXIT_SUCCESS;
}

#endif //ENTRYPOINT_H
