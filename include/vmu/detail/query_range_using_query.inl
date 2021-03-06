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

#ifndef VMU_QUERY_RANGE_USING_QUERY_INL
#define VMU_QUERY_RANGE_USING_QUERY_INL

#include "../query.hpp"

namespace vmu {

    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>> query_range(Address begin, Address end)
    {
        std::vector<basic_region<RegionAddress>> regions;
        auto       checked_begin = detail::address_cast<RegionAddress>(begin);
        const auto checked_end   = detail::address_cast<RegionAddress>(end);

        while (checked_begin < checked_end) {
            regions.emplace_back(query<RegionAddress>(checked_begin));
            checked_begin = regions.back().end();
        }

        return regions;
    }

    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(Address begin, Address end, std::error_code& ec)
    {
        std::vector<basic_region<RegionAddress>> regions;
        auto       checked_begin = detail::address_cast<RegionAddress>(begin);
        const auto checked_end   = detail::address_cast<RegionAddress>(end);

        while (checked_begin < checked_end) {
            regions.emplace_back(query<RegionAddress>(checked_begin, ec));
            if(ec)
                return regions;
            checked_begin = regions.back().end();
        }

        return regions;
    }


    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end)
    {
        std::vector<basic_region<RegionAddress>> regions;
        auto       checked_begin = detail::address_cast<RegionAddress>(begin);
        const auto checked_end   = detail::address_cast<RegionAddress>(end);

        while (checked_begin < checked_end) {
            regions.emplace_back(query<RegionAddress>(handle, checked_begin));
            checked_begin = regions.back().end();
        }

        return regions;
    }

    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end, std::error_code& ec)
    {
        std::vector<basic_region<RegionAddress>> regions;
        auto       checked_begin = detail::address_cast<RegionAddress>(begin);
        const auto checked_end   = detail::address_cast<RegionAddress>(end);

        while (checked_begin < checked_end) {
            regions.emplace_back(query<RegionAddress>(handle, checked_begin, ec));
            if(ec)
                return regions;
            checked_begin = regions.back().end();
        }

        return regions;
    }
}

#endif // !VMU_WINDOWS_QUERY_INL
