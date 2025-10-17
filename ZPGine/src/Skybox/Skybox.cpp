//
// Created by phatt on 10/17/25.
//

#include "Skybox.h"

namespace ZPG {
/*
void makeCubeMapTexture(Texture& self, const std::string& skyboxTexturesDirectory) {
    // Cube Map texture.
    glGenTextures(1, &self.textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, self.textureID);

    std::vector<std::string> textureFacesPaths = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg",
    };

    stbi_set_flip_vertically_on_load(false);


    for (unsigned int i{}; i < textureFacesPaths.size(); i++) {
        std::stringstream path;
        path << skyboxTexturesDirectory
            << (skyboxTexturesDirectory.ends_with('/') ? "" : "/")
            << textureFacesPaths[i];

        printf("Loading cubemap texture %s\n", path.str().c_str());

        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.str().c_str(), &width, &height, &nrChannels, 0);

        if (data == nullptr) {
            std::cerr << "Cube map texture failed to load: " << path.str() << "\n";
            continue;
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
*/

} // ZPG