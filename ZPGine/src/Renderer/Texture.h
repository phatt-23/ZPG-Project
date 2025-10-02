//
// Created by phatt on 9/22/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Core.h"

namespace ZPG {

class Texture {
public:
    virtual ~Texture();

    virtual const std::string& GetName() const = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;

    virtual void SetData(const void *data, u32 size) = 0;

    static Ref<Texture> Create(const std::string& filepath);
    static Ref<Texture> Create(const std::string& name, const std::string& filepath);
};

}

#endif //TEXTURE_H
