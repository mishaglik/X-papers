#ifndef ENGINE_RENDERENGINE_GVEC_HPP
#define ENGINE_RENDERENGINE_GVEC_HPP
#include <array>
#include <cstddef>

namespace xppr {

template<class T, size_t n>
using vec = std::array<T, n>;

using vec3f = vec<float, 3>;
using vec4f = vec<float, 4>;

}

#endif /* ENGINE_RENDERENGINE_GVEC_HPP */
