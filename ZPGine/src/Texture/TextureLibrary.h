#pragma once

#include "Texture.h"

namespace ZPG {

class TextureLibrary {
public:
    TextureLibrary();
    ~TextureLibrary();

    void AddTexture(const std::string& name, const ref<Texture>& texture);
    bool Exists(const std::string& name) const;
    const ref<Texture>& GetTexture(const std::string& name);
private:
    std::unordered_map<std::string, ref<Texture>> m_Textures;
};

}
