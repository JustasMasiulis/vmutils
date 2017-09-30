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

namespace vmu { namespace detail {

    inline unsigned long page_size_impl() noexcept
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return info.dwPageSize;
    }

}}

namespace vmu {

    inline unsigned long page_size() noexcept
    {
        const static auto size = detail::page_size_impl();
        return size;
    }


    inline local_region query(std::uintptr_t address)
    {
        detail::MEMORY_BASIC_INFORMATION info;
        if (detail::VirtualQuery(reinterpret_cast<const void*>(address)
                                 , &info
                                 , sizeof(info)) == 0)
            detail::throw_last_error("VirtualQuery() failed");

        if (info.State == detail::mem_reserve)
            info.Protect = PAGE_NOACCESS;

        const auto base = reinterpret_cast<std::uintptr_t>(info.BaseAddress);
        return {base, info.RegionSize, info.Protect, (info.Type & detail::mem_private)
                                                     == 0, (info.Protect & PAGE_GUARD),
                info.State != detail::mem_free};
    }
    inline local_region query(std::uintptr_t address, std::error_code& ec)
    {
        detail::MEMORY_BASIC_INFORMATION info;
        if (detail::VirtualQuery(reinterpret_cast<const void*>(address)
                                 , &info
                                 , sizeof(info)) == 0)
            ec = detail::get_last_error();

        if (info.State == detail::mem_reserve)
            info.Protect = PAGE_NOACCESS;

        const auto base = reinterpret_cast<std::uintptr_t>(info.BaseAddress);
        return {base, info.RegionSize, info.Protect, (info.Type & detail::mem_private)
                                                     == 0, (info.Protect & PAGE_GUARD)
                                                           != 0, info.State
                                                                 != detail::mem_free};
    }

    inline std::vector<local_region> query_range(std::uintptr_t begin, std::uintptr_t end)
    {
        std::vector<local_region> regions;
        while (begin < end) {
            regions.emplace_back(query(begin));
            begin = regions.back().end();
        }

        return regions;
    }
    inline std::vector<local_region>
    query_range(std::uintptr_t begin, std::uintptr_t end, std::error_code& ec)
    {
        std::vector<local_region> regions;
        while (begin < end) {
            regions.emplace_back(query(begin, ec));
            if (ec)
                return regions;

            begin = regions.back().end();
        }

        return regions;
    }


    template<typename Handle>
    inline remote_region query(Handle handle, std::uint64_t address)
    {
        throw std::logic_error("not implemented");
    }
    template<typename Handle>
    inline remote_region query(Handle handle, std::uint64_t address, std::error_code& ec)
    {
        throw std::logic_error("not implemented");
    }

    template<typename Handle>
    inline std::vector<remote_region>
    query_range(Handle handle, std::uint64_t begin, std::uint64_t end)
    {
        throw std::logic_error("not implemented");
    }
    template<typename Handle>
    inline std::vector<remote_region> query_range(Handle handle
                                                  , std::uint64_t begin
                                                  , std::uint64_t end
                                                  , std::error_code& ec)
    {
        throw std::logic_error("not implemented");
    }
}

#endif // !VMU_WINDOWS_QUERY_INL
