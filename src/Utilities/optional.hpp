#ifndef UTILITIES_OPTIONAL_HPP
#define UTILITIES_OPTIONAL_HPP

#include <compare>
#include <concepts>
#include <cstdlib>
#include <type_traits>
#include <utility>
#include "defines.hpp"
namespace xppr {

template <typename T, typename Err, Err NoErr = 0, Err ErrHasNoValue = 1>
    requires std::destructible<T> && std::is_scalar_v<Err> &&
             requires { NoErr != ErrHasNoValue; }
class Optional {
   public:
    // Error = Default constructor
    /*implicit*/ constexpr Optional(Err err = ErrHasNoValue) noexcept
        : m_err(err) {
        assert(err != NoErr);
    }
    /*implicit*/ constexpr Optional& operator=(Err err) noexcept {
        assert(err != NoErr);
        reset();
        m_err = (err == NoErr) ? ErrHasNoValue : err;
    }

    // Value copy constructor
    /*implicit*/ constexpr Optional(const T& t) noexcept : m_err(NoErr) {
        new (m_data) T(t);
    }
    constexpr Optional& operator=(const T& t) noexcept {
        reset();
        new (m_data) T(t);
        m_err = NoErr;
    }

    // Value move constructor
    /*implicit*/ constexpr Optional(T&& t) noexcept : m_err(NoErr) {
        new (m_data) T(std::move(t));
    }
    constexpr Optional& operator=(T&& t) noexcept {
        reset();
        new (m_data) T(std::move(t));
        m_err = NoErr;
    }

    // Copy constructor
    constexpr Optional(const Optional& oth) noexcept : m_err(oth.m_err) {
        if (oth) {
            new (m_data) T(*oth);
        }
    }
    constexpr Optional& operator=(const Optional& oth) noexcept {
        reset();
        m_err = oth.m_err;
        if (oth) {
            new (m_data) T(*oth);
        }
    }

    // Move constructor
    constexpr Optional(Optional&& oth) noexcept : m_err(oth.m_err) {
        if (oth) {
            new (m_data) T(std::move(*oth));
            oth.m_err = ErrHasNoValue;
        }
    }

    constexpr Optional& operator=(Optional&& oth) noexcept {
        reset();
        m_err = oth.m_err;
        if (oth) {
            new (m_data) T(*oth);
            oth.m_err = ErrHasNoValue;
        }
    }

    // Emplace constructor
    template <class... Args>
    constexpr Optional(Args&&... args) {
        emplace(std::forward<Args>(args)...);
    }

    constexpr ~Optional() { reset(); }

    template <class... Args>
    constexpr T& emplace(Args&&... args) {
        reset();
        new (m_data) T(std::forward<Args>(args)...);
        m_err = NoErr;
    }

    constexpr void reset() noexcept {
        if (hasValue()) {
            value().T::~T();
        }
    }

    constexpr T& operator*() {
        requireValue();
        return *reinterpret_cast<T*>(m_data);
    }
    constexpr const T& operator*() const {
        requireValue();
        return *reinterpret_cast<const T*>(m_data);
    }

    constexpr T* operator->() {
        requireValue();
        return reinterpret_cast<T*>(m_data);
    }
    constexpr const T* operator->() const {
        requireValue();
        return reinterpret_cast<const T*>(m_data);
    }

    constexpr bool hasValue() const noexcept { return m_err == NoErr; }
    constexpr operator bool() const noexcept { return hasValue(); }

    constexpr T& value() { return **this; }
    constexpr const T& value() const { return **this; }

    constexpr Err error() const { return m_err; }

    constexpr std::strong_ordering operator<=>(Err err) const noexcept {
        return m_err <=> err;
    }

   private:
    /**
     * @brief Needed to be destructible if T has no trivial destructor;
     */
    byte_t m_data[sizeof(T)];
    Err m_err = ErrHasNoValue;

    constexpr void requireValue() const {
        // FIXME: Abortion or throw required.
        abort();
        // throw ErrHasNoValue;
    }
};
}  // namespace xppr

#endif /* UTILITIES_OPTIONAL_HPP */
