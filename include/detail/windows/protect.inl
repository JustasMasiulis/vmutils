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

#ifndef VMU_WINDOWS_PROTECT_MEMORY_INL
#define VMU_WINDOWS_PROTECT_MEMORY_INL

#include "error.hpp"
#include "../../protect.hpp"

namespace vmu {

    inline void protect(std::uintptr_t begin
                        , std::uintptr_t end
                        , protection::storage prot)
    {
        detail::winapi::DWORD old;
        if (detail::winapi::VirtualProtect(reinterpret_cast<void*>(begin)
            , end - begin
            , prot.native()
            , &old) == FALSE)
            detail::throw_last_error("VirtualProtect() failed");
    }


    inline void protect(std::uintptr_t begin
                        , std::uintptr_t end
                        , protection::storage prot
                        , std::error_code& ec) noexcept
    {
        detail::winapi::DWORD old;
        if (detail::winapi::VirtualProtect(reinterpret_cast<void*>(begin)
            , end - begin
            , prot.native()
            , &old) == FALSE)
            ec = detail::get_last_error();
    }

} // namespace vmu

#endif // !VMU_WINDOWS_PROTECT_MEMORY_INL
