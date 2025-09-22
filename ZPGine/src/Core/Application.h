//
// Created by phatt on 9/22/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

namespace ZPG {

class Application
{
public:
    void Run();
private:
    static inline Application* s_Instance = nullptr;
};

// defined by client
Application* CreateApplication();

}

#endif //APPLICATION_H
