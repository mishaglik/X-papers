#ifndef ENGINE_RENDERENGINE_COLOR_HPP
#define ENGINE_RENDERENGINE_COLOR_HPP
#include "RenderEngine/GVec.hpp"
#include <cstddef>
#include <cstdint>
#include <Utilities/utils.hpp>

namespace xppr {
    struct Color : vec4f
    {
        constexpr Color() = default;
        
        constexpr Color(const Color& oth)            = default;
        constexpr Color& operator=(const Color& oth) = default;

        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : vec4f{r / 255.f, g / 255.f, b / 255.f, a / 255.f} {}

        constexpr /*implicit*/ Color(uint32_t color) :
            vec4f{
                ((color >> 24) & 0xFF) / 255.f,
                ((color >> 16) & 0xFF) / 255.f,
                ((color >>  8) & 0xFF) / 255.f,
                ((color >>  0) & 0xFF) / 255.f,
            } {}

        constexpr Color(float r, float g, float b, float a = 1.f) : vec4f{r, g, b, a} {
            assertValues();
        }

        constexpr explicit Color(const vec4f& vec) : vec4f(vec) {}
        constexpr explicit Color(const vec3f& vec) : vec4f{vec[0], vec[1], vec[2], 1.f} {}

        constexpr uint8_t r() const {return static_cast<uint8_t>(255 * (*this)[0]);}
        constexpr uint8_t g() const {return static_cast<uint8_t>(255 * (*this)[1]);}
        constexpr uint8_t b() const {return static_cast<uint8_t>(255 * (*this)[2]);}
        constexpr uint8_t a() const {return static_cast<uint8_t>(255 * (*this)[3]);}

        constexpr float rf() const {return (*this)[0];}
        constexpr float gf() const {return (*this)[1];}
        constexpr float bf() const {return (*this)[2];}
        constexpr float af() const {return (*this)[3];}

        constexpr Color& setR(uint8_t r) { (*this)[0] = r / 255.; return *this;}
        constexpr Color& setG(uint8_t g) { (*this)[1] = g / 255.; return *this;}
        constexpr Color& setB(uint8_t b) { (*this)[2] = b / 255.; return *this;}
        constexpr Color& setA(uint8_t a) { (*this)[3] = a / 255.; return *this;}

        constexpr Color& setRf(float r) { (*this)[0] = r; assert(r <= 1.f); assert(r >= 0.f); return *this;}
        constexpr Color& setGf(float g) { (*this)[1] = g; assert(g <= 1.f); assert(g >= 0.f); return *this;}
        constexpr Color& setBf(float b) { (*this)[2] = b; assert(b <= 1.f); assert(b >= 0.f); return *this;}
        constexpr Color& setAf(float a) { (*this)[3] = a; assert(a <= 1.f); assert(a >= 0.f); return *this;}

        constexpr Color& setRGBA (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { return setR(r).setG(g).setB(b).setA(a); }

        constexpr Color& setRGBA (uint8_t color[4]) { return setR(color[0]).setG(color[1]).setB(color[2]).setA(color[3]);}
        constexpr Color& setRGBA (uint32_t color) { 
            return setR((color >> 24) & 0xFF).
                   setG((color >> 16) & 0xFF).
                   setB((color >>  8) & 0xFF).
                   setB((color >>  0) & 0xFF);
        }

        constexpr uint32_t toUInt() const {
            return (r() << 24) | 
                   (g() << 16) | 
                   (b() <<  8) |
                   (a() <<  0);
            }

        constexpr Color& setRGBAf(float r, float g, float b, float a = 255) { return setRf(r).setGf(g).setBf(b).setAf(a);}

        constexpr bool operator==(const Color& oth) const { return toUInt() == oth.toUInt(); }
        constexpr bool operator!=(const Color& oth) const = default;

        /**
         * @brief Color modulation.
         *  Also works as operator*=(const Color& color) due to upcast. 
         * @param v - vector multiply to
         * @return constexpr Color& 
         */
        constexpr Color& operator*=(const vec4f& v) {
            (*this)[0] *= v[0];
            (*this)[1] *= v[1];
            (*this)[2] *= v[2];
            (*this)[3] *= v[3];
            assertValues();
            return *this;
        }

        constexpr Color& operator+=(const Color& color) {
            (*this)[0] += color[0];
            (*this)[1] += color[1];
            (*this)[2] += color[2];
            (*this)[3] += color[3];

            if(rf() > 1.f) [[unlikely]]{ setRf(1.f); }
            if(gf() > 1.f) [[unlikely]]{ setGf(1.f); }
            if(bf() > 1.f) [[unlikely]]{ setBf(1.f); }
            if(af() > 1.f) [[unlikely]]{ setAf(1.f); }
            
            assertValues();
            return *this;
        }

        constexpr Color& operator-=(const Color& color) {
            (*this)[0] += color[0];
            (*this)[1] += color[1];
            (*this)[2] += color[2];
            (*this)[3] += color[3];

            if(rf() < 0.f) [[unlikely]]{ setRf(0.f); }
            if(gf() < 0.f) [[unlikely]]{ setGf(0.f); }
            if(bf() < 0.f) [[unlikely]]{ setBf(0.f); }
            if(af() < 0.f) [[unlikely]]{ setAf(0.f); }
            
            assertValues();
            return *this;
        }

        
    private:
        constexpr void assertValues() const {
            assert(rf() <= 1.f);
            assert(gf() <= 1.f);
            assert(bf() <= 1.f);
            assert(af() <= 1.f);

            assert(rf() >= 0.f);
            assert(gf() >= 0.f);
            assert(bf() >= 0.f);
            assert(af() >= 0.f);
        }
    };
    namespace Colors {
                                           //  R  G  B  A
        constexpr const Color Black       = 0x00'00'00'FF;
        constexpr const Color Red         = 0xFF'00'00'FF;
        constexpr const Color Green       = 0x00'FF'00'FF;
        constexpr const Color Blue        = 0x00'00'FF'FF;
        constexpr const Color Yellow      = 0xFF'FF'00'FF;
        constexpr const Color Magenta     = 0xFF'00'FF'FF;
        constexpr const Color Cyan        = 0x00'FF'FF'FF;
        constexpr const Color White       = 0xFF'FF'FF'FF;
        constexpr const Color Transparent = 0x00'00'00'00;
    
    }

    constexpr Color operator*(const Color& lhs, const vec4f& rhs) {
        return Color(lhs) *= rhs;
    }

    constexpr Color operator*(const vec4f& lhs, const Color& rhs) {
        return rhs * lhs;
    }

    constexpr Color operator+(const Color& lhs, const Color& rhs) {
        return Color(lhs) += rhs;
    }

    constexpr Color operator-(const Color& lhs, const Color& rhs) {
        return Color(lhs) -= rhs;
    }

}
#endif /* ENGINE_RENDERENGINE_COLOR_HPP */
