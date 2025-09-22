//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"
#include "Core/EntryPoint.h"    // main function gets put here

class ClientApp : public ZPG::Application
{
public:

private:

};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}
