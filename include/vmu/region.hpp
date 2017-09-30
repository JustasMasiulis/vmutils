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

#include <cstdint>
#include "protection.hpp"

namespace vmu {

    /// \brief The class representing memory region
    /// \tparam Ptr The type representing pointer in address space
    template<typename Ptr>
    struct basic_region {
        constexpr basic_region() noexcept
                : base_address(0)
                , size(0)
                , prot(0)
                , shared(false)
                , guarded(false)
                , in_use(false) {}

        constexpr basic_region(Ptr base_
                               , Ptr size_
                               , protection::storage protection
                               , bool shared_
                               , bool guarded_
                               , bool in_use_) noexcept
                : base_address(base_)
                , size(size_)
                , prot(protection)
                , shared(shared_)
                , guarded(guarded_)
                , in_use(in_use_) {}

        Ptr                 base_address;
        Ptr                 size;
        protection::storage prot{};
        bool                shared;
        bool                guarded;
        bool                in_use;

        constexpr Ptr begin() const noexcept { return base_address; }

        constexpr Ptr end() const noexcept { return base_address + size; }

        constexpr explicit operator bool() const noexcept { return in_use; }
    };


    using local_region  = basic_region<std::uintptr_t>;
    using remote_region = basic_region<std::uint64_t>;

}

#endif // include guard
