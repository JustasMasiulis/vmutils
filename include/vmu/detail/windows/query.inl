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

#ifndef VMU_WINDOWS_QUERY_INL
#define VMU_WINDOWS_QUERY_INL

#include "error.hpp"
#include "../../query.hpp"
#include "vmu/detail/address_cast.hpp"

namespace vmu { namespace detail {

    template<class RegionAddress, class InfoT>
    inline basic_region<RegionAddress> parse_info(const InfoT& info) noexcept
    {
        const auto end = uintptr_cast(info.BaseAddress) + uintptr_cast(info.RegionSize);

        if(info.State == mem_free)
            return {address_cast<RegionAddress>(info.BaseAddress)
                    , address_cast<RegionAddress>(end)};

        return {address_cast<RegionAddress>(info.BaseAddress)
                , address_cast<RegionAddress>(end)
                , info.State == mem_reserve ? no_access : info.Protect
                , (info.Type & mem_private) == 0
                , (info.Protect & page_guard) != 0};
    }

    struct native_query {
        using address_type = const void*;

        static MEMORY_BASIC_INFORMATION query(void* handle, address_type address)
        {
            MEMORY_BASIC_INFORMATION info;
            if (VirtualQueryEx(handle, address, &info, sizeof(info)) == 0)
                throw_last_error("VirtualQueryEx() failed");

            return info;
        }
        static MEMORY_BASIC_INFORMATION
        query(void* handle, address_type address, std::error_code& ec) noexcept
        {
            MEMORY_BASIC_INFORMATION info;
            if (VirtualQueryEx(handle, address, &info, sizeof(info)) == 0)
                ec = get_last_error();

            return info;
        }
    };

    struct wow64_query {
        using address_type = std::uint64_t;

        static MEMORY_BASIC_INFORMATION query(void* handle, address_type address)
        {
            if (address >= std::numeric_limits<std::uint32_t>::max())
                throw std::logic_error("not implemented");

            return native_query::query(handle
                                       , address_cast_unchecked<const void*>(address));
        }
        static MEMORY_BASIC_INFORMATION
        query(void* handle, address_type address, std::error_code& ec)
        {
            if (address >= std::numeric_limits<std::uint32_t>::max())
                throw std::logic_error("not implemented");

            return native_query::query(handle
                                       , address_cast_unchecked<const void*>(address)
                                       , ec);
        }
    };

#ifdef _WIN64
    using remote_query = native_query;
#else
    using remote_query = wow64_query;
#endif

    template<class RegionAddress, class QueryT, class Address>
    inline basic_region<RegionAddress> query_impl(void* handle, Address address)
    {
        using addr_t = typename QueryT::address_type;
        return parse_info<RegionAddress>(QueryT::query(handle
                                                       , address_cast<addr_t>(address)));
    };

    template<class RegionAddress, class QueryT, class Address>
    inline basic_region<RegionAddress>
    query_impl(void* handle, Address addr, std::error_code& ec)
    noexcept(!detail::ptr_checked<typename QueryT::address_type, Address>::value)
    {
        using addr_t = typename QueryT::address_type;
        return parse_info<RegionAddress>(QueryT::query(handle
                                                       , address_cast<addr_t>(addr)
                                                       , ec));
    };

}}

namespace vmu {

    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address)
    {
        return detail::query_impl<RegionAddress, detail::native_query>(
                detail::GetCurrentProcess(), address);
    }
    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec)
    {
        return detail::query_impl<RegionAddress, detail::native_query>(
                detail::GetCurrentProcess(), address, ec);
    }

    template<class RegionAddress = std::uint64_t, class Address>
    inline basic_region<RegionAddress> query(native_handle_t handle, Address address)
    {
        return detail::query_impl<RegionAddress, detail::remote_query>(handle, address);
    }
    template<class RegionAddress = std::uint64_t, class Address>
    inline basic_region<RegionAddress>
    query(native_handle_t handle, Address address, std::error_code& ec)
    {
        return detail::query_impl<RegionAddress, detail::remote_query>(handle
                                                                       , address
                                                                       , ec);
    }

}

#endif // include guard
