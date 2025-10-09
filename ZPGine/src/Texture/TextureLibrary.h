#pragma once

#include "Texture.h"

namespace ZPG {

class TextureLibrary {
public:
    TextureLibrary();
    ~TextureLibrary();

    void AddTexture(const std::string& name, const Ref<Texture>& texture);
    bool Exists(const std::string& name);
    const Ref<Texture>& GetTexture(const std::string& name);
private:
    std::unordered_map<std::string, Ref<Texture>> m_Textures;
};

}
