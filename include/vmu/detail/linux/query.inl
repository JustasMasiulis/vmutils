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

#ifndef VMU_LINUX_QUERY_INL
#define VMU_LINUX_QUERY_INL

#include "../checked_pointers.hpp"
#include "../../query.hpp"
#include <fstream>
#include <sys/types.h>
#include <unistd.h>

namespace vmu { namespace detail {

    inline vmu::protection_t transform_prot(char (&prot)[4]) noexcept
    {
        return {(prot[0] != '-') | ((prot[1] != '-') * 2) | ((prot[2] != '-') * 4)};
    }

    template<class Region>
    inline void fill_region_gaps(Region& regions)
    {
        for (std::size_t i = 1; i < regions.size(); ++i)
            if (regions[i].begin() != regions[i - 1].end()) {
                regions.emplace(regions.begin() + i
                                , regions[i - 1].end()
                                , regions[i].begin() - regions[i - 1].end()
                                , 0
                                , false
                                , false
                                , false);
                // increment i twice
                ++i;
            }
    }

    template<class RegionAddress>
    inline basic_region<RegionAddress> query_impl(std::ifstream& maps, std::uint64_t address)
    {
        std::uint64_t begin;
        std::uint64_t end = 0;
        char          prot[4];

        for (; maps; maps.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
            maps >> std::hex >> begin;
            // address is in free memory and the last end is the beginning of the region
            if (begin > address)
                return {pointer_cast<RegionAddress>(end)
                        , pointer_cast<RegionAddress>(begin - end)
                        , 0
                        , false
                        , false
                        , false};

            // ignore the dash between addresses
            maps.ignore();
            maps >> std::hex >> end;

            if (end < address)
                continue;

            // ignore the space
            maps.ignore();
            maps.read(prot, 4);

            return {pointer_cast<RegionAddress>(begin)
                    , pointer_cast<RegionAddress>(end - begin)
                    , transform_prot(prot)
                    , prot[3] != '-'
                    , false
                    , true};
        }

        return {pointer_cast<RegionAddress>(end)
                , detail::pointer_cast_unchecked<RegionAddress>(
                        std::numeric_limits<as_uintptr_t<RegionAddress>>::max())
                , 0
                , false
                , false
                , false};
    }

    template<class RegionAddress>
    inline std::vector<basic_region<RegionAddress>>
    query_range_impl(std::ifstream& maps, std::uint64_t first, std::uint64_t last)
    {
        std::uint64_t                            begin;
        std::uint64_t                            end = 0;
        char                                     prot[4];
        std::vector<basic_region<RegionAddress>> regions;

        for (; maps; maps.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
            maps >> std::hex >> begin;
            if (begin > last)
                break;

            // ignore the dash between addresses
            maps.ignore();
            maps >> std::hex >> end;

            if (end < first)
                continue;

            // ignore the space
            maps.ignore();
            maps.read(prot, 4);

            regions.emplace_back(detail::pointer_cast<RegionAddress>(begin)
                                 , detail::pointer_cast<RegionAddress>(end - begin)
                                 , transform_prot(prot)
                                 , prot[3] != '-'
                                 , false
                                 , true);
        }

        // we now have the regions, but still need to find if there are any gaps and
        // fill them with free memory regions
        fill_region_gaps(regions);
        return regions;
    }

}}

namespace vmu {

    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(Address address)
    {
        return query<RegionAddress>(::getpid(), detail::pointer_cast<std::uint64_t>
                (address));
    }
    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec)
    {
        return query<RegionAddress>(::getpid(), address, ec);
    }

    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>> query_range(Address begin, Address end)
    {
        return query_range<RegionAddress>(::getpid(), begin, end);
    }
    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(Address begin, Address end, std::error_code& ec)
    {
        return query_range<RegionAddress>(::getpid(), begin, end, ec);
    }


    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(native_handle_t handle, Address address)
    {
        std::ifstream maps;
        maps.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        maps.open("/proc/" + std::to_string(handle) + "/maps");

        return detail::query_impl<RegionAddress>(maps,
                detail::pointer_cast<std::uint64_t>(address));
    }
    template<class RegionAddress, class Address>
    inline remote_region query(native_handle_t handle, Address address, std::error_code& ec)
    {
        std::ifstream maps("/proc/" + std::to_string(static_cast<int>(handle)) + "/maps");
        if (!maps.is_open()) {
            ec = std::make_error_code(std::errc::io_error);
            return {};
        }

        return detail::query_impl<RegionAddress>(maps,
                detail::pointer_cast<std::uint64_t>(address));
    }

    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end)
    {
        std::ifstream maps;
        maps.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        maps.open("/proc/" + std::to_string(handle) + "/maps");

        return detail::query_range_impl<RegionAddress>(maps,
                detail::pointer_cast<std::uint64_t>(begin),
                detail::pointer_cast<std::uint64_t>(end));
    }
    template<class RegionAddress, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end, std::error_code& ec)
    {
        std::ifstream maps("/proc/" + std::to_string(static_cast<int>(handle)) + "/maps");
        if (!maps.is_open()) {
            ec = std::make_error_code(std::errc::io_error);
            return {};
        }

        return detail::query_range_impl<RegionAddress>(maps,
                detail::pointer_cast<std::uint64_t>(begin),
                detail::pointer_cast<std::uint64_t>(end));
    }

}

#endif // include guard
