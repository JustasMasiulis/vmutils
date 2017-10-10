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

namespace vmu { namespace detail { namespace impl {

    template<std::size_t S>
    struct as_uintptr_of_size;

    template<>
    struct as_uintptr_of_size<4> {
        using type = std::uint32_t;
    };
    template<>
    struct as_uintptr_of_size<8> {
        using type = std::uint64_t;
    };

    template<bool>
    struct pointer_checker {
        constexpr static bool is_checked = false;

        template<class P2, class P1>
        inline static constexpr void check(P1) noexcept {}
    };

#if !defined(VMU_NO_PTR_CHECKING)

    template<>
    struct pointer_checker<true> {
        constexpr static bool is_checked = true;

        template<class P2, class P1>
        inline static constexpr void check(P1 p1)
        {
            if ((typename as_uintptr_of_size<sizeof(P1)>::type) (p1)
                > std::numeric_limits<typename as_uintptr_of_size<sizeof(P2)>::type>::max())
                throw std::overflow_error(
                        "attempt to cast to pointer of insufficient size");
        }
    };

#endif

}}}

namespace vmu { namespace detail {

    #if defined(VMU_NO_PTR_CHECKING)
        constexpr static bool checked_pointers = false;
    #else
        constexpr static bool checked_pointers = true;
    #endif

    template<class T>
    using as_uintptr_t = typename impl::as_uintptr_of_size<sizeof(T)>::type;


    template<typename Px, typename Py>
    inline constexpr Px pointer_cast_unchecked(Py pointer) noexcept
    {
        // C style cast because reinterpret cast cannot do conversion from
        // simple types like long to int.
        // A cast like *reinterpret_cast<Px*>(&pointer); may produce UB due to
        // differing alignment requirements
        return (Px) (pointer);
    }

    template<typename Px, typename Py>
    inline constexpr Px pointer_cast(Py pointer) noexcept(!checked_pointers)
    {
        using my_pointer_checker = impl::pointer_checker<(sizeof(Py) > sizeof(Px))>;
        my_pointer_checker::template check<Px>(pointer);

        return pointer_cast_unchecked<Px>(pointer);
    }


    // TODO do some TMP to avoid casts in some cases
    template<class Address>
    inline constexpr Address advance_ptr(Address pointer, Address size) noexcept
    {
        return pointer_cast_unchecked<Address>(
                pointer_cast_unchecked<as_uintptr_t<Address>>(pointer)
                + pointer_cast_unchecked<as_uintptr_t<Address>>(size));
    }

    template<class Address>
    inline constexpr Address ptr_distance(Address p1, Address p2) noexcept
    {
        return pointer_cast_unchecked<Address>(
                pointer_cast_unchecked<as_uintptr_t<Address>>(p1)
                - pointer_cast_unchecked<as_uintptr_t<Address>>(p2));
    }

}}

#endif // include guard
