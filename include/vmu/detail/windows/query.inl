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
#include "../checked_pointers.hpp"

namespace vmu { namespace detail {

    inline unsigned long page_size_impl() noexcept
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return info.dwPageSize;
    }

    template<class RegionAddress, class InfoT>
    inline basic_region<RegionAddress> parse_info(InfoT& info) noexcept
    {
        if (info.State == mem_reserve)
            info.Protect = no_access;

        return {detail::pointer_cast<RegionAddress>(info.BaseAddress)
                , info.RegionSize
                , info.Protect
                , (info.Type & mem_private) == 0
                , (info.Protect & page_guard) != 0
                , info.State != mem_free};
    }

    struct native_query {
        using address_type = const void*;

        static vmu::detail::MEMORY_BASIC_INFORMATION query(void* handle, address_type address)
        {
            vmu::detail::MEMORY_BASIC_INFORMATION info;
            if (vmu::detail::VirtualQueryEx(handle, address, &info, sizeof(info)) == 0)
                throw_last_error("VirtualQueryEx() failed");

            return info;
        }
        static vmu::detail::MEMORY_BASIC_INFORMATION
        query(void* handle, address_type address, std::error_code& ec) noexcept
        {
            vmu::detail::MEMORY_BASIC_INFORMATION info;
            if (vmu::detail::VirtualQueryEx(handle, address, &info, sizeof(info)) == 0)
                ec = get_last_error();

            return info;
        }
            
    struct wow64_query {
        using address_type = std::uint64_t;

        static vmu::detail::MEMORY_BASIC_INFORMATION query(void* handle, address_type address)
        {
            throw std::logic_error("not implemented");
        }
        static vmu::detail::MEMORY_BASIC_INFORMATION
        query(void* handle, address_type address, std::error_code& ec)
        {
            throw std::logic_error("not implemented");
        }
    };

    template<class RegionAddress, class QueryT, class Address>
    inline basic_region<RegionAddress> query_impl(void* handle, Address address)
    {
        auto info{QueryT::query(handle
                                , detail::pointer_cast<QueryT::address_type>(address))};

        return parse_info(info);
    };

    template<class RegionAddress, class QueryT, class Address>
    inline basic_region<RegionAddress> query_impl(void* handle
                                                  , Address addr
                                                  , std::error_code& ec) noexcept(!checked_pointers)
    {
        auto info{QueryT::query(handle
                                , detail::pointer_cast<QueryT::address_type>(addr)
                                , ec)};

        return parse_info(info);
    };

}}

namespace vmu {

    inline unsigned long page_size() noexcept
    {
        const static auto size = detail::page_size_impl();
        return size;
    }


    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address)
    {
        return detail::query_impl<RegionAddress, detail::native_query>(GetCurrentProcess()
                                                                       , address);
    }
    template<class RegionAddress = std::uintptr_t, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec)
    {
        return detail::query_impl<RegionAddress, detail::native_query>(GetCurrentProcess()
                                                                       , address
                                                                       , ec);
    }

    template<class RegionAddress = std::uintptr_t, class Address, typename Handle>
    inline remote_region query(Handle handle, Address address)
    {
#ifdef _WIN64
        return detail::query_impl<RegionAddress, detail::native_query>(handle, address);
#else
        return detail::query_impl<RegionAddress, detail::wow64_query>(handle, address);
#endif
    }
    template<class RegionAddress = std::uintptr_t, class Address, typename Handle>
    inline remote_region query(Handle handle, Address address, std::error_code& ec) noexcept
    {
#ifdef _WIN64
        return detail::query_impl<RegionAddress, detail::native_query>(handle, address, ec);
#else
        return detail::query_impl<RegionAddress, detail::wow64_query>(handle, address, ec);
#endif
    }

}

#endif // include guard
