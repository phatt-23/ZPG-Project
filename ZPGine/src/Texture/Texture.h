//
// Created by phatt on 9/22/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

namespace ZPG {

class Texture {
public:
    virtual ~Texture() {}

    virtual const std::string& GetName() const = 0;
    virtual void BindToSlot(u32 slotIndex) = 0;
    virtual void Unbind() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;

    virtual void SetData(const void *data, u32 size) = 0;

    static ref<Texture> Create(const std::string& filepath);
    static ref<Texture> Create(const std::string& name, const std::string& filepath);
    static ref<Texture> Create(const std::string& name, u32 width, u32 height);
};

}

#endif //TEXTURE_H
