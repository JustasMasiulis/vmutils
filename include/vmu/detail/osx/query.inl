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

#ifndef VMU_OSX_QUERY_INL
#define VMU_OSX_QUERY_INL

#include "../checked_pointers.hpp"
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

    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query_impl(vm_map_t handle, Address address)
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
            return {detail::pointer_cast<RegionAddress>(region_base)
                    , detail::pointer_cast<RegionAddress>(region_size)
                    , protection_t(0)
                    , false
                    , false
                    , false};

        return {detail::pointer_cast<RegionAddress>(region_base)
                , detail::pointer_cast<RegionAddress>(region_size)
                , info.protection
                , is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD
                , true};
    }

    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress>
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
            return {detail::pointer_cast<RegionAddress>(region_base)
                    , detail::pointer_cast<RegionAddress>(region_size)
                    , protection_t(0)
                    , false
                    , false
                    , false};

        return {detail::pointer_cast<RegionAddress>(region_base)
                , detail::pointer_cast<RegionAddress>(region_size)
                , info.protection
                , detail::is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD
                , true};
    }

}}

namespace vmu {

    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(Address address)
    {
        return detail::query_impl<RegionAddress>(::mach_task_self(), address);
    };
    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec)
    {
        return detail::query_impl<RegionAddress>(::mach_task_self(), address, ec);
    }


    template<class RegionAddress, class Address>
    inline remote_region query(native_handle_t handle, Address address)
    {
        return detail::query_impl<RegionAddress>(handle, address);
    }
    template<class RegionAddress, class Address>
    inline remote_region query(native_handle_t handle, Address address, std::error_code& ec)
    {
        return detail::query_impl<RegionAddress>(::mach_task_self(), address, ec);
    }
    
}

#endif // include guard
