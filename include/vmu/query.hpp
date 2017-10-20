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

#ifndef VMU_QUERY_HPP
#define VMU_QUERY_HPP

#include <vector>
#include <system_error>
#include "region.hpp"

namespace vmu {

#if defined(_WIN32)
    using native_handle_t = void*;
#elif defined(__apple__)
    using native_handle_t = ::mach_port_t;
#else
    using native_handle_t = ::pid_t;
#endif

    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address);

    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec);

    template<class RegionAddress = std::uintptr_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(Address begin, Address end);

    template<class RegionAddress = std::uintptr_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(Address begin, Address end, std::error_code& ec);


    template<class RegionAddress = std::uint64_t, class Address>
    inline basic_region<RegionAddress> query(native_handle_t handle, Address address);

    template<class RegionAddress = std::uint64_t, class Address>
    inline basic_region<RegionAddress>
    query(native_handle_t handle, Address address, std::error_code& ec);

    template<class RegionAddress = std::uint64_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end);

    template<class RegionAddress = std::uint64_t, class Address>
    inline std::vector<basic_region<RegionAddress>>
    query_range(native_handle_t handle, Address begin, Address end, std::error_code& ec);

}

#if defined(_WIN32)
    #include "detail/windows/query.inl"
    #include "detail/query_range_using_query.inl"
#elif defined(__APPLE__)
    #include "detail/osx/query.inl"
    #include "detail/query_range_using_query.inl"
#else
    #include "detail/linux/query.inl"
#endif

#endif // include guard
