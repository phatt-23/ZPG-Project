//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include "FirstScene.h"

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        m_SceneManager.AddScene("First", ZPG::CreateRef<FirstScene>());
    }
private:
};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // main function gets put here
