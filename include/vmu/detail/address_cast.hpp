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

#ifndef VMU_CHECKED_POINTERS_HPP
#define VMU_CHECKED_POINTERS_HPP

#include <stdexcept>
#include <limits>

#include "uintptr_cast.hpp"

namespace vmu { namespace detail {

    template<bool>
    struct _address_cast_checker {
        constexpr static bool is_checked = false;

        template<class P2, class P1>
        inline static constexpr void check(P1) noexcept {}
    };

#if !defined(VMU_NO_PTR_CHECKING)

    template<>
    struct _address_cast_checker<true> {
        constexpr static bool is_checked = true;

        template<class P2, class P1>
        inline static constexpr void check(P1 p1)
        {
            if ( uintptr_cast(p1) > std::numeric_limits<as_uintptr_t<P2>>::max())
                throw std::overflow_error(
                        "attempt to cast address to type of insufficient size");
        }
    };

#endif

    template<class P1, class P2>
    struct ptr_checked {
        constexpr static bool value = _address_cast_checker<(sizeof(P2) > sizeof(P1))
                                                      >::is_checked;
    };

    template<typename Px, typename Py>
    inline constexpr Px address_cast(Py ptr) noexcept(!ptr_checked<Px, Py>::value)
    {
        using checker = _address_cast_checker<(sizeof(Py) > sizeof(Px))>;
        checker::template check<Px>(ptr);

        return address_cast_unchecked<Px>(ptr);
    }

    // TODO do some TMP to avoid casts in some cases
    template<class Address>
    inline constexpr Address advance_ptr(Address ptr, Address size) noexcept
    {
        return address_cast_unchecked<Address>(uintptr_cast(ptr) + uintptr_cast(size));
    }

    template<class Address>
    inline constexpr Address ptr_distance(Address p1, Address p2) noexcept
    {
        return address_cast_unchecked<Address>(uintptr_cast(p1) - uintptr_cast(p2));
    }

}}

#endif // include guard
