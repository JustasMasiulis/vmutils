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

namespace vmu {

    /// \brief The class representing memory region
    /// \tparam Ptr The type representing pointer in address space
    template<typename Ptr>
    class basic_region {
        Ptr          _begin{static_cast<Ptr>(0)};
        Ptr          _end{static_cast<Ptr>(0)};
        protection_t _protection{0};
        bool         _used{false};
        bool         _shared{false};
#ifndef __linux__
        bool         _guarded{false};
#endif

    public:
        constexpr Ptr begin() const noexcept { return _begin; }

        constexpr Ptr end() const noexcept { return _end; }

        constexpr Ptr size() const noexcept
        {
            return detail::address_cast_unchecked<Ptr>(
                    detail::uintptr_cast(_end) - detail::uintptr_cast(_begin));
        }

        constexpr protection_t protection() const noexcept { return _protection; }

        constexpr bool guarded() const noexcept
        {
#ifndef __linux__
            return _guarded;
#else
            return false;
#endif
        }

        constexpr bool shared() const noexcept { return _shared; };

        constexpr explicit operator bool() const noexcept { return _used; }
    };


    using local_region  = basic_region<std::uintptr_t>;
    using remote_region = basic_region<std::uint64_t>;

}

#endif // include guard
