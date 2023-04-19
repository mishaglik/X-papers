#ifndef ENGINE_RENDERENGINE_GVEC_HPP
#define ENGINE_RENDERENGINE_GVEC_HPP
#include <array>
#include <cstddef>

namespace xppr {

template<class T, size_t n>
using vec = std::array<T, n>;

using vec3f = vec<float, 3>;
using vec4f = vec<float, 4>;

constexpr vec4f& operator*=(vec4f& lhs, const vec4f& rhs) {
    for(size_t i = 0; i < 4; ++i) {
        lhs[i] *= rhs[i];
    }
    return lhs;
}

constexpr vec4f&& operator*=(vec4f&& lhs, const vec4f& rhs) {
    for(size_t i = 0; i < 4; ++i) {
        lhs[i] *= rhs[i];
    }
    return std::move(lhs);
}

constexpr vec4f operator*(const vec4f& lhs, const vec4f& rhs) {
    return vec4f(lhs) *= rhs;
}

}

#endif /* ENGINE_RENDERENGINE_GVEC_HPP */
