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

#ifndef JM_PROTECTION_WINDOWS_PROTECT_MEMORY_HPP
#define JM_PROTECTION_WINDOWS_PROTECT_MEMORY_HPP

#include <cstdint>
#include <system_error>
#include "winapi.hpp"

namespace vmu { namespace detail {

    using native_protection_t = winapi::DWORD;

    inline void protect_memory(std::uintptr_t begin
                               , std::uintptr_t end
                               , winapi::DWORD native_prot)
    {
        winapi::DWORD old;
        if (winapi::VirtualProtect(reinterpret_cast<void*>(begin)
            , end - begin
            , native_prot
            , &old) == FALSE)
            throw std::system_error(std::error_code(static_cast<int>(winapi::GetLastError()), std::system_category())
                                    , "VirtualProtectEx() failed");
    }


    inline void protect_memory(std::uintptr_t begin
                               , std::uintptr_t end
                               , winapi::DWORD native_prot
                               , std::error_code& ec) noexcept
    {
        winapi::DWORD old;
        if (winapi::VirtualProtect(reinterpret_cast<void*>(begin)
            , end - begin
            , native_prot
            , &old) == FALSE)
            ec = std::error_code(static_cast<int>(winapi::GetLastError()), std::system_category());
    }

}} // namespace jm::detail

#endif // !JM_PROTECTION_WINDOWS_PROTECT_MEMORY_HPP
