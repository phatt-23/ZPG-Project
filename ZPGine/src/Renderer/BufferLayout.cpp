#include "BufferLayout.h"
#include "Core/Core.h"

namespace ZPG {

BufferLayout::BufferLayout(std::initializer_list<BufferElement>&& elements) : m_Elements(elements) {
    u32 offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        auto& element = m_Elements[i];
        element.Offset = offset;
        offset += element.Size;
    }
    m_Stride = offset;
}
BufferLayout::~BufferLayout() {
    
}
u32 BufferLayout::GetStride() const { 
    return m_Stride; 
}
const std::vector<BufferElement> BufferLayout::GetElements() const { 
    return m_Elements; 
}
std::vector<BufferElement>::const_iterator BufferLayout::begin() const { 
    return m_Elements.begin(); 
}
std::vector<BufferElement>::const_iterator BufferLayout::end() const { 
    return m_Elements.end(); 
}

}
