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

#ifndef VMU_REGION_HPP
#define VMU_REGION_HPP

#include "protection.hpp"
#include "vmu/detail/address_cast.hpp"

// a dirty macro to not have large #ifdef blocks
#ifdef __linux__
    #define VMU_NOT_FOR_LINUX(expr)
    #define VMU_COMMA_NOT_FOR_LINUX(expr)
#else
    #define VMU_NOT_FOR_LINUX(expr) expr
    #define VMU_COMMA_NOT_FOR_LINUX(expr) , expr
#endif

namespace vmu {

    /// \brief The class representing memory region
    /// \tparam Ptr The type representing pointer in address space
    template<class Address>
    class basic_region {
        Address      _begin{static_cast<Address>(0)};
        Address      _end{static_cast<Address>(0)};
        protection_t _protection{0};
        bool         _used{false};
        bool         _shared{false};
        VMU_NOT_FOR_LINUX(bool _guarded{false});

    public:
        using address_type = Address;

        constexpr basic_region() noexcept = default;

        /// \brief Constructs a memory region that is not in use.
        constexpr basic_region(address_type begin, address_type end) noexcept
                : _begin(begin)
                , _end(end) {}

        constexpr basic_region(address_type begin
                               , address_type end
                               , protection_t protection
                               , bool shared
                               VMU_COMMA_NOT_FOR_LINUX(bool guarded)) noexcept
                : _begin(begin)
                , _end(end)
                , _protection(protection)
                , _used(true)
                , _shared(shared)
                VMU_COMMA_NOT_FOR_LINUX(_guarded(guarded)) {}

        /// \brief Returns the base address of the region.
        constexpr address_type begin() const noexcept { return _begin; }

        /// \brief Returns one past the end of the region.
        constexpr address_type end() const noexcept
        {
            return detail::address_cast_unchecked<Address>(detail::uintptr_cast(_end) + 1);
        }

        /// \brief Returns the size of the region.
        constexpr detail::as_uintptr_t<address_type> size() const noexcept
        {
            return detail::uintptr_cast(detail::uintptr_cast(_end) - detail::uintptr_cast(_begin));
        }

        /// \brief Returns the protection of the region.
        constexpr protection_t protection() const noexcept { return _protection; }

        /// \brief Returns whether the memory region is guarded.
        constexpr bool guarded() const noexcept
        {
#ifndef __linux__
            return _guarded;
#else
            return false;
#endif
        }

        /// \brief Returns whether the memory region is shared.
        constexpr bool shared() const noexcept { return _shared; };

        /// \brief Returns whether the memory region is in use.
        constexpr explicit operator bool() const noexcept { return _used; }
    };


    using local_region  = basic_region<std::uintptr_t>;
    using remote_region = basic_region<std::uint64_t>;

}

#endif // include guard
