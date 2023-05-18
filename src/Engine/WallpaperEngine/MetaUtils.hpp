#ifndef ENGINE_WALLPAPERENGINE_METAUTILS_HPP
#define ENGINE_WALLPAPERENGINE_METAUTILS_HPP
#include <Engine/WallpaperEngine/MetaTypes.hpp>
#include <concepts>
#include <cstdint>
#include <utility>
#include <array>
#include <Engine/WallpaperEngine/Connector.hpp>

namespace xppr::meta {

template<class T>
concept Meta = std::derived_from<T, MetaObject> && std::same_as<decltype(T::Type), const MetaType* const>;

template<class T>
requires std::derived_from<T, MetaObject>
T* meta_cast(MetaObject* object, const MetaType* type) {
    if(object->m_type == type) {
        return static_cast<T*>(object);
    }
    return nullptr;
}

template<Meta T>
T* meta_cast2(MetaObject* object) {
    if(object->m_type == T::Type) {
        return static_cast<T*>(object);
    }
    return nullptr;
}

namespace detail {

    namespace traits {
        template<typename T>
        struct is_meta_typed : std::false_type {};

        template<>
        struct is_meta_typed<uint64_t> : std::true_type {};

        template<>
        struct is_meta_typed<const char*> : std::true_type {};
        
        template<>
        struct is_meta_typed<std::string> : std::true_type {};

        template<>
        struct is_meta_typed<MetaObject* > : std::true_type {};

        template<typename T>
        struct is_meta_typed<std::vector<T>> : is_meta_typed<T> {};
    }

    template<size_t s1, size_t s2, size_t ...Idx1, size_t ...Idx2>
    constexpr std::array<char, sizeof...(Idx1) + sizeof...(Idx2)> ccat(std::array<char, s1>lhs, std::array<char, s2> rhs, std::index_sequence<Idx1...>, std::index_sequence<Idx2...>) {
        return {lhs[Idx1]..., rhs[Idx2]...};
    }

    template<size_t s1, size_t s2>
    constexpr std::array<char, s1 + s2 - 1> concat(std::array<char, s1>lhs, std::array<char, s2> rhs) {
        return ccat(lhs, rhs, std::make_index_sequence<s1-1>{}, std::make_index_sequence<s2>{});
    }


    template<class T>
    concept MetaParametr = traits::is_meta_typed<T>::value;

    template<MetaParametr... Ts>
    struct GetSignature;

    template<MetaParametr T, MetaParametr... Ts>
    struct GetSignature<T, Ts...>{
        constexpr static const char* Get() {
            return m_sign.data();
        }
        constexpr static auto m_sign = concat(GetSignature<T>::m_sign, GetSignature<Ts...>::m_sign);
    };

    template<>
    struct GetSignature<> {
        constexpr static const char* Get() { return m_sign.data(); }
        static constexpr const std::array<char, 1> m_sign = {'\0'};
    };

    template<>
    struct GetSignature<uint64_t> {
        constexpr static const char* Get() {return m_sign.data();}
        static constexpr const std::array<char, 2> m_sign = {'L', '\0'};
    };

    template<>
    struct GetSignature<const char*> {
        constexpr static const char* Get() {return m_sign.data();}
        static constexpr const std::array<char, 2> m_sign = {'s', '\0'};
    };

    template<>
    struct GetSignature<std::string> {
        constexpr static const char* Get() {return m_sign.data();}
        static constexpr const std::array<char, 2> m_sign = {'s', '\0'};
    };

    template<>
    struct GetSignature<MetaObject*> {
        constexpr static const char* Get() {return m_sign.data();}
        static constexpr const std::array<char, 2> m_sign = {'O', '\0'};
    };


    template<MetaParametr T>
    struct GetSignature<std::vector<T>> {
        constexpr static const char* Get() {return m_sign.data();}
        static constexpr const std::array<char, 2> lbracket = {'[', 0};
        static constexpr const std::array<char, 2> rbracket = {']', 0};
        static constexpr const auto m_sign = concat(lbracket, concat(GetSignature<T>::m_sign, rbracket));

    };

    template<MetaParametr T>
    struct arg_cast 
    {
        T&& operator()(MetaTyped object) {
            return std::move(std::get<T>(object));
        }
    };

    template<>
    struct arg_cast<const char*> 
    {
        const char* operator()(MetaTyped object) {
            return std::get<std::string>(object).c_str();
        }
    };

    template<MetaParametr T>
    struct arg_cast<std::vector<T>> 
    {
        std::vector<T> operator()(MetaTyped object) {
            std::vector<MetaTyped>& v = std::get<std::vector<MetaTyped>>(object);
            std::vector<T> ret;

            for(size_t i = 0; i < v.size(); ++i) {
                ret.emplace_back(arg_cast<T>{}(v[i]));
            }
            return ret;
        }
    };

    template<typename... Args>
    bool All(bool arg, Args&&... args) {
        return arg && All(std::forward<Args>(args)...);
    }

    template<>
    inline bool All(bool arg) {
        return arg;
    }

    template<class F>
    struct GetCallback;

    template<Meta T, MetaParametr... Args>
    struct GetCallback<  MetaObject* (T::*)(Args...)> {
        using MethType = MetaObject* (T::*)(Args...);

        template<MethType meth>
        static MetaObject* call(ArgPack* ap) {
            return call_impl<meth>(ap, std::make_index_sequence<sizeof...(Args)>{});
        }  

    private:
        template<MethType Meth, std::size_t... Idx>
        static MetaObject* call_impl(ArgPack* ap, std::index_sequence<Idx...>) {
            if(T* self = meta_cast<T>(ap->self, T::Type)) {
                
                // if(!All(arg_check<Args>(ap->m_data[Idx], ap->m_signature[Idx])...)) {
                //     return nullptr;
                // }

                return (self->*Meth)(arg_cast<Args>{}(ap->m_args[Idx])...);
            }
            return nullptr;
        }
    public:
        constexpr static const char* signature = GetSignature<Args...>::Get(); 

    };

    template<Meta T, MetaParametr... Args>
    struct GetCallback<  void (T::*)(Args...)> {
        using MethType = void (T::*)(Args...);

        template<MethType meth>
        static MetaObject* call(ArgPack* ap) {
            return call_impl<meth>(ap, std::make_index_sequence<sizeof...(Args)>{});
        }  

    private:
        template<MethType Meth, std::size_t... Idx>
        static MetaObject* call_impl(ArgPack* ap, std::index_sequence<Idx...>) {
            if(T* self = meta_cast2<T>(ap->self)) {
                
                // if(!All(arg_check<Args>(ap->m_data[Idx], ap->m_signature[Idx])...)) {
                //     return nullptr;
                // }

                (self->*Meth)(arg_cast<Args>{}(ap->m_args[Idx])...);
            }
            return nullptr;
        }
    public:
        constexpr static const char* signature = GetSignature<Args...>::Get(); 
    };

}

constexpr static inline size_t SignatureSize(const char* format) {
    size_t ans = 0;
    while(*format) {
        switch (*format++) {
            case 'L':
            case 's':
            case 'O':
                ans++;
                break;
            case '[':
            case ']':
                break;
            default: 
                return -1ul;
        }
    }
    return ans;
}

}
#define META_METHOD(type, name) \
        {#name, xppr::meta::detail::GetCallback<decltype(&type::name)>::signature, xppr::meta::detail::GetCallback<decltype(&type::name)>::call<&type::name>}



#endif /* ENGINE_WALLPAPERENGINE_METAUTILS_HPP */
