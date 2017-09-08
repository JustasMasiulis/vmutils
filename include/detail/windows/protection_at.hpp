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

#ifndef JM_PROTECTION_WINDOWS_PROTECTION_AT_HPP
#define JM_PROTECTION_WINDOWS_PROTECTION_AT_HPP

#include "winapi.hpp"
#include <cstdint>


namespace jm { namespace detail {

    inline protections_at(winapi::HANDLE handle, std::uintptr_t begin, std::uintptr_t end)
    {


        std::vector<spm::memory::region> regions;
        SPM_NATIVE_SUSBSYSTEM(handle, {
            for (auto cur_addr = range.start; cur_addr < range.end;) {
                const auto info = native.query(pointer_cast<subsystem_t::ptr>(range.start));
                const auto begin = cur_addr;
                cur_addr += info.RegionSize;

                regions.emplace_back(begin, cur_addr, info.Protect);
            }
        });

        return regions;
    }

}}

#endif // !JM_PROTECTION_WINDOWS_PROTECTION_AT_HPP