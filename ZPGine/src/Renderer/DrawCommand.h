#pragma once


namespace ZPG
{
    class Mesh;

    class DrawCommand
    {
    public:
        i32 entityID;
        m4 transform;
        Mesh* mesh;
    };


}
