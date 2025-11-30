#pragma once


namespace ZPG
{
    class Mesh;

    class DrawCommand
    {
    public:
        i32 entityID;  // these are named like this unfortunately (name collision with class names)
        m4 transform;
        Mesh* mesh;
    };


}
