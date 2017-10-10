/*
* Copyright 2017 Justas Masiulis
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef VMU_PROTECTION_HPP
#define VMU_PROTECTION_HPP

namespace vmu {

    #if defined(_WIN32)
        using native_protection_t = unsigned long;
    #else
        using native_protection_t = int;
    #endif

}

namespace vmu {

    enum class access : native_protection_t {
        none = 0, read = 1, write = 2, exec = 4
    };


    constexpr inline native_protection_t to_native(access flags);


    constexpr inline access from_native(native_protection_t flags);


    class protection_t {
        native_protection_t _native;

    public:
        explicit constexpr protection_t()
                : _native() {}

        constexpr protection_t(access flags)
                : _native(to_native(flags)) {}

        constexpr protection_t(native_protection_t native) noexcept
                : _native(native) {}

        constexpr bool accessible() const noexcept;
        constexpr bool readable() const noexcept;
        constexpr bool writable() const noexcept;
        constexpr bool executable() const noexcept;

        constexpr native_protection_t native() const noexcept { return _native; }
        constexpr access to_flags() const { return from_native(_native); }

        constexpr operator const native_protection_t&() const noexcept { return _native; }
        constexpr operator native_protection_t&() noexcept { return _native; }
    };


    constexpr access operator|(access lhs, access rhs) noexcept
    {
        return static_cast<access>(static_cast<native_protection_t>(lhs)
                                  | static_cast<native_protection_t>(rhs));
    }

    constexpr access operator&(access lhs, access rhs) noexcept
    {
        return static_cast<access>(static_cast<native_protection_t>(lhs)
                                  & static_cast<native_protection_t>(rhs));
    }

    constexpr access operator^(access lhs, access rhs) noexcept
    {
        return static_cast<access>(static_cast<native_protection_t>(lhs)
                                  ^ static_cast<native_protection_t>(rhs));
    }

    constexpr access operator~(access rhs) noexcept
    {
        return static_cast<access>(~static_cast<native_protection_t>(rhs));
    }

    constexpr access& operator|=(access& lhs, access rhs) noexcept
    {
        lhs = static_cast<access> (static_cast<native_protection_t>(lhs)
                                  | static_cast<native_protection_t>(rhs));

        return lhs;
    }

    constexpr access& operator&=(access& lhs, access rhs) noexcept
    {
        lhs = static_cast<access>(static_cast<native_protection_t>(lhs)
                                 & static_cast<native_protection_t>(rhs));

        return lhs;
    }

    constexpr access& operator^=(access& lhs, access rhs) noexcept
    {
        lhs = static_cast<access>(static_cast<native_protection_t>(lhs)
                                 ^ static_cast<native_protection_t>(rhs));

        return lhs;
    }

}

#ifdef _WIN32

    #include "detail/windows/protection.inl"

#else
    #include "detail/posix/protection.inl"
#endif

#endif // !VMU_PROTECTION_HPP
