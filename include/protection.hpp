#ifndef JM_PROTECTION_PROTECTION_HPP
#define JM_PROTECTION_PROTECTION_HPP

#include "detail/config.hpp"

#if defined(JM_PG_WINDOWS)
#include "detail/windows/basic_types.hpp"
#include "detail/windows/protection_at.hpp"
#elif defined(JM_PG_LINUX)
#define 
#elif defined(JM_PG_APPLE)
#endif

namespace vmu { namespace protection {

    enum class flags : native_protection_t
    {
        none = 0,
        read = 1,
        write = 2,
        exec = 4
    };


    native_protection_t to_native(flags flags)
    {

    }

    flags to_flags(native_protection_t flags)
    {

    }

    struct storage
    {
        native_protection_t _native;

    public:
        bool accessible();
        bool readable();
        bool writable();
        bool executable();

        
        native_protection_t native() { return _native; }
        operator const native_protection_t&() const noexcept { return _native; }
        operator native_protection_t&() noexcept { return _native; }
    };


    constexpr flags operator |(flags lhs, flags rhs) noexcept
    {
        return static_cast<flags>(static_cast<native_protection_t>(lhs)
                                        | static_cast<native_protection_t>(rhs));
    }

    constexpr flags operator &(flags lhs, flags rhs) noexcept
    {
        return static_cast<flags>(static_cast<native_protection_t>(lhs)
                                        & static_cast<native_protection_t>(rhs));
    }

    constexpr flags operator ^(flags lhs, flags rhs) noexcept
    {
        return static_cast<flags>(static_cast<native_protection_t>(lhs)
                                        ^ static_cast<native_protection_t>(rhs));
    }

    constexpr flags operator ~(flags rhs) noexcept
    {
        return static_cast<flags>(~static_cast<native_protection_t>(rhs));
    }

    constexpr flags& operator |=(flags& lhs, flags rhs) noexcept
    {
        lhs = static_cast<flags> (static_cast<native_protection_t>(lhs)
                                        | static_cast<native_protection_t>(rhs));

        return lhs;
    }

    constexpr flags& operator &=(flags& lhs, flags rhs) noexcept
    {
        lhs = static_cast<flags>(static_cast<native_protection_t>(lhs)
                                       & static_cast<native_protection_t>(rhs));

        return lhs;
    }

    constexpr flags& operator ^=(flags& lhs, flags rhs) noexcept
    {
        lhs = static_cast<flags>(static_cast<native_protection_t>(lhs)
                                       ^ static_cast<native_protection_t>(rhs));

        return lhs;
    }

}}

#endif // !JM_PROTECTION_PROTECTION_HPP
