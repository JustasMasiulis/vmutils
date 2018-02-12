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

#include "../../query.hpp"
#include "../error_handlers.hpp"
#include <mach/mach_traps.h>
#include <mach/vm_region.h>
#include <mach/mach_init.h>

namespace vmu { namespace detail {

    constexpr inline bool is_shared(int sharing) noexcept
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

    template<class RegionAddress, class Address, class Handler>
    inline basic_region<RegionAddress>
    query_impl(vm_map_t handle, Address address, Handler&& handler)
    noexcept(Handler::is_noexcept)
    {
        // The address is aligned to the enclosing region
        auto region_base                      = address_cast<::mach_vm_address_t>(address);
        ::mach_vm_size_t          region_size = 0;
        ::vm_region_extended_info info;
        ::mach_msg_type_number_t  info_size   = sizeof(::vm_region_extended_info);
        ::mach_port_t             object_name = 0;

        const auto kr = mach_vm_region(handle
                                       , &region_base
                                       , &region_size
                                       , VM_REGION_EXTENDED_INFO
                                       , reinterpret_cast<::vm_region_info_t>(&info)
                                       , &info_size
                                       , &object_name);
        if(kr != KERN_SUCCESS) {
            handler(kr, "mach_vm_region() failed");
            return {};
        }

        if (region_base > address_cast<::mach_vm_address_t>(address))
            return {address_cast<RegionAddress>(address)
                    , address_cast<RegionAddress>(region_base)};

        return {detail::address_cast_unchecked<RegionAddress>(region_base)
                , advance_ptr(region_base, region_size)
                , info.protection
                , is_shared(info.share_mode)
                , info.user_tag == VM_MEMORY_GUARD};
    }

}}

namespace vmu {

    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(Address address)
    {
        return detail::query_impl<RegionAddress>(::mach_task_self()
                                                 , address
                                                 , detail::handle_with_exception{});
    };
    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(Address address, std::error_code& ec)
    {
        return detail::query_impl<RegionAddress>(::mach_task_self()
                                                 , address
                                                 , detail::handle_with_ec{ec});
    }


    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(native_handle_t handle, Address address)
    {
        return detail::query_impl<RegionAddress>(handle
                                                 , address
                                                 , detail::handle_with_exception{});
    }
    template<class RegionAddress, class Address>
    inline basic_region<RegionAddress> query(native_handle_t handle
                                             , Address address
                                             , std::error_code& ec)
    {
        return detail::query_impl<RegionAddress>(handle
                                                 , address
                                                 , detail::handle_with_ec{ec});
    }

}

#endif // include guard
