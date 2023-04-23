#ifndef X11_PAIR_INT_HPP
#define X11_PAIR_INT_HPP

namespace xppr {

template <typename T>
struct pair_impl {
   public:
    explicit pair_impl() = default;
    constexpr pair_impl(T a, T b) : x(a), y(b) {}

    constexpr pair_impl(const pair_impl<T>& other) : x(other.x), y(other.y) {}

    constexpr pair_impl(pair_impl<T>&& other) : x(other.x), y(other.y) {
        other.x = other.y = T();
    }

   public:
    T x;
    T y;
};

using pair_t = pair_impl<int>;

}  // namespace xppr

#endif  // !X11_PAIR_INT_HPP