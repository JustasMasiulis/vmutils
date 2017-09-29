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

namespace vmu { namespace protection {

        enum class flags : native_protection_t {
            none = 0,
            read = 1,
            write = 2,
            exec = 4
        };


        constexpr inline native_protection_t to_native(flags flags);


        constexpr inline flags from_native(native_protection_t flags);


        class storage {
            native_protection_t _native;

        public:
            constexpr storage(flags flags)
                    : _native(to_native(flags)) {}

            constexpr storage(native_protection_t native) noexcept
                    : _native(native) {}

            constexpr bool accessible() const noexcept;
            constexpr bool readable() const noexcept;
            constexpr bool writable() const noexcept;
            constexpr bool executable() const noexcept;

            constexpr native_protection_t native() const noexcept { return _native; }
            constexpr flags to_flags() const { return from_native(_native); }

            constexpr operator const native_protection_t&() const noexcept { return _native; }
            constexpr operator native_protection_t&() noexcept { return _native; }
        };


        constexpr flags operator|(flags lhs, flags rhs) noexcept
        {
            return static_cast<flags>(static_cast<native_protection_t>(lhs) | static_cast<native_protection_t>(rhs));
        }

        constexpr flags operator&(flags lhs, flags rhs) noexcept
        {
            return static_cast<flags>(static_cast<native_protection_t>(lhs) & static_cast<native_protection_t>(rhs));
        }

        constexpr flags operator^(flags lhs, flags rhs) noexcept
        {
            return static_cast<flags>(static_cast<native_protection_t>(lhs) ^ static_cast<native_protection_t>(rhs));
        }

        constexpr flags operator~(flags rhs) noexcept
        {
            return static_cast<flags>(~static_cast<native_protection_t>(rhs));
        }

        constexpr flags& operator|=(flags& lhs, flags rhs) noexcept
        {
            lhs = static_cast<flags> (static_cast<native_protection_t>(lhs) | static_cast<native_protection_t>(rhs));

            return lhs;
        }

        constexpr flags& operator&=(flags& lhs, flags rhs) noexcept
        {
            lhs = static_cast<flags>(static_cast<native_protection_t>(lhs) & static_cast<native_protection_t>(rhs));

            return lhs;
        }

        constexpr flags& operator^=(flags& lhs, flags rhs) noexcept
        {
            lhs = static_cast<flags>(static_cast<native_protection_t>(lhs) ^ static_cast<native_protection_t>(rhs));

            return lhs;
        }

    }
}

#ifdef _WIN32
    #include "detail/windows/protection.inl"
#else
    #include "detail/posix/protection.inl"
#endif

#endif // !VMU_PROTECTION_HPP
