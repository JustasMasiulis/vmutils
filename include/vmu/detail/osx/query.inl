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
        return sharing == SM_SHARED
               || sharing == SM_TRUESHARED
               || sharing == SM_SHARED_ALIASED;
    }

    extern "C" kern_return_t mach_vm_region(vm_map_t target_task
                                            , mach_vm_address_t* address
                                            , mach_vm_size_t* size
                                            , vm_region_flavor_t flavor
                                            , vm_region_info_t info
                                            , mach_msg_type_number_t* infoCnt
                                            , mach_port_t* object_name);

    template<class Ptr, class Address>
    inline basic_region<Ptr> query_impl(vm_map_t handle, Address address)
    {
        // The address is aligned to the enclosing region
        ::mach_vm_address_t       region_base = address;
        ::mach_vm_size_t          region_size = 0;
        ::vm_region_extended_info info;
        ::mach_msg_type_number_t  info_size   = sizeof(::vm_region_extended_info);
        ::mach_port_t             object_name = 0;

        const auto kr = mach_vm_region(::mach_task_self()
                                       , &region_base
                                       , &region_size
                                       , VM_REGION_EXTENDED_INFO
                                       , reinterpret_cast<::vm_region_info_t>(&info)
                                       , &info_size
                                       , &object_name);

        if (kr != KERN_SUCCESS)
            throw std::system_error(std::error_code(kr, std::system_category())
                                    , "mach_vm_region() failed");

        if (region_base > address)
            return {(Ptr) (region_base) // TODO checked pointer cast
                    , (Ptr) (region_size), protection::storage(0), false, false, false};

        // cannot use reinterpret_cast because it will fail on stuff like unsigned long ->
        // unsigned int etc
        return {(Ptr) (region_base)
                , (Ptr) (region_size)
                , info.protection
                , detail::is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD
                , true};
    }

    template<class Ptr, class Address>
    inline basic_region<Ptr>
    query_impl(vm_map_t handle, Address address, std::error_code& ec)
    {
        // The address is aligned to the enclosing region
        ::mach_vm_address_t       region_base = address;
        ::mach_vm_size_t          region_size = 0;
        ::vm_region_extended_info info;
        ::mach_msg_type_number_t  info_size   = sizeof(::vm_region_extended_info);
        ::mach_port_t             object_name = 0;

        const auto kr = mach_vm_region(::mach_task_self()
                                       , &region_base
                                       , &region_size
                                       , VM_REGION_EXTENDED_INFO
                                       , reinterpret_cast<::vm_region_info_t>(&info)
                                       , &info_size
                                       , &object_name);

        if (kr != KERN_SUCCESS)
            ec = std::error_code(kr, std::system_category());

        if (region_base > address)
            return {(Ptr) (region_base)
                    , (Ptr) (region_size)
                    , protection::storage(0)
                    , false
                    , false
                    , false};

        // cannot use reinterpret_cast because it will fail on stuff like unsigned long ->
        // unsigned int etc
        return {(Ptr) (region_base)
                , (Ptr) (region_size)
                , info.protection
                , detail::is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD
                , true};
    }

}}

namespace vmu {

    inline local_region query(std::uintptr_t address)
    {
        return detail::query_impl<std::uintptr_t>(::mach_task_self(), address);
    };

    inline local_region query(std::uintptr_t address, std::error_code& ec)
    {
        return detail::query_impl<std::uintptr_t>(::mach_task_self(), address, ec);
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
        return detail::query_impl<std::uint64_t>(handle, address);
    }
    template<typename Handle>
    inline remote_region query(Handle handle, std::uint64_t address, std::error_code& ec)
    {
        return detail::query_impl<std::uint64_t>(::mach_task_self(), address, ec);
    }

    template<typename Handle>
    inline std::vector<remote_region>
    query_range(Handle handle, std::uint64_t begin, std::uint64_t end)
    {
        std::vector<remote_region> regions;
        while (begin < end) {
            regions.emplace_back(query(handle, begin));
            begin = regions.back().end();
        }

        return regions;
    }
    template<typename Handle>
    inline std::vector<remote_region> query_range(Handle handle
                                                  , std::uint64_t begin
                                                  , std::uint64_t end
                                                  , std::error_code& ec)
    {
        std::vector<remote_region> regions;
        while (begin < end) {
            regions.emplace_back(query(handle, begin, ec));
            if (ec)
                return regions;

            begin = regions.back().end();
        }

        return regions;
    }
}

#endif // !VMU_LINUX_QUERY_INL
