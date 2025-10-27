//
// Created by phatt on 10/26/25.
//

#ifndef WORKSPACE_VERTEX_ARRAY_LIBRARY_H
#define WORKSPACE_VERTEX_ARRAY_LIBRARY_H


namespace ZPG {

class VertexArray;

class VertexArrayLibrary {
public:
    void AddVAO(const std::string& name, const ref<VertexArray>&);
    const ref<VertexArray>& GetVAO(const std::string& name) const;
    const std::unordered_map<std::string, ref<VertexArray>>& GetVAOs() const;
    bool Exists(const std::string& name) const;
private:
    std::unordered_map<std::string, ref<VertexArray>> m_VAOs;
};

}

#endif