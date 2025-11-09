#pragma once 

#include "BufferElement.h"

namespace ZPG {

class BufferLayout {
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement>&& elements);
    ~BufferLayout();

    u32 GetStride() const;
    const std::vector<BufferElement>& GetElements() const;

    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

private:
    std::vector<BufferElement> m_Elements;
    unsigned int m_Stride;
};

}
