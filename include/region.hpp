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

    template<typename Ptr>
    struct basic_region
    {
        constexpr basic_region() noexcept
            : begin(0)
            , end(0)
            , prot(0)
            , shared(false)
            , guarded(false)
            , in_use(false) {}

        Ptr                 begin;
        Ptr                 end;
        protection::storage prot;
        bool                shared;
        bool                guarded;
        bool                in_use;

        constexpr Ptr size() const noexcept { return end - begin; }
        constexpr operator bool() const noexcept { return in_use; }
    };


    using local_region  = basic_region<std::uintptr_t>;
    using remote_region = basic_region<std::uint64_t>;

}

#endif // !VMU_REGION_HPP