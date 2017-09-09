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
* See the License for the specific language governing protectionand
* limitations under the License.
*/

#ifndef JM_PROTECTION_WINDOWS_PROTECTION_AT_HPP
#define JM_PROTECTION_WINDOWS_PROTECTION_AT_HPP

#include <cstdint>
#include "winapi.hpp"

namespace jm { namespace detail {

    inline winapi::DWORD protection_at(std::uintptr_t address)
    {
        winapi::MEMORY_BASIC_INFORMATION info;
        if (winapi::VirtualQuery(reinterpret_cast<const void*>(address)
            , &info
            , sizeof(info)) == 0)
            throw std::system_error();
    }

}}

#endif // !JM_PROTECTION_WINDOWS_PROTECTION_AT_HPP