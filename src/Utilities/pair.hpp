#ifndef X11_PAIR_INT_HPP
#define X11_PAIR_INT_HPP

#include <concepts>
namespace xppr {

template<typename T>
concept Copyable = std::copyable<T>;
template <Copyable T>
struct pair_impl {
   public:
    explicit pair_impl() = default;
    constexpr pair_impl(T a, T b) : x(a), y(b) {}

    constexpr pair_impl(const pair_impl<T>& other) : x(other.x), y(other.y) {}

    constexpr pair_impl(pair_impl<T>&& other) : x(other.x), y(other.y) {
        other.x = other.y = T();
    }

    pair_impl& operator=(const pair_impl& other) = default;
    pair_impl& operator=(pair_impl& other) = default;

   public:
    T x;
    T y;
};

using pair_t = pair_impl<int>;
using upair_t = pair_impl<unsigned long long>;

}  // namespace xppr

#endif  // !X11_PAIR_INT_HPP
