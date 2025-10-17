//
// Created by phatt on 9/22/25.
//

#ifndef CORE_H
#define CORE_H

#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

namespace ZPG {

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;
typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef float f32;
typedef double f64;
typedef u_char byte;

typedef glm::vec1 v1;
typedef glm::vec2 v2;
typedef glm::vec3 v3;
typedef glm::vec4 v4;
typedef glm::i32vec1 iv1;
typedef glm::i32vec2 iv2;
typedef glm::i32vec3 iv3;
typedef glm::i32vec4 iv4;
typedef glm::mat3 m3;
typedef glm::mat4 m4;
typedef glm::quat qtr;

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T>
ref<T> MakeRef(T* ptr) {
    return std::shared_ptr<T>(ptr);  // when creating shared_ptr, musn't use make_shared
}

template<typename T, typename ...Args>
ref<T> MakeRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
scope<T> MakeScope(T* ptr) {
    return std::unique_ptr<T>(ptr);  // when creating unique_ptr, musn't use make_unique
}

template<typename T, typename ...Args>
scope<T> MakeScope(Args... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}

#define ZPG_BIT(x) (1 << (x))
#define ZPG_IM_UNUSED(_PARAM) ((void)_PARAM)
#define ZPG_ARRAY_LENGTH(array) (sizeof(array)/sizeof(*array))

// #define ZPG_FORWARD_EVENT_TO_MEMBER_FN_USE_LAMBDAS
#ifdef ZPG_FORWARD_EVENT_TO_MEMBER_FN_USE_LAMBDAS
    #define ZPG_FORWARD_EVENT_TO_MEMBER_FN(fn) ([this](auto&& e) -> bool { return fn(std::forward<decltype(e)>(e)); })
#else
    #define ZPG_FORWARD_EVENT_TO_MEMBER_FN(fn) (std::bind(&(fn), this, std::placeholders::_1))
#endif



#endif //CORE_H
