#include "TextureLibrary.h"
#include "Debug/Asserter.h"

namespace ZPG {

TextureLibrary::TextureLibrary() = default;
TextureLibrary::~TextureLibrary() {

}
void TextureLibrary::AddTexture(const std::string& name, const ref<Texture>& texture) {
    ZPG_CORE_ASSERT(!Exists(name), "Texture with this name is already in the library: {}", name);
    m_Textures[name] = texture;
}
bool TextureLibrary::Exists(const std::string& name) const {
    return m_Textures.contains(name);
}
const ref<Texture>& TextureLibrary::GetTexture(const std::string& name) const {
    ZPG_CORE_ASSERT(Exists(name), "Texture with this name isn't in the texture library: {}", name);
    return m_Textures.at(name);
}

const std::unordered_map<std::string, std::shared_ptr<Texture>>& TextureLibrary::GetTextures() const {
    return m_Textures;
}
}
