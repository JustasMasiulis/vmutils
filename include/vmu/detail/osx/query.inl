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

#include "../../query.hpp"
#include <mach/mach.h>

namespace vmu { namespace detail {

    inline bool is_shared(int sharing) noexcept
    {
        return sharing == SM_SHARED || sharing == SM_TRUESHARED || sharing == SM_SHARED_ALIASED;
    }

}}

namespace vmu {

    inline local_region query(std::uintptr_t address)
    {
        // The address is aligned to the enclosing region
        ::mach_vm_address_t       region_base = address;
        ::mach_vm_size_t          region_size = 0;
        ::vm_region_extended_info info;
        ::mach_msg_type_number_t  info_size   = sizeof(::vm_region_extended_info);
        ::mach_port_t             object_name = 0;

        const auto kr = ::mach_vm_region(::mach_task_self()
                                         , &region_base
                                         , &region_size
                                         , VM_REGION_EXTENDED_INFO
                                         , reinterpret_cast<::vm_region_info_t>(&info)
                                         , &info_size
                                         , &object_name);

        if (kr != KERN_SUCCESS)
            throw std::system_error(std::error_code(kr, std::system_category()), "mach_vm_region() failed");

        if (region_base > address)
            return {static_cast<std::uintptr_t>(region_base)
                    , static_cast<std::uintptr_t>(region_size)
                    , protection::storage(0)
                    , false
                    , false
                    , false};

        return {static_cast<std::uintptr_t>(region_base)
                , static_cast<std::uintptr_t>(region_size)
                , info.protection
                , detail::is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD
                , true};
    };

    inline local_region query(std::uintptr_t address, std::error_code& ec)
    {
        // The address is aligned to the enclosing region
        ::mach_vm_address_t       region_base = address;
        ::mach_vm_size_t          region_size = 0;
        ::vm_region_extended_info info;
        ::mach_msg_type_number_t  info_size   = sizeof(::vm_region_extended_info);
        ::mach_port_t             object_name = 0;

        const auto kr = ::mach_vm_region(::mach_task_self()
                                         , &region_base
                                         , &region_size
                                         , VM_REGION_EXTENDED_INFO
                                         , reinterpret_cast<::vm_region_info_t>(&info)
                                         , &info_size
                                         , &object_name);

        if (kr != KERN_SUCCESS) {
            ec = std::error_code(kr, std::system_category());
            return {};
        }

        if (region_base > address)
            return {static_cast<std::uintptr_t>(region_base)
                    , static_cast<std::uintptr_t>(region_size)
                    , protection::storage(0)
                    , false
                    , false
                    , false};

        return {static_cast<std::uintptr_t>(region_base)
                , static_cast<std::uintptr_t>(region_size)
                , info.protection
                , detail::is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD
                , true};
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
    inline std::vector<local_region> query_range(std::uintptr_t begin, std::uintptr_t end, std::error_code& ec)
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
    inline remote_region query(const Handle& handle, std::uint64_t address)
    {

    }
    template<typename Handle>
    inline remote_region query(const Handle& handle, std::uint64_t address, std::error_code& ec)
    {

    }

    template<typename Handle>
    inline std::vector<remote_region> query_range(const Handle& handle, std::uint64_t begin, std::uint64_t end)
    {

    }
    template<typename Handle>
    inline std::vector<remote_region>
    query_range(const Handle& handle, std::uint64_t begin, std::uint64_t end, std::error_code& ec)
    {

    }
}

#endif // !VMU_LINUX_QUERY_INL
